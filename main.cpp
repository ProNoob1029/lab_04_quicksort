#include <iostream>
#include "quicksort.h"
#include "heapsort.h"
#include "hybrid.h"

static void quicksortVsHeapsortNumOfOps() {
    std::cout << "profiling quicksort vs heapsort num of ops\n";

    constexpr int maxSize = 10000;
    constexpr int numOfTests = 5;
    int arr[maxSize] = {}, tempArr[maxSize + 1] = {};

    setupOperations(AVERAGE);

    for (int n = 100; n <= maxSize; n += 100) {
        std::cout << "n: " << n << '\n';
        for (int test = 0; test < numOfTests; test++) {
            FillRandomArray(arr, n);
            CopyArray(tempArr + 1, arr, n);
            quicksort<true>(arr, n);
            heapsort<true>(tempArr, n);
        }
    }

    profiler.divideValues(quicksortAssign, numOfTests);
    profiler.divideValues(quicksortCompare, numOfTests);
    profiler.divideValues(heapsortAssignAverage, numOfTests);
    profiler.divideValues(heapsortCompareAverage, numOfTests);

    profiler.addSeries(quicksortOps, quicksortAssign, quicksortCompare);
    profiler.addSeries(heapsortOpsAverage, heapsortAssignAverage, heapsortCompareAverage);

    profiler.createGroup(quickVsHeap, quicksortOps, heapsortOpsAverage);
    profiler.createGroup(quickVsHeapAssign, quicksortAssign, heapsortAssignAverage);
    profiler.createGroup(quickVsHeapCompare, quicksortCompare, heapsortCompareAverage);
}

static void quicksortVsHybridNumOfOps() {
    std::cout << "profiling quicksort vs hybrid quicksort num of ops\n";

    constexpr int maxSize = 10000;
    constexpr int numOfTests = 5;
    int arr[maxSize], tempArr[maxSize];

    setupOperations(AVERAGE);

    for (int n = 100; n <= maxSize; n += 100) {
        std::cout << "n: " << n << '\n';
        for (int test = 0; test < numOfTests; test++) {
            FillRandomArray(arr, n);
            CopyArray(tempArr, arr, n);
            quicksort<true>(arr, n);
            quicksortHybridThreshold<true>(tempArr, n, 5);
        }
    }

    profiler.divideValues(quicksortAssign, numOfTests);
    profiler.divideValues(quicksortCompare, numOfTests);
    profiler.divideValues(hybridAssign, numOfTests);
    profiler.divideValues(hybridCompare, numOfTests);

    profiler.addSeries(quicksortOps, quicksortAssign, quicksortCompare);
    profiler.addSeries(hybridQuicksort, hybridAssign, hybridCompare);

    profiler.createGroup(quickVsHybridOps, quicksortOps, hybridQuicksort);
    profiler.createGroup(quickVsHybridAssign, quicksortAssign, hybridAssign);
    profiler.createGroup(quickVsHybridCompare, quicksortCompare, hybridCompare);
}

static void quicksortVsHybridTiming() {
    constexpr int maxN = 10000;
    constexpr int workBatch = 15;
    constexpr int rounds = 100;
    //without static it's too big for the stack
    static int work[workBatch][maxN], src[workBatch][maxN];

    //pre init to remove randoms in-between thresholds
    for (auto &arr: src) {
        FillRandomArray(arr, maxN);
    }

    //multiple rounds to remove random cpu spikes
    for (int round = 1; round <= rounds; round++) {
        std::cout << "\nRound: " << round << " / " << rounds << " =====================";

        for (int n = 100; n <= maxN; n += 100) {
            CopyArray(work, src, workBatch);

            profiler.startTimer(hybridTime, n);
            for (auto &arr: work) {
                quicksortHybridThreshold<false>(arr, n, 50);
            }
            profiler.stopTimer(hybridTime, n);

            CopyArray(work, src, workBatch);

            profiler.startTimer(quicksortTime, n);
            for (auto &arr: work) {
                quicksort<false>(arr, n);
            }
            profiler.stopTimer(quicksortTime, n);
        }
    }

    profiler.createGroup(quickVsHybridTiming, quicksortTime, hybridTime);
}

int main() {
    profiler.reset("Quicksort_Cases_Comparison");
    quicksortCompareCases();
    profiler.reset("Quicksort_vs_Heapsort_Comparison");
    quicksortVsHeapsortNumOfOps();
    profiler.reset("Hybrid_Quicksort_Threshold_Finding");
    findThresholdsNumOfOps();
    findThresholdsTiming();
    profiler.reset("Quicksort_vs_Hybrid_Quicksort_Comparison");
    quicksortVsHybridNumOfOps();
    quicksortVsHybridTiming();
    profiler.showReport();
    checkQuicksortCorrectness();
    checkHybridQuicksortCorrectness();
    checkQuicksortRandoCorrectness();
    return 0;
}
