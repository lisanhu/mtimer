//
// Created by Sanhu Li on 2019-06-28.
//

#ifndef MLOG_TIMER_H
#define MLOG_TIMER_H

#ifdef __cplusplus
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#else
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#endif
#include <pthread.h>

typedef struct mtimer_entries {
    char const **fnames;
    clock_t *elapses;
    uint64_t len, cap;
    pthread_mutex_t lock;
} mtimer_entries;

extern mtimer_entries logger;

static void dump_log() {
    for (uint64_t i = 0; i < logger.len; ++i) {
        fprintf(stderr, "%s | %.3Lf |\n", logger.fnames[i],
                (long double) (logger.elapses[i]) / CLOCKS_PER_SEC * 1000);
    }
}

#define CHECK_PTR(ptr) \
if (!(ptr)) { \
    perror(NULL); \
    exit(-1); \
}

static mtimer_entries log_new() {
    int const INIT_CAP = 100000;

    char const **fnames = (char const **) (
            malloc(INIT_CAP * sizeof(char const *))
    );

    CHECK_PTR(fnames)

    clock_t *elapses = (clock_t *) (
            malloc(INIT_CAP * sizeof(clock_t))
    );

    CHECK_PTR(elapses)

    mtimer_entries log;
    log.fnames = fnames;
    log.elapses = elapses;
    log.len = 0;
    log.cap = INIT_CAP;
    pthread_mutex_init(&log.lock, NULL);
    return log;
}

static void log_puts(mtimer_entries *log, char const *fname, clock_t elapse) {
    pthread_mutex_lock(&log->lock);

    if (log->cap < log->len + 1) {
        char const **fnames = (char const **) (
                realloc((void *) log->fnames, 2 * log->cap * sizeof(char const *))
        );
        clock_t *elapses = (clock_t *) (
                realloc((void *) log->elapses, 2 * log->cap * sizeof(clock_t))
        );

        if (!fnames || !elapses) {
            perror(NULL);
            exit(-1);
        } else {
            log->cap *= 2;
            log->fnames = fnames;
            log->elapses = elapses;
        }
    }

    log->fnames[log->len] = fname;
    log->elapses[log->len] = elapse;
    log->len++;

    pthread_mutex_unlock(&log->lock);
}

#define TIMER_START \
clock_t timer = clock();

static void _timer_stop(char const *fname, clock_t start) {
    log_puts(&logger, fname, clock() - start);
}

#define TIMER_STOP \
_timer_stop(__PRETTY_FUNCTION__, timer);

#ifdef __cplusplus

template<typename ... Ts>
static void _timer_stop(char const *fname, clock_t start) {
    log_puts(&logger, fname, clock() - start);
}

#define TIMER_T_STOP(T) \
_timer_stop<T>(__PRETTY_FUNCTION__, timer);

#else
#endif

#endif