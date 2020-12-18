#include <iostream>
#include <vector>
#include <chrono>
#include <random>

#include <omp.h>
#include "utimer.cpp"

using std::chrono::high_resolution_clock;
using std::vector;
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

/// generate random vector
vi rand_vec(int N, int nw) {
    utimer *timer_rand = new utimer("Rand Numbers");
    vi randArr(N);
    #pragma omp parallel for num_threads(nw)
    for(int &x: randArr)
        x = rand_generator(MINN, MAXX);
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
    int len = arr.size();
    bool is_sorted = false;
    int startIndex[2] = {0, 1}; // 0 - Even, 1 - Odd
    // Even Index starts from 0, Odd Index starts from 1.
    // Both will increase by 2 in each step.

    while (!is_sorted) {
        is_sorted = true;

        for (int ind: startIndex) {
        // start ind: 0 - even or 1 - odd
            #pragma omp parallel for num_threads(nw) 
            for(int i = ind; i < len - 1; i+=2) {
                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                    is_sorted = false;
                }
            }
        //
        }
    }
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

    vi Arr = rand_vec(N, nw);
    // print_vec(Arr);

    // Start & Run OddEvenSort
    utimer *timer = new utimer("Parallel Code");
    OddEvenSort(Arr, nw);
    timer -> ~utimer();

    // print Arr
    // print_vec(Arr);

    return 0;
}
