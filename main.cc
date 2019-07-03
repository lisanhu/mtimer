//
// Created by Sanhu Li on 2019-07-02.
//

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include "mtimer.h"


typedef struct mtimer_entries {
	char const **fnames;
	uint64_t *elapsed;
	uint64_t len, cap;
} mtimer_entries;

template <typename T>
uint64_t timer_start(char const *fname) {
}

void dump_log() {
	fprintf(stderr, "Good Bye!\n");
}

typedef struct timespec ts_t;

ts_t const some_time = {0, 100000000}; // 100ms

template <typename T>
class SomeClass {
	T ele;
public:
	SomeClass(T v): ele(v) {}
	T get() {
		nanosleep(&some_time, nullptr); return ele;
	}
};

int main(int argc, char const **argv) {
	atexit(dump_log);
	SomeClass<int > e(10);
	SomeClass<double > f(1.5);
	return 0;
}