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

bool float_num_immed(
    float_num_t flt,
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    ...
);

#endif

uint64_t int64_get_sign(int64_t i);
int64_t int64_add(int64_t a, int64_t b);

float_num_t float_num_normalize(float_num_t flt);
float_num_t float_num_create(int64_t exponent, uint64_t size_cap, sig_num_t sig);
void float_num_free(float_num_t flt);

float_num_t float_num_set_exponent(float_num_t flt, int64_t exponent);

bool float_num_is_zero(float_num_t flt);

#endif
