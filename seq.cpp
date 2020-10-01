#include <iostream>
#include <vector>
#include <chrono>

#include "lib/tools.cpp"
#include "lib/utimer.cpp"


#define ARG_COUNT 4


void OddEvenSort(std::vector < int > inputArr) {
    // 
    int len = inputArr.size();
    bool is_sorted = false;
    int startIndex[2] = {0, 1}; // 0 - Even, 1 - Odd
    // Even Index starts from 0, Odd Index starts from 1.
    // Both will increase by 2 in each step.

    while (!is_sorted) {
        is_sorted = true;

        for (int ind: startIndex) {
            // start ind: 0 - even or 1 - odd
            while (ind < len - 1) {
                if (inputArr[ind] > inputArr[ind + 1]) {
                    std::swap(inputArr[ind], inputArr[ind + 1]);
                    is_sorted = false;
                }
                ind += 2;
            }
        }
    }
    // Print Sorted Vector
    tools::print(inputArr);
}

int main(int argc, char * argv[]) {
    // Read Input
    if (argc != ARG_COUNT) {
        std::cout << "Input Error. Number of argument is wrong..\n";
        exit(0);
    }

    int N = atoi(argv[1]);
    int limit = atoi(argv[2]);
    int seed = atoi(argv[3]);

    // input validation
    if (N < 0 || limit < 0 || seed < 0) {
        std::cout << "Input Error. Invalid Input..\n";
        exit(0);
    }

    if (seed == 0)
        srand(time(NULL));
    else
        srand(seed);

    std::vector < int > Arr = tools::rand_vec(N, limit);

    // Start & Run OddEvenSort
    utimer *timer = new utimer("Seq Code");
    auto start = std::chrono::high_resolution_clock::now();
    OddEvenSort(Arr);
    timer -> ~utimer();

    return 0;
}