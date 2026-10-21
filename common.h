#pragma once
#include "Profiler.h"

static auto profiler = Profiler();

static int N_SIZE = 0;

static void printArray(int arr[], const int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << '\n';
}