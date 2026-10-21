#pragma once
#include "common.h"

constexpr char hybridAssign[] = "hybrid_quicksort_assign";
constexpr char hybridCompare[] = "hybrid_quicksort_compare";

const int THRESHOLD = 5;

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
    const int pos = partition(arr, left, right);
    quicksortHybrid(arr, left, pos - 1);
    quicksortHybrid(arr, pos + 1, right);
}

static void quicksortHybrid(int arr[], const int n) {
    N_SIZE = n;
    quicksortHybrid(arr, 0, n - 1);
}