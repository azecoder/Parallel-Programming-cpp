#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <random>

#include "utimer.cpp"


using std::chrono::high_resolution_clock;
using std::vector;
using std::thread;
using std::mutex;
using std::swap;
using std::cout;
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

#define vk vector
#define pb push_back

typedef vk<int> vi;

const int ARG_COUNT = 3;
const int MINN = -1e5;
const int MAXX = 1e5+10;



// mt19937 random number generator
int rand_generator(int mn, int mx) {
    thread_local random_device rd;
    thread_local mt19937 rng(rd());  
    thread_local uniform_int_distribution<int> uid;
    return uid(rng, decltype(uid)::param_type{mn,mx});
}

// generate random vector
vi rand_vec(int N, int nw) {
    // utimer
    utimer *timer_rand = new utimer("Generate Rand Vec");

    vk <thread> threads(nw);	// threads vector
    vi randArr(N);	// to save generated rand values for ecah thread
    for(int th = 0; th < nw; th++) {

        // thread (function, parameters)
        threads[th] = thread(
            [&](const int si, const int ei) {
                // loop over all items
                for(int i = si; i < ei; i++)
                    randArr[i] = rand_generator(MINN, MAXX);
            },
            th * N / nw,
            (th+1) == nw ? N : (th+1) * N / nw
        );
    }

    std::for_each(threads.begin(), threads.end(), [](thread &th) {
        th.join();
    });

    // print time to generate rand vector
    timer_rand -> ~utimer();

    return randArr;
}

/// print vector
void print_vec(vi &vec) {
    std::cout << "\n";
    for (auto x: vec)
        std::cout << x << "\t";
    std::cout << "\n\n";
}

/// Odd-Even Sort
void OddEvenSort(vi &arr, int nw) {
    //
    int N = arr.size();
    bool is_sorted = false;
    int startIndex[2] = {0, 1}; // 0 - Even, 1 - Odd
    // Even Index starts from 0, Odd Index starts from 1.
    // Both will increase by 2 in each step.

    while (!is_sorted) {
        is_sorted = true;

        for (int ind: startIndex) {
            // number of threads
            //const size_t nw = thread::hardware_concurrency();
            // utimer *timer_oe = new utimer("odd - even");

            vk <thread> threads(nw);	// threads vector
            for(int th = 0; th < nw; th++) {
                int len = N / nw;
                int sInd = th * len - (th * len) % 2 + ind;
                int eInd = (th+1) == nw ? N : std::min(sInd + len, N);

                threads[th] = thread(
                    [&](const int si, const int ei) {
                        // loop over all items
                        for(int i = si; i < ei-1; i+=2)
                            if(arr[i] > arr[i+1]) {
                                swap(arr[i], arr[i + 1]);
                                is_sorted = false;
                            }
                    },
                    sInd,
                    eInd
                );
            }

            std::for_each(threads.begin(), threads.end(), [](thread &th) {
                th.join();
            });

            // cout << "is sorted: " << is_sorted << "\n";
            // timer_oe -> ~utimer();
        }
    }

}

int main(int argc, char * argv[]) {
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

    // call OddEvenSort function
    utimer *timer = new utimer("Parallel Code");
    OddEvenSort(Arr, nw);
    timer -> ~utimer();

    // print sorted vector
    // print_vec(Arr);

    return 0;
}
