//
// Created by Sanhu Li on 2019-07-08.
//

#include <stdio.h>
#include <string.h>

#include "mtimer.h"

mtimer_logger logger;

template<typename T>
T addup(T a, T b) {
    TIMER_START
    int mem;
    memset(&mem, 1, sizeof(int));
    for (uint64_t i = 0; i < 500000000UL; ++i) {
        memset(&mem, 1, sizeof(int));
    }
    TIMER_STOP
    return a + b;
}

int main() {
    logger = log_new();
    atexit(dump_log);
    printf("Hello playground!\n");

    printf("Adding up 1, and 2: %d\n", addup(1, 2));
    printf("Adding up 1, and 2: %d\n", addup(1, 2));
    printf("Adding up 1.1, and 2.1: %f\n", addup(1.1, 2.1));
    printf("Adding up 1.1, and 2.1: %f\n", addup(1.1, 2.1));
    return 0;
}