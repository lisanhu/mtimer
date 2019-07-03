cmake_minimum_required(VERSION 3.14)
project(mtimer C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_executable(mtimer-c main.c mtimer.h)
add_executable(mtimer-cc main.cc mtimer.h)
