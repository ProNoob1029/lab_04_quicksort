/**
 *  Author: Morar Dragoș Bogdan
 *  Group: 30424
 *  Lab: 4
 *  Subject: Quicksort Comparisons
 *  Quicksort cases:
 *          Average: Quicksort good yes O(n log n)!
 *          Best: Take an already sorted array and
 *      recursively swap the middle of the partition
 *      with the end so the pivot selection is always
 *      good.
 *          Worst: A sorted array is worst when we
 *      take as pivot the end of the array because it
 *      has to always swap.
 *  Quicksort vs Heapsort:
 *      Heapsort does about 58% more operations than
 *      quicksort, although quicksort is much more
 *      random.
 *  Hybridization threshold:
 *          From my very difficult findings, hybridization
 *      has less operations than normal quicksort only
 *      for a threshold of 5, and it's only a 1%
 *      improvement. Increasing threshold increases
 *      operations.
 *          When testing time, the advantage of hybrid
 *      quicksort shows, because the linear operations
 *      of insertions sort go quicker. Here I struggled
 *      to get a clean graph, so I resorted to modifying
 *      the profiler in order to be able to calculate
 *      the min and median of the statistics. Even then
 *      I had to run a lot of runs to smooth it out.
 *      For our operations on ints I found a good
 *      thresholds of 50.
 *  Quicksort vs Hybrid Quicksort
 *          Operations are only 1% better for hybrid
 *      quicksort with a thresholds of 5.
 *          Time is about 37% better for hybrid
 *      quicksort.
 */

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
