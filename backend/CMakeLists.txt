cmake_minimum_required(VERSION 3.27)
project(MBTI_Melody)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MBTI_Melody
        main.cpp
        min_heap.h
        rb_map.h
        song.h)
