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
#include <cstdbool>
#include <cstdarg>
#include <cstring>
#else
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#endif
#include <pthread.h>

typedef struct mtimer_logger {
    char const **fnames;
    clock_t *elapses;
    uint64_t len, cap;
    pthread_mutex_t lock;
} mtimer_logger;

extern mtimer_logger logger;

//static void dump_log() {
//    for (uint64_t i = 0; i < logger.len; ++i) {
//        fprintf(stderr, "%s | %.3Lf | %p\n", logger.fnames[i],
//                (long double) (logger.elapses[i]) / CLOCKS_PER_SEC * 1000,
//                logger.fnames[i]);
//    }
//}

#define CHECK_PTR(ptr) \
if (!(ptr)) { \
    perror(NULL); \
    exit(-1); \
}

static mtimer_logger log_new() {
    int const INIT_CAP = 100000;

    char const **fnames = (char const **) (
            malloc(INIT_CAP * sizeof(char const *))
    );

    CHECK_PTR(fnames)

    clock_t *elapses = (clock_t *) (
            malloc(INIT_CAP * sizeof(clock_t))
    );

    CHECK_PTR(elapses)

    mtimer_logger log;
    log.fnames = fnames;
    log.elapses = elapses;
    log.len = 0;
    log.cap = INIT_CAP;
    pthread_mutex_init(&log.lock, NULL);
    return log;
}

static void log_puts(mtimer_logger *log, char const *fname, clock_t elapse) {
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


typedef struct ht_entry_t {
    char const *fname;
    uint64_t clocks, counter;
} ht_entry_t;

#define HT_NUM_ENTRIES 101
typedef struct ht_t {
    ht_entry_t entries[HT_NUM_ENTRIES];
} ht_t;

static ht_t ht_init() {
    ht_t ht;
    memset(&ht.entries, 0, HT_NUM_ENTRIES * sizeof(ht_entry_t));
    return ht;
}

static int hash_f(char const *key) {
    return (uint64_t) key % HT_NUM_ENTRIES;
}

static int next_i(int start, int cur, int limit) {
    if (limit <= cur + 1) {
        cur = 0;
    }
    cur++;
    if (start == cur) {
        return limit;
    }
    return cur;
}

static int search_slot(ht_t ht, char const *key) {
    int h = hash_f(key);

    /// if it's not empty
    int total_entries = HT_NUM_ENTRIES;
    for (int i = h; i < total_entries; i = next_i(h, i, total_entries)) {
        if (ht.entries[i].fname == key) {
            /// found
            return i;
        } else if (ht.entries[i].fname == NULL) {
            /// first time
            return i;
        }
    }
    return -1;
}

static int eprintf(char const *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int v = vfprintf(stderr, fmt, args);
    va_end(args);
    return v;
}


static bool ht_accumulate(ht_t *ht, char const *fn, uint64_t clocks) {
    int idx = search_slot(*ht, fn);
    if (idx != -1) {
        ht->entries[idx].fname = fn;
        ht->entries[idx].clocks += clocks;
        ht->entries[idx].counter++;
    } else {
        eprintf("Error for key: %s, value: %ul, probably hash table too small", fn, clocks);
        return false;
    }
    return true;
}

static void dump_log() {
    ht_t ht = ht_init();

    eprintf("Function Name : Total Time : Num of Calls : Average Time \n");

    for (int i = 0; i < logger.len; ++i) {
        ht_accumulate(&ht, logger.fnames[i], logger.elapses[i]);
    }

    for (int i = 0; i < HT_NUM_ENTRIES; ++i) {
//        eprintf("%s %d %d\n", ht.entries[i].fname, ht.entries[i].clocks, ht.entries[i].counter);
        if (ht.entries[i].fname != NULL && ht.entries[i].counter > 0) {
            uint64_t clocks = ht.entries[i].clocks;
            long double time = (long double) clocks / CLOCKS_PER_SEC * 1000; // in milli seconds
            long double avg_time = time / ht.entries[i].counter;
            eprintf("%s : %Lf : %lu : %Lf \n",
                    ht.entries[i].fname,
                    time,
                    ht.entries[i].counter,
                    avg_time
            );
        }
    }
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