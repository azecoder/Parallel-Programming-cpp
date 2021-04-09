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

enum Flag {
    GO_ON,
    RESTART,
    EOS
};

class Task {
public:
    int begin = 0, end = 0;
    bool is_sorted = true;
    Task() {}
    Task(int begin, int end) : begin(begin), end(end), is_sorted(true) {}

    inline bool operator==(const Task &task) {
        return (begin == task.begin) && (end == task.end);
    }
};

class Emitter {
private:
    int nw, wIdx = 0;
    vii ranges[2];
    int startIndex = 0; // 0 - even, 1 - odd
public:
    Emitter() {}
    Emitter(vi Arr, int nw) : nw(nw) {
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
        cout << "\t restart Emitter\n";
        wIdx = 0;
        startIndex = (startIndex + 1) % 2; // even after odd, odd after even
    }
};

class Worker {
private:
    vk<int> &Arr;
public:
    Worker(vi &Arr) : Arr(Arr) {}

    Task compute(Task &task) {
        cout << "\nWorker compute\n";
        bool is_sorted = true;
        for(int i = task.begin; i <= task.end; i+=2) {
            if(Arr[i] > Arr[i+1]) {
                swap(Arr[i], Arr[i+1]);
                if(is_sorted) is_sorted = false;
            }
        }
        task.is_sorted = is_sorted;
        return task;
    }
};

class Collector {
private:
    int nw;
    bool is_sorted = true;
    int wCount = 0;
public:
    Collector(int nw) : nw(nw) {}

    Flag collect(Task &task) {
        if(!task.is_sorted)
            is_sorted = false;
        
        wCount++;
        // after checking all workers
        if(wCount == nw) {
            // if tasks for all workers are sorted, then finish
            if(is_sorted) {
                is_sorted = true;
                wCount = 0;
                return Flag::EOS;
            }
            // if tasks for all workers are not sorted, then restart
            is_sorted = true;
            wCount = 0;
            return Flag::RESTART;
        }
        // continue for next worker 
        return Flag::GO_ON;
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


    Emitter emitter(Arr, nw);
    Worker worker(Arr);
    Collector collector(nw);

    deque<Task> emitter_worker[nw];
    deque<Task> worker_collector[nw];
    deque<Flag> collector_emitter;

    Task EOS(-1, -1);

    utimer *timer = new utimer("EmitterCollector Code");
    thread emitter_thread([&](Emitter e) {
        while(true) {
            int thIdx = 0;
            this_thread::sleep_for(chrono::microseconds(1));
            // While Emitter can emit
            while(e.hasNext()) {
                // get new task
                Task task = e.next();
                // send this task to the worker
                emitter_worker[thIdx].push_front(task);
                // contine for the next thread
                thIdx = (thIdx + 1) % nw;
            }

            // Wait sth from Collector
            Flag signal = collector_emitter.back();
            collector_emitter.pop_back();

            if(signal == Flag::RESTART) {
                cout << "Flag restart\n";
                e.restart();
            } else if(signal == Flag::EOS) {
                cout << "Flag end\n";
                break;
            }
        }
        // send EOS to all Workers
        for(int i = 0; i < nw; i++)
            emitter_worker[i].push_front(EOS);
    }, emitter);

    vk<thread> worker_threads(nw);	// threads vector
    for (int th = 0; th < nw; th++) {
        worker_threads[th] = thread([&](Worker w, int wIdx) {
            while(true) {
                //
                cout << "worker(" << wIdx << ")\n";
                this_thread::sleep_for(chrono::microseconds(100));
                if(emitter_worker[wIdx].empty()) {
                    continue;
                }
                cout << "\t-passed(" << wIdx << ")\n";
                // get task form emitter 
                Task task = emitter_worker[wIdx].back();
                emitter_worker[wIdx].pop_back();
                if(task == EOS) {
                    worker_collector[wIdx].push_front(EOS);
                    break;
                }
                // else
                worker_collector[wIdx].push_front(w.compute(task));
            }
        }, worker, th);
    }

    thread collector_thread([&](Collector c) {
        int wIdx = 0;
        int wCount = 0;
        vi visited(nw, 0);

        while(true) {            
            this_thread::sleep_for(chrono::microseconds(100));
            if(visited[wIdx] || worker_collector[wIdx].empty()) {
                wIdx = (wIdx + 1) % nw;
                continue;
            }

            // get taks form worker
            visited[wIdx] = 1; 
            Task task = worker_collector[wIdx].back();
            worker_collector[wIdx].pop_back();
            cout << "collector task(" << wIdx << "): [" << task.begin << "," << task.end << "]\n";
            // if tasks finished for the worker
            if(task == EOS) {
                wCount++;
                if(wCount == nw) {
                    break;
                }
            } else {
                // send task to the collector
                Flag signal = c.collect(task);
                if(signal != Flag::GO_ON) {
                    collector_emitter.push_front(signal);
                }
            }
        }
    }, collector);

    emitter_thread.join();
    for(thread &th: worker_threads) {
        th.join();
    }
    collector_thread.join();
    timer->~utimer();


    // Print Sorted Vector
    // print_vec(Arr);

    return 0;
}