#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "utimer.cpp"

using namespace std;

#define vk vector
#define pb push_back

typedef vk<int> vi;

const int ARG_COUNT = 2;
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

/// Odd-Even Sort
void OddEvenSort(vi &Arr) {
    //
    int N = Arr.size();
    vi startIndex = {0, 1};	// 0 - Even, 1 - Odd
    // Even Index starts from 0, Odd Index starts from 1.
    // Both will increase by 2 in each step.

    bool is_sorted = false;
    utimer *timer = new utimer("Sequential Code");
    while (!is_sorted) {
        is_sorted = true;
        for (int ind: startIndex)
            // start ind: 0 - even or 1 - odd
            for (int i = ind; i < N - 1; i += 2)
                if (Arr[i] > Arr[i + 1]) {
                    swap(Arr[i], Arr[i + 1]);
                    if(is_sorted) is_sorted = false;
                }
    }
    timer->~utimer();
}

int main(int argc, char *argv[]) {
    // Read Input
    if (argc != ARG_COUNT) {
        cout << "Input Error. Number of argument is wrong..\n";
        exit(0);
    }

    int N = atoi(argv[1]);

    // input validation
    if (N < 0) {
        cout << "Input Error. Invalid Input..\n";
        exit(0);
    }

    vi Arr = rand_vec(N);

    // Run OddEvenSort
    OddEvenSort(Arr);

    // Print Sorted Vector
    // print_vec(Arr);

    return 0;
}