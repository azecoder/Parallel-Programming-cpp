#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>

#include "utimer.cpp"


using std::chrono::high_resolution_clock;
using std::vector;
using std::thread;
using std::mutex;
using std::swap;
using std::cout;

#define ARG_COUNT 4


/// generate random vector
vector<int> rand_vec(int nloop, int limit) {
    utimer *timer_rand = new utimer("Generate Rand Vec");

    // number of threads
    const size_t nthreads = thread::hardware_concurrency();

    vector<thread> threads(nthreads);	// threads vector
    vector<int> threadArr[nthreads];	// to save generated rand values for ecah thread
    for(int th = 0; th < nthreads; th++) {
        // configure rand
        int seed = timer_rand->elapsed();
        srand(seed);

        // thread (function, parameters)
        threads[th] = thread(
            [&](const int si, const int ei, const int th) {
                // loop over all items
                for(int i = si; i < ei; i++)
                    threadArr[th].push_back((i*rand())%limit);
            },
            th * nloop / nthreads,
            (th+1) == nthreads ? nloop : (th+1) * nloop / nthreads,
            th
        );
    }

    std::for_each(threads.begin(), threads.end(), [](thread &th) {
        th.join();
    });

    vector<int> retArr;
    for(int i = 0; i < nthreads; i++)
        for(int x: threadArr[i])
            retArr.push_back(x);

    // print time to generate rand vector
    timer_rand -> ~utimer();

    return retArr;
}

/// print vector
void print_vec(vector<int> &vec) {
    std::cout << "\n";
    for (auto x: vec)
        std::cout << x << "\t";
    std::cout << "\n\n";
}

/// Odd-Even Sort
void OddEvenSort(vector<int> inputArr) {
    //
    int nloop = inputArr.size();
    bool is_sorted = false;
    int startIndex[2] = {0, 1}; // 0 - Even, 1 - Odd
    // Even Index starts from 0, Odd Index starts from 1.
    // Both will increase by 2 in each step.

    while (!is_sorted) {
        is_sorted = true;

        for (int ind: startIndex) {
            // number of threads
            const size_t nthreads = thread::hardware_concurrency();

            vector<thread> threads(nthreads);	// threads vector

            for(int th = 0; th < nthreads; th++) {
                int len = nloop / nthreads;
                int startInd = th * len - (th * len) % 2 + ind;
                int endInd = (th+1) == nthreads ? nloop : std::min(startInd + len, nloop);

                threads[th] = thread(
                    [&](const int si, const int ei, const int th) {
                        // loop over all items
                        for(int i = si; i < ei; i+=2)
                            if(inputArr[i] > inputArr[i+1]) {
                                swap(inputArr[i], inputArr[i + 1]);
                                is_sorted = false;
                            }
                    },
                    startInd,
                    endInd,
                    th
                );
            }

            std::for_each(threads.begin(), threads.end(), [](thread &th) {
                th.join();
            });
        }
    }

    // print sorted vector
    print_vec(inputArr);
}

int main(int argc, char * argv[]) {
    // read inputs
    if (argc != ARG_COUNT) {
        cout << "Input Error. Number of argument is wrong..\n";
        exit(0);
    }

    int N = atoi(argv[1]);
    int limit = atoi(argv[2]);
    int seed = atoi(argv[3]);

    // input validation
    if (N < 0 || limit < 0 || seed < 0) {
        cout << "Input Error. Invalid Input..\n";
        exit(0);
    }

    if (seed == 0)
        srand(time(NULL));
    else
        srand(seed);

    // genereate random vector
    vector<int> Arr = rand_vec(N, limit);

    // call OddEvenSort function
    utimer *timer = new utimer("Parallel Code");
    OddEvenSort(Arr);
    timer -> ~utimer();

    return 0;
}
