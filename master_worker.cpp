#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <deque>

#include "utimer.cpp"

using namespace std;

#define vk vector
#define pb push_back

typedef vk<int> vi;
typedef pair<int, int> pii;
typedef vk<pii> vii;

const int ARG_COUNT = 3;
const int MINN = -1e5;
const int MAXX = 1e5 + 10;


/// generate random vector
vi rand_vec(int N) {
    vi randArr(N);
    for (int &x: randArr)
        x = rand() % MAXX;
    return randArr;
}

/// print vector
void print_vec(vi &vec) {
    cout << "\n";
    for (auto x: vec)
        cout << x << "\t";
    cout << "\n\n";
}

/// make ranges for threads
vii make_ranges(int N, int nw, int nt) {
    // number taks, number workers, type number - [0 - even, 1 - odd]
    vi idxList;
    for(int i = 0; i < N; i++)
        if(i%2 == nt)   idxList.pb(i);
    
    int sz = int(idxList.size());
    int chunk = sz / nw;
    int mod = sz % nw;

    int idx = 0;
    vii retList(nw);
    for(int th = 0; th < nw; th++) {
        int st = idx;
        int ei = idx + chunk - 1 + (mod-- > 0);
        retList[th] = make_pair(idxList[st], idxList[ei]);
        idx = ei + 1;
    }
    return retList;
}

class Task {
public:
    int begin = 0, end = 0;
    Task() {}
    Task(int begin, int end) : begin(begin), end(end) {}

    inline bool operator==(const Task &task) {
        return (begin == task.begin) && (end == task.end);
    }
};

class Master {
private:
    int nw, wIdx = 0;
    vii ranges[2];
    int startIndex = 0; // 0 - even, 1 - odd
public:
    Master() {}
    Master(vi Arr, int nw) : nw(nw) {
        int N = Arr.size();
        ranges[0] = make_ranges(N-1, nw, 0); // for even - start index
        ranges[1] = make_ranges(N-1, nw, 1); // for odd - start index
    }

    Task next() {
        Task new_task = Task(ranges[startIndex][wIdx].first, ranges[startIndex][wIdx].second);
        wIdx++;
        return new_task;
    }

    bool hasNext() {
        return wIdx < nw;
    }

    void restart() {
        wIdx = 0;
        startIndex = (startIndex + 1) % 2; // even after odd, odd after even
    }
};

class Worker {
private:
    vk<int> &Arr;
public:
    Worker(vi &Arr) : Arr(Arr) {}

    bool compute(Task &task) {
        bool is_sorted = true;
        for(int i = task.begin; i <= task.end; i+=2) {
            if(Arr[i] > Arr[i+1]) {
                swap(Arr[i], Arr[i+1]);
                if(is_sorted) is_sorted = false;
            }
        }
        return is_sorted;
    }
};

int main(int argc, char *argv[]) {
    // Read Input
    if (argc != ARG_COUNT) {
        cout << "Input Error. Number of argument is wrong..\n";
        exit(0);
    }

    int N = atoi(argv[1]);
    int nw = atoi(argv[2]);

    // input validation
    if (N < 1 || nw == 0) {
        cout << "Invalid Input..\n";
        exit(0);
    }

    vi Arr = rand_vec(N);
    // print_vec(Arr);


    Master master(Arr, nw);
    Worker worker(Arr);

    deque<Task> master_worker[nw];
    deque<bool> worker_master[nw];

    Task EOS(-1, -1);

    utimer *timer = new utimer("MasterWorker Code");
    thread master_thread([&](Master m) {
        bool is_sorted = false;

        while(!is_sorted) {
            is_sorted = true;
            int thIdx = 0;
            while(m.hasNext()) {
                Task task = m.next();
                master_worker[thIdx].push_front(task);
                thIdx = (thIdx + 1) % nw;
            }

            // 
            thIdx = 0;
            int wCount = 0;
            vi visited(nw, 0);
            while(true) {
                //
                if(visited[thIdx] || worker_master[thIdx].empty()) {
                    thIdx = (thIdx + 1) % nw;
                    continue;
                }
                //
                visited[thIdx] = 1;
                if(!worker_master[thIdx].back())
                    is_sorted = false;
                worker_master[thIdx].pop_back();
                thIdx = (thIdx + 1) % nw;
                //
                if(++wCount == nw)
                    break;
            }

            //
            m.restart();

            // for even indices
            thIdx = 0;
            while(m.hasNext()) {
                Task task = m.next();
                master_worker[thIdx].push_front(task);
                thIdx = (thIdx + 1) % nw;
            }

            // 
            thIdx = 0;
            wCount = 0;
            fill(visited.begin(), visited.end(), 0);
            while(true) {
                //
                if(visited[thIdx] || worker_master[thIdx].empty()) {
                    thIdx = (thIdx + 1) % nw;
                    continue;
                }
                //
                visited[thIdx] = 1;
                if(!worker_master[thIdx].back())
                    is_sorted = false;
                worker_master[thIdx].pop_back();
                thIdx = (thIdx + 1) % nw;
                //
                if(++wCount == nw)
                    break;
            }

            //
            m.restart();
        }
        //
        for(int i = 0; i < nw; i++)
            master_worker[i].push_front(EOS);
    }, master);

    vk<thread> worker_threads(nw);	// threads vector
    for (int th = 0; th < nw; th++) {
        worker_threads[th] = thread([&](Worker w, int wIdx) {
            while(true) {
                //
                this_thread::sleep_for(chrono::microseconds(1));
                if(master_worker[wIdx].empty())
                    continue;
                //
                Task task = master_worker[wIdx].back();
                master_worker[wIdx].pop_back();
                if(task == EOS)
                    break;
                // else
                worker_master[wIdx].push_front(w.compute(task));
            }
        }, worker, th);
    }

    master_thread.join();
    for(thread &th: worker_threads) {
        th.join();
    }
    timer->~utimer();


    // Print Sorted Vector
    // print_vec(Arr);

    return 0;
}