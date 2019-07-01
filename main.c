#include <stdio.h>
#include "timer.h"

int main() {
    timer t;
    timer_start(&t);
    ts_t interval = {0, 1000000L};
    nanosleep(&interval, NULL);
    timer_stop(&t);
    timer_elog(__PRETTY_FUNCTION__, t);
    return 0;
}