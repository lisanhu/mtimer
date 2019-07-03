//
// Created by Sanhu Li on 2019-07-02.
//
#include "mtimer.h"


mtimer_logger logger;

typedef struct timespec ts_t;

ts_t const some_time = {1, 0}; // 1s

template<typename T>
class SomeClass {
    T ele;
public:
    explicit SomeClass(T v) : ele(v) {}

    T get() {
        TIMER_START
        nanosleep(&some_time, nullptr);
        printf("%s\n", __PRETTY_FUNCTION__);
        TIMER_T_STOP(T)
        return ele;
    }
};

double double_num(double n) {
    TIMER_START
    nanosleep(&some_time, nullptr);
    TIMER_STOP
    return n * 2;
}

int double_num(int n) {
    TIMER_START
    nanosleep(&some_time, nullptr);
    TIMER_STOP
    return n * 2;
}

int main(int argc, char const **argv) {
    atexit(dump_log);
    logger = log_new();
    SomeClass<int> e(10);
    SomeClass<double> f(1.5);

    double_num(f.get());
    double_num(f.get());
    double_num(e.get());
    return 0;
}