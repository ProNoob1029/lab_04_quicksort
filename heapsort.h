#pragma once
#include "common.h"

constexpr char algorithmsComparison[] = "1_quicksort_vs_heapsort_ops_comparison";
constexpr char algorithmsAssignComparison[] = "quicksort_vs_heapsort_assign_comparison";
constexpr char algorithmsCompareComparison[] = "quicksort_vs_heapsort_compare_comparison";

constexpr char heapsortOpsAverage[] = "heapsort_ops_average";
constexpr char heapsortAssignAverage[] = "heapsort_assign_average";
constexpr char heapsortCompareAverage[] = "heapsort_compare_average";

static int left(const int i) {
    return 2 * i;
}

static int right(const int i) {
    return 2 * i + 1;
}

template<bool COUNT>
static void heapify(int arr[], const int size, const int i) {
    const int leftI = left(i);
    const int rightI = right(i);
    int swapI = i;
    if (leftI <= size && greater<COUNT>(arr[leftI], arr[swapI], heapsortCompareAverage)) {
        swapI = leftI;
    }
    if (rightI <= size && greater<COUNT>(arr[rightI], arr[swapI], heapsortCompareAverage)) {
        swapI = rightI;
    }
    if (swapI != i) {
        std::swap(arr[i], arr[swapI]);
        if constexpr (COUNT) profiler.countOperation(heapsortAssignAverage, N_SIZE, 3);
        heapify<COUNT>(arr, size, swapI);
    }
}

template<bool COUNT>
static void buildHeap(int arr[], const int size) {
    for (int i = size / 2; i > 0; i--) {
        heapify<COUNT>(arr, size, i);
    }
}

template<bool COUNT>
static void heapsort(int arr[], const int n) {
    N_SIZE = n;
    buildHeap<COUNT>(arr, n);
    int heapSize = n;
    for (int i = n; i >= 2; i--) {
        std::swap(arr[1], arr[i]);
        if constexpr (COUNT) profiler.countOperation(heapsortAssignAverage, N_SIZE, 3);
        heapSize--;
        heapify<COUNT>(arr, heapSize, 1);
    }
}
