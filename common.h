#pragma once
#include <iostream>
#include "Profiler.h"

static auto profiler = Profiler();

static int N_SIZE = 0;

static void printArray(int arr[], const int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << '\n';
}


template<bool COUNT>
static bool greater(const int first, const int second, const char opName[]) {
    if constexpr (COUNT) profiler.countOperation(opName, N_SIZE);
    return first > second;
}
