#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include <omp.h>
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
    return uid(rng, decltype(uid)::param_type{mn,mx});
}

/// generate random vector
vi rand_vec(int N, int nw) {
    vi randArr(N);
    // utimer *timer_rand = new utimer("Generate Rand Vec");
    #pragma omp parallel for num_threads(nw)
    for(int &x: randArr)
        x = rand_generator(MINN, MAXX);
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

/// Odd-Even Sort
void OddEvenSort(vi &Arr, int nw) {
    //
    int len = Arr.size();
    vi startIndex = {0, 1}; // 0 - Even, 1 - Odd
    // Even Index starts from 0, Odd Index starts from 1.
    // Both will increase by 2 in each step.

    utimer *timer = new utimer("Parallel Code");
    while (!is_sorted(Arr.begin(), Arr.end())) {
        for (int ind: startIndex)
            // start ind: 0 - even or 1 - odd
            #pragma omp parallel for num_threads(nw)
            for(int i = ind; i < len - 1; i+=2) {
                if (Arr[i] > Arr[i + 1])
                    swap(Arr[i], Arr[i + 1]);
            }
    }
    timer->~utimer();
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

    // Run OddEvenSort
    OddEvenSort(Arr, nw);

    // Print Sorted Vector
    // print_vec(Arr);

    return 0;
}
