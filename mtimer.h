//
// Created by Sanhu Li on 2019-06-28.
//

#ifndef MLOG_TIMER_H
#define MLOG_TIMER_H

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct timespec ts_t;

typedef struct mtimer {
    ts_t counter;
    ts_t start;
} timer;

static void timer_start(mtimer *timer);
static void timer_stop(mtimer *timer);
static void timer_log(FILE *stream, char const *function_name, mtimer t);
static void timer_elog(char const *function_name, mtimer t);
static void timer_vlog(char const *function_name, mtimer t);

#ifdef __cplusplus
};
#endif

static double ts_to_milli_seconds(ts_t t);

double ts_to_milli_seconds(ts_t t) {
    return t.tv_sec * 1E3 + t.tv_nsec / 1E6;
}

void timer_start(mtimer *timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->start);
}

void timer_stop(mtimer *timer) {
    ts_t now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    timer->counter.tv_sec += now.tv_sec - timer->start.tv_sec;
    timer->counter.tv_nsec += now.tv_nsec - timer->start.tv_nsec;
}

void timer_log(FILE *stream, char const *function_name, mtimer t) {
    fprintf(stream, "%s | %.3lf |\n", function_name, ts_to_milli_seconds(t.counter));
}

void timer_elog(char const *function_name, mtimer t) {
    timer_log(stderr, function_name, t);
}

void timer_vlog(char const *function_name, mtimer t) {
    timer_log(stdout, function_name, t);
}

#endif