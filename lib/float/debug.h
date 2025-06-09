#ifndef __FLOAT_DEBUG_H__
#define __FLOAT_DEBUG_H__

#include <stdbool.h>

#include "header.h"

#ifdef DEBUG

float_num_t float_num_create_immed(
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    ...
);

#endif

float_num_t float_num_create(int64_t exponent, uint64_t size, sig_num_t sig);
void float_num_free(float_num_t flt);

bool float_num_is_zero(float_num_t flt);

#endif
