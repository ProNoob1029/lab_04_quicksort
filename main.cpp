#include <iostream>
#include "Profiler.h"
#include "common.h"
#include "quicksort.h"
#include "heapsort.h"

static void quicksortVsHeapsortNumOfOps() {
    std::cout << "profiling quicksort vs heapsort num of ops\n";

    constexpr int maxSize = 5000;
    constexpr int numOfTests = 5;
    int arr[maxSize] = {}, tempArr[maxSize + 1] = {};

    setupOperations(AVERAGE);

    for (int n = 100; n <= maxSize; n += 100) {
        std::cout << "n: " << n << '\n';
        for (int test = 0; test < numOfTests; test++) {
            FillRandomArray(arr, n);
            CopyArray(tempArr + 1, arr, n);
            quicksort(arr, n);
            heapsort(tempArr, n);
        }
    }

    profiler.divideValues(quicksortAssign, numOfTests);
    profiler.divideValues(quicksortCompare, numOfTests);
    profiler.divideValues(heapsortAssignAverage, numOfTests);
    profiler.divideValues(heapsortCompareAverage, numOfTests);

    profiler.addSeries(quicksortOps, quicksortAssign, quicksortCompare);
    profiler.addSeries(heapsortOpsAverage, heapsortAssignAverage, heapsortCompareAverage);

    profiler.createGroup(algorithmsComparison, quicksortOps, heapsortOpsAverage);
    profiler.createGroup(algorithmsAssignComparison, quicksortAssign, heapsortAssignAverage);
    profiler.createGroup(algorithmsCompareComparison, quicksortCompare, heapsortCompareAverage);
}

int main() {
    profiler.reset("Quicksort_Cases_Comparison");
    quicksortCompareCases();
    profiler.reset("Quicksort_vs_Heapsort_Comparison");
    quicksortVsHeapsortNumOfOps();
    profiler.reset();
    checkQuicksortCorrectness();
    return 0;
}
