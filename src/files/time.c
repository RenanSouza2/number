#ifndef ___TIME_H__
#define ___TIME_H__

#include <time.h>

#include "../../mods/macros/U64.h"

uint64_t get_time()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return time.tv_sec * (uint64_t)1e9 + time.tv_nsec;
}

#endif
