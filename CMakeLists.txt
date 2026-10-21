cmake_minimum_required(VERSION 4.3)
project(lab_04_quicksort)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab_04_quicksort main.cpp
        Profiler.h
        heapsort.h
        common.h
        quicksort.h)
