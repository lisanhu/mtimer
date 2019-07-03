#include <stdio.h>
#include "mtimer.h"

mtimer_logger logger;

typedef struct timespec ts_t;

int main() {
    logger = log_new();
    atexit(dump_log);
    TIMER_START
    ts_t interval = {1, 0};
    nanosleep(&interval, NULL);
    TIMER_STOP
    return 0;
}