#pragma once
#include "common.h"

constexpr char quicksortCasesComparison[] = "1_quicksort_cases_comparison";
constexpr char quicksortAverageVsBest[] = "2_quicksort_average_vs_best";

constexpr char quicksortAssignCasesComparison[] = "quicksort_assign_cases_comparison";
constexpr char quicksortCompareCasesComparison[] = "quicksort_compare_cases_comparison";
constexpr char quicksortCompareAverageVsBest[] = "quicksort_compare_average_vs_best";

constexpr char quicksortOpsAverage[] = "quicksort_ops_average";
constexpr char quicksortOpsBest[] = "quicksort_ops_best";
constexpr char quicksortOpsWorst[] = "quicksort_ops_worst";

constexpr char quicksortAssignAverage[] = "quicksort_assign_average";
constexpr char quicksortCompareAverage[] = "quicksort_compare_average";
constexpr char quicksortAssignBest[] = "quicksort_assign_best";
constexpr char quicksortCompareBest[] = "quicksort_compare_best";
constexpr char quicksortAssignWorst[] = "quicksort_assign_worst";
constexpr char quicksortCompareWorst[] = "quicksort_compare_worst";
constexpr char quicksortTime[] = "quicksort_time";

enum Case { AVERAGE = 0, BEST = 1, WORST = 2 };

static const char *quicksortAssign = quicksortAssignAverage;
static const char *quicksortCompare = quicksortCompareAverage;
static const char *quicksortOps = quicksortOpsAverage;

template<bool COUNT, bool RANDO>
static int partition(int arr[], const int left, const int right) {
    if constexpr (RANDO) {
        if (const int pivotIdx = left + rand() % (right - left + 1); pivotIdx != right) {
            std::swap(arr[pivotIdx], arr[right]);
            if constexpr (COUNT) profiler.countOperation(quicksortAssign, N_SIZE, 3);
        }
    }

    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (arr[j] <= arr[right]) {
            i++;
            //No swap needed if i and j are at same index
            if (i != j) {
                std::swap(arr[j], arr[i]);
                if constexpr (COUNT) profiler.countOperation(quicksortAssign, N_SIZE, 3);
            }
        }
        if constexpr (COUNT) profiler.countOperation(quicksortCompare, N_SIZE);
    }
    std::swap(arr[i + 1], arr[right]);
    if constexpr (COUNT) profiler.countOperation(quicksortAssign, N_SIZE, 3);
    return i + 1;
}

template<bool COUNT, bool RANDO>
static void quicksort(int arr[], const int left, const int right) {
    if (left >= right) {
        return;
    }
    const int pos = partition<COUNT, RANDO>(arr, left, right);
    quicksort<COUNT, RANDO>(arr, left, pos - 1);
    quicksort<COUNT, RANDO>(arr, pos + 1, right);
}

template<bool COUNT>
static void quicksort(int arr[], const int n) {
    N_SIZE = n;
    quicksort<COUNT, false>(arr, 0, n - 1);
}

static void quicksortRando(int arr[], const int n) {
    N_SIZE = n;
    quicksort<false, true>(arr, 0, n - 1);
}

static void arrangeBestCase(int arr[], const int left, const int right) {
    if (left >= right) {
        return;
    }
    const int mid = left + (right - left) / 2;

    arrangeBestCase(arr, left, mid - 1);
    arrangeBestCase(arr, mid + 1, right);

    // move median to end
    std::swap(arr[mid], arr[right]);
}

//Needs a sorted array with unique values
static void arrangeBestCase(int arr[], const int n) {
    arrangeBestCase(arr, 0, n - 1);
}

static void generateQuicksortArray(int arr[], const int n, const Case testcase) {
    switch (testcase) {
        case AVERAGE:
            FillRandomArray(arr, n, 10, 50000, false, UNSORTED);
            break;
        case BEST:
            FillRandomArray(arr, n, 10, 50000, true, ASCENDING);
            arrangeBestCase(arr, n);
            break;
        case WORST:
            FillRandomArray(arr, n, 10, 50000, false, ASCENDING);
            break;
    }
}

static void setupOperations(const Case testCase) {
    switch (testCase) {
        case AVERAGE:
            quicksortAssign = quicksortAssignAverage;
            quicksortCompare = quicksortCompareAverage;
            quicksortOps = quicksortOpsAverage;
            break;
        case BEST:
            quicksortAssign = quicksortAssignBest;
            quicksortCompare = quicksortCompareBest;
            quicksortOps = quicksortOpsBest;
            break;
        case WORST:
            quicksortAssign = quicksortAssignWorst;
            quicksortCompare = quicksortCompareWorst;
            quicksortOps = quicksortOpsWorst;
            break;
    }
}

static void quicksortProfilingNumOfOps(const Case testCase) {
    std::cout << "profiling quicksort num of ops\n";

    constexpr int maxSize = 10000;
    const int numOfTests = testCase == AVERAGE ? 10 : 1;
    int arr[maxSize];

    setupOperations(testCase);

    for (int n = 100; n <= maxSize; n += 100) {
        std::cout << "n: " << n << '\n';
        for (int test = 0; test < numOfTests; test++) {
            generateQuicksortArray(arr, n, testCase);
            quicksort<true>(arr, n);
        }
    }

    profiler.divideValues(quicksortAssign, numOfTests);
    profiler.divideValues(quicksortCompare, numOfTests);

    profiler.addSeries(quicksortOps, quicksortAssign, quicksortCompare);
}

static void quicksortCompareCases() {
    quicksortProfilingNumOfOps(AVERAGE);
    quicksortProfilingNumOfOps(BEST);
    quicksortProfilingNumOfOps(WORST);
    profiler.createGroup(quicksortCasesComparison, quicksortOpsAverage, quicksortOpsBest, quicksortOpsWorst);
    profiler.createGroup(quicksortAverageVsBest, quicksortOpsAverage, quicksortOpsBest);
    profiler.createGroup(quicksortAssignCasesComparison, quicksortAssignAverage, quicksortAssignBest,
                         quicksortAssignWorst);
    profiler.createGroup(quicksortCompareCasesComparison, quicksortCompareAverage, quicksortCompareBest,
                         quicksortCompareWorst);
    profiler.createGroup(quicksortCompareAverageVsBest, quicksortCompareAverage, quicksortCompareBest);
}

static void checkQuicksortCorrectness() {
    std::cout << "==========================================\n";
    std::cout << "Checking quicksort correctness...\n";

    constexpr int n = 8;
    int arr[] = {4, 8, 1, 7, 8, 3, 2, 1};

    std::cout << "Initial array: ";
    printArray(arr, n);

    quicksort<false>(arr, n);

    std::cout << "Sorted array: ";
    printArray(arr, n);

    if (IsSorted(arr, n)) {
        std::cout << "It's sorted!\n";
    } else {
        std::cout << "Not sorted!\n";
    }
}

static void checkQuicksortRandoCorrectness() {
    std::cout << "==========================================\n";
    std::cout << "Checking quicksort with random pivot correctness...\n";

    constexpr int n = 8;
    int arr[] = {4, 8, 1, 7, 8, 3, 2, 1};

    std::cout << "Initial array: ";
    printArray(arr, n);

    //seeding random
    srand(time(nullptr));
    quicksortRando(arr, n);

    std::cout << "Sorted array: ";
    printArray(arr, n);

    if (IsSorted(arr, n)) {
        std::cout << "It's sorted!\n";
    } else {
        std::cout << "Not sorted!\n";
    }
}
