#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "utimer.cpp"

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace ff;
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
    return uid(rng, decltype(uid)::param_type{mn,mx});
}

/// generate random vector
vi rand_vec(int N, int nw) {
    vi randArr(N);

    ff::ParallelFor pr(nw);
    ffTime(START_TIME);
    pr.parallel_for(0, N, 1, 0, [&](const long idx) {
        randArr[idx] = rand_generator(MINN, MAXX);
    });
    ffTime(STOP_TIME);
    // printf("Rand Gen Time = %g\n", ffTime(GET_TIME));
    return randArr;
}

/// print vector
void print_vec(vi &vec) {
    cout << "\n";
    for (auto x: vec)
        cout << x << "\t";
    cout << "\n\n";
}

/// Odd-Even Sort
void OddEvenSort(vi &Arr, int nw) {
    //
    int N = Arr.size();
    vi startIndex = {0, 1}; // 0 - Even, 1 - Odd
    // Even Index starts from 0, Odd Index starts from 1.
    // Both will increase by 2 in each step.

    ff::ParallelFor pr(256);
    ffTime(START_TIME);
    while (!is_sorted(Arr.begin(), Arr.end())) {
        for(int ind: startIndex) {
            // start ind: 0 - even or 1 - odd
            pr.parallel_for(ind, N - 1, 2, [&](const long idx) {
                if(Arr[idx] > Arr[idx + 1])
                    swap(Arr[idx], Arr[idx + 1]);
            }, nw);
        }
    }
    ffTime(STOP_TIME);
    printf("Parallel Time = %g\n", ffTime(GET_TIME));
}

int main(int argc, char * argv[]) {
    // Read Input
    if (argc != ARG_COUNT) {
        cout << "Input Error. Number of argument is wrong..\n";
        exit(0);
    }

    int N = atoi(argv[1]);
    int nw = atoi(argv[2]);

    // input validation
    if (N < 0 || nw == 0) {
        cout << "Input Error. Invalid Input..\n";
        exit(0);
    }

    // genereate random vector
    vi Arr = rand_vec(N, nw);
    // print_vec(Arr);

    // Start & Run OddEvenSort
    OddEvenSort(Arr, nw);

    // Print Sorted Vector
    // print_vec(Arr);

    return 0;
}
