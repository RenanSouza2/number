#ifndef __FLT_DEBUG_H__
#define __FLT_DEBUG_H__

#include <stdbool.h>

#include "header.h"

#ifdef DEBUG

flt_num_t flt_num_create_immed(
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    ...
);

bool flt_num_immed(
    flt_num_t flt,
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    ...
);

#endif

uint64_t int64_get_sign(int64_t i);
int64_t int64_add(int64_t a, int64_t b);

flt_num_t flt_num_normalize(flt_num_t flt);
flt_num_t flt_num_create(int64_t exponent, uint64_t size_cap, sig_num_t sig);

flt_num_t flt_num_set_exponent(flt_num_t flt, int64_t exponent);
flt_num_t flt_num_set_size(flt_num_t flt, uint64_t size);

bool flt_num_is_zero(flt_num_t flt);

#endif
