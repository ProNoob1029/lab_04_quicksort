#pragma once
#include <iostream>
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

static bool compare(const int first, const int second) {
    profiler.countOperation(heapsortCompareAverage, N_SIZE);
    return first > second;
}

static void heapify(int arr[], const int size, const int i) {
    const int leftI = left(i);
    const int rightI = right(i);
    int swapI = i;
    if (leftI <= size && compare(arr[leftI], arr[swapI])) {
        swapI = leftI;
    }
    if (rightI <= size && compare(arr[rightI], arr[swapI])) {
        swapI = rightI;
    }
    if (swapI != i) {
        std::swap(arr[i], arr[swapI]);
        profiler.countOperation(heapsortAssignAverage, N_SIZE, 3);
        heapify(arr, size, swapI);
    }
}

static void buildHeap(int arr[], const int size) {
    for (int i = size / 2; i > 0; i--) {
        heapify(arr, size, i);
    }
}

static void heapsort(int arr[], const int n) {
    N_SIZE = n;
    buildHeap(arr, n);
    int heapSize = n;
    for (int i = n; i >= 2; i--) {
        std::swap(arr[1], arr[i]);
        profiler.countOperation(heapsortAssignAverage, N_SIZE, 3);
        heapSize--;
        heapify(arr, heapSize, 1);
    }
}
