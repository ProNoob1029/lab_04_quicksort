#pragma once
#include "common.h"

constexpr char hybridAssign[] = "hybrid_quicksort_assign";
constexpr char hybridCompare[] = "hybrid_quicksort_compare";
constexpr char hybridQuicksort[] = "hybrid_quicksort";
constexpr char hybridTime[] = "hybrid_quicksort_time";
constexpr char DEFAULT_THRESHOLD = 50;

inline int THRESHOLD = DEFAULT_THRESHOLD;

template<bool COUNT>
static void insertionSort(int arr[], const int left, const int right) {
    for (int i = left + 1; i <= right; i++) {
        const int key = arr[i];
        if constexpr (COUNT) profiler.countOperation(hybridAssign, N_SIZE);
        int j = i - 1;
        while (j >= left && greater<COUNT>(arr[j], key, hybridCompare)) {
            arr[j + 1] = arr[j];
            if constexpr (COUNT) profiler.countOperation(hybridAssign, N_SIZE);
            j--;
        }
        arr[j + 1] = key;
        if constexpr (COUNT) profiler.countOperation(hybridAssign, N_SIZE);
    }
}

template<bool COUNT>
static int partitionHybrid(int arr[], const int left, const int right) {
    const int pivot = arr[right];
    if constexpr (COUNT) profiler.countOperation(hybridAssign, N_SIZE);
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            i++;
            //No swap needed if i and j are at same index
            if (i != j) {
                std::swap(arr[j], arr[i]);
                if constexpr (COUNT) profiler.countOperation(hybridAssign, N_SIZE, 3);
            }
        }
        if constexpr (COUNT) profiler.countOperation(hybridCompare, N_SIZE);
    }
    std::swap(arr[i + 1], arr[right]);
    if constexpr (COUNT) profiler.countOperation(hybridAssign, N_SIZE, 3);
    return i + 1;
}

template<bool COUNT>
static void quicksortHybrid(int arr[], const int left, const int right) {
    if (right - left + 1 < THRESHOLD) {
        insertionSort<COUNT>(arr, left, right);
        return;
    }
    const int pos = partitionHybrid<COUNT>(arr, left, right);
    quicksortHybrid<COUNT>(arr, left, pos - 1);
    quicksortHybrid<COUNT>(arr, pos + 1, right);
}

template<bool COUNT>
static void quicksortHybrid(int arr[], const int n) {
    N_SIZE = n;
    THRESHOLD = DEFAULT_THRESHOLD;
    quicksortHybrid<COUNT>(arr, 0, n - 1);
}

template<bool COUNT>
static void quicksortHybridThresholdTesting(int arr[], const int n, const int threshold) {
    N_SIZE = threshold;
    THRESHOLD = threshold;
    quicksortHybrid<COUNT>(arr, 0, n - 1);
}

static void findThresholdsNumOfOps() {
    constexpr int n = 10000;
    constexpr int numOfOpsTests = 10;
    //without static it's too big for the stack
    static int arr[numOfOpsTests][n], tempArr[numOfOpsTests][n];

    //pre init to remove randoms in-between thresholds
    for (auto &a: arr) {
        FillRandomArray(a, n);
    }

    for (int threshold = 5; threshold <= 125; threshold++) {
        std::cout << "\nThreshold: " << threshold;
        CopyArray(tempArr, arr, numOfOpsTests);
        for (auto &i: tempArr) {
            quicksortHybridThresholdTesting<true>(i, n, threshold);
        }
    }
    profiler.divideValues(hybridAssign, numOfOpsTests);
    profiler.divideValues(hybridCompare, numOfOpsTests);
    profiler.addSeries(hybridQuicksort, hybridAssign, hybridCompare);
}

static int median(int arr[], const int n) {
    insertionSort<false>(arr, 0, n - 1);
    return (n % 2) ? arr[n / 2] : (arr[n / 2 - 1] + arr[n / 2]) / 2;
}

static void findThresholdsTiming() {
    constexpr int thresholdMin = 5;
    constexpr int thresholdMax = 125;
    constexpr int thresholdCount = thresholdMax - thresholdMin + 1;
    constexpr int n = 1000;
    constexpr int workBatch = 15;
    constexpr int rounds = 50;
    //without static it's too big for the stack
    static int work[workBatch][n], src[workBatch][n];
    static int mins[thresholdCount] = {};
    static int thresholds[thresholdCount];
    static int samples[thresholdCount][rounds];

    //pre init to remove randoms in-between thresholds
    for (auto &arr: src) {
        FillRandomArray(arr, n);
    }

    //multiple rounds to remove random cpu spikes
    for (int round = 1; round <= rounds; round++) {
        std::cout << "\nRound: " << round << " =====================";

        //randomize tests order to remove bias from cpu throttling
        FillRandomArray(thresholds, thresholdCount, thresholdMin, thresholdMax, true, UNSORTED);

        for (const auto &threshold: thresholds) {
            CopyArray(work, src, workBatch);

            profiler.startTimer(hybridTime, threshold);
            for (auto &arr: work) {
                quicksortHybridThresholdTesting<false>(arr, n, threshold);
            }
            /**
             *  Modified profiler.stopTimer() to make it return the duration of last run.
             *  This was necessary to be able to calculate min and median.
             *  The function was also modified to count microseconds instead of milliseconds
             *  because when casting duration to int, milliseconds lose a lot of precision.
             *  Results could be reproduced with vanilla profiler, but you would need
             *  a lot of runs to account for the loss and precision, and you would
             *  need to leave out min and median graphs
             */
            const int time = profiler.stopTimer(hybridTime, threshold);

            const int i = threshold - thresholdMin;
            samples[i][round - 1] = time;
            if (mins[i] > time || mins[i] == 0) {
                mins[i] = time;
            }
        }
    }

    for (int i = 0; i < thresholdCount; i++) {
        profiler.countOperation("min", i + thresholdMin, mins[i]);
        profiler.countOperation("median", i + thresholdMin, median(samples[i], rounds));
    }
    profiler.createGroup("min vs median", "min", "median");
}

static void checkHybridQuicksortCorrectness() {
    std::cout << "\n==========================================\n";
    std::cout << "\nChecking hybrid quicksort correctness...\n";

    constexpr int n = 8;
    int arr[] = {4, 8, 1, 7, 8, 3, 2, 1};

    std::cout << "\nInitial array: ";
    printArray(arr, n);

    quicksortHybrid<false>(arr, n);

    std::cout << "\nSorted array: ";
    printArray(arr, n);

    if (IsSorted(arr, n)) {
        std::cout << "\nIt's sorted!\n";
    } else {
        std::cout << "\nNot sorted!\n";
    }
}
