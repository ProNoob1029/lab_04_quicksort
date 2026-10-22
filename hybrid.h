#pragma once
#include "common.h"

constexpr char hybridAssign[] = "hybrid_quicksort_assign";
constexpr char hybridCompare[] = "hybrid_quicksort_compare";
constexpr char hybridTime[] = "hybrid_quicksort_time";

inline int THRESHOLD = 5;

static void insertionSort(int arr[], const int left, const int right) {
    for (int i = left + 1; i <= right; i++) {
        const int key = arr[i];
        profiler.countOperation(hybridAssign, N_SIZE);
        int j = i - 1;
        while (j >= left && greater(arr[j], key, hybridCompare)) {
            arr[j + 1] = arr[j];
            profiler.countOperation(hybridAssign, N_SIZE);
            j--;
        }
        arr[j + 1] = key;
        profiler.countOperation(hybridAssign, N_SIZE);
    }
}

static int partitionHybrid(int arr[], const int left, const int right) {
    const int pivot = arr[right];
    profiler.countOperation(hybridAssign, N_SIZE);
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            i++;
            //No swap needed if i and j are at same index
            if (i != j) {
                std::swap(arr[j], arr[i]);
                profiler.countOperation(hybridAssign, N_SIZE, 3);
            }
        }
        profiler.countOperation(hybridCompare, N_SIZE);
    }
    std::swap(arr[i + 1], arr[right]);
    profiler.countOperation(hybridAssign, N_SIZE, 3);
    return i + 1;
}

static void quicksortHybrid(int arr[], const int left, const int right) {
    if (right - left + 1 < THRESHOLD) {
        insertionSort(arr, left, right);
        return;
    }
    const int pos = partitionHybrid(arr, left, right);
    quicksortHybrid(arr, left, pos - 1);
    quicksortHybrid(arr, pos + 1, right);
}

static void quicksortHybrid(int arr[], const int n) {
    N_SIZE = n;
    quicksortHybrid(arr, 0, n - 1);
}

static void quicksortHybridThresholdTesting(int arr[], const int n, const int threshold) {
    N_SIZE = threshold;
    THRESHOLD = threshold;
    quicksortHybrid(arr, 0, n - 1);
}

static void profileHybridQuicksort(const int threshold, const int numOfOpsTests) {
    constexpr int n = 2000;
    constexpr int timingTests = 2000;
    constexpr int prefilledArrays = 10;
    int arr[n], tempArr[prefilledArrays][n];

    for (int i = 0; i < numOfOpsTests; i++) {
        FillRandomArray(arr, n);
        quicksortHybridThresholdTesting(arr, n, threshold);
    }

    for (int i = prefilledArrays; i <= timingTests; i += prefilledArrays) {
        for (auto & a : tempArr) {
            FillRandomArray(a, n);
        }
        profiler.startTimer(hybridTime, threshold);
        for (auto & a : tempArr) {
            quicksortHybridThresholdTesting(a, n, threshold);
        }
        profiler.stopTimer(hybridTime, threshold);
    }
}

static void testThresholds() {
    constexpr int numOfOpsTests = 100;
    for (int threshold = 5; threshold <= 15; threshold++) {
        std::cout << "\nThreshold: " << threshold;
        profileHybridQuicksort(threshold, numOfOpsTests);
    }
    profiler.divideValues(hybridAssign, numOfOpsTests);
    profiler.divideValues(hybridCompare, numOfOpsTests);
}

static void findThresholds() {
    constexpr int prefilledArrays = 10;
    constexpr int n = 1000;
    constexpr int timingTests = 1000;
    constexpr int numOfOpsTests = 100;
    int arr[prefilledArrays][n], tempArr[prefilledArrays][n];

    //pre init to remove randoms in-between thresholds
    for (auto & a : arr) {
        FillRandomArray(a, n);
    }

    for (int threshold = 5; threshold <= 50; threshold++) {
        std::cout << "\nThreshold: " << threshold;

        for (int i = prefilledArrays; i <= numOfOpsTests; i++) {
            CopyArray(tempArr, arr, prefilledArrays);
            for (auto & a : tempArr) {
                quicksortHybridThresholdTesting(a, n, threshold);
            }
        }

        for (int i = prefilledArrays; i <= timingTests; i += prefilledArrays) {
            CopyArray(tempArr, arr, prefilledArrays);
            profiler.startTimer(hybridTime, threshold);
            for (auto & a : tempArr) {
                quicksortHybridThresholdTesting(a, n, threshold);
            }
            profiler.stopTimer(hybridTime, threshold);
        }
    }
    profiler.divideValues(hybridAssign, numOfOpsTests);
    profiler.divideValues(hybridCompare, numOfOpsTests);
}

static void checkHybridQuicksortCorrectness() {
    std::cout << "\n==========================================\n";
    std::cout << "\nChecking hybrid quicksort correctness...\n";

    constexpr int n = 8;
    int arr[] = {4, 8, 1, 7, 8, 3, 2, 1};

    std::cout << "\nInitial array: ";
    printArray(arr, n);

    quicksortHybrid(arr, n);

    std::cout << "\nSorted array: ";
    printArray(arr, n);

    if (IsSorted(arr, n)) {
        std::cout << "\nIt's sorted!\n";
    } else {
        std::cout << "\nNot sorted!\n";
    }
}