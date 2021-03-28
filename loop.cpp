#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include <thread>
#include "utimer.cpp"

using namespace std;

#define vk vector
#define pb push_back

typedef vk<int> vi;

const int ARG_COUNT = 3;
const int MINN = -1e5;
const int MAXX = 1e5 + 10;


/// mt19937 random number generator
int rand_generator(int mn, int mx) {
    thread_local random_device rd;
    thread_local mt19937 rng(rd());
    thread_local uniform_int_distribution<int> uid;
    return uid(rng, decltype(uid)::param_type {mn,mx});
}

/// generate random vector
vi rand_vec(int N, int nw) {
    vk<thread> threads(nw);	// threads vector
    vi randArr(N);	// to save generated rand values for each thread
    int chunk = N / nw;

    // utimer *timer_rand = new utimer("Generate Rand Vec");
    for (int th = 0; th < nw; th++) {
        // thread (function, parameters)
        threads[th] = thread([&](const int si, const int ei) {
            // loop over all items
            for (int i = si; i < ei; i++)
                randArr[i] = rand_generator(MINN, MAXX);
        },
        th * chunk,
        th == (nw - 1) ? N : (th + 1) * chunk);
    }

    for_each(threads.begin(), threads.end(), [](thread & th) {
        th.join();
    });
    // timer_rand->~utimer();

    return randArr;
}

/// print vector
void print_vec(vi &vec) {
    cout << "\n";
    for (auto x: vec)
        cout << x << "\t";
    cout << "\n\n";
}

void Loop(vi &Arr, int nw) {
    //
    int N = Arr.size();

    vk<thread> threads(nw);	// threads vector
    int chunk = N / nw;

    utimer *timer = new utimer("Parallel Code");
    while(!is_sorted(Arr.begin(), Arr.end())) {
        for (int th = 0; th < nw; th++) {
            int sInd = th * chunk;
            int eInd = (th + 1) == nw ? N : (th + 1) * chunk;
            // cout << "th: " << th << " [" << sInd << ", " << eInd << "]\n";
            threads[th] = thread([&](const int si, const int ei) {
                // loop over all items
                for (int i = si; i < ei; i += 2) {
                    if (Arr[i] > Arr[i + 1])
                        swap(Arr[i], Arr[i + 1]);
                }
            }, sInd, eInd);
        }
        for_each(threads.begin(), threads.end(), [](thread & th) {
            th.join();
        });
        for (int th = 0; th < nw; th++) {
            int sInd = th * chunk;
            sInd += (sInd%2) ? 0 : 1;
            int eInd = (th + 1) == nw ? N : (th + 1) * chunk;
            // cout << "th: " << th << " [" << sInd << ", " << eInd << "]\n";
            threads[th] = thread([&](const int si, const int ei) {
                // loop over all items
                for (int i = si; i < ei; i += 2) {
                    if (Arr[i] > Arr[i + 1])
                        swap(Arr[i], Arr[i + 1]);
                }
            }, sInd, eInd);
        }
        for_each(threads.begin(), threads.end(), [](thread & th) {
            th.join();
        });
    }
    timer->~utimer();
}

int main(int argc, char *argv[]) {
    // read inputs
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

    // genereate random vector
    vi Arr = rand_vec(N, nw);
    // print_vec(Arr);

    // Run Loop
    Loop(Arr, nw);

    // Print Sorted Vector
    // print_vec(Arr);

    return 0;
}