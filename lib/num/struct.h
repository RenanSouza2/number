#ifndef __NUM_STRUCT_H__
#define __NUM_STRUCT_H__

#include <stdbool.h>

#include "../../mods/macros/struct.h"
#include "../../mods/macros/uint.h" // IWYU pragma: keep

typedef uint64_t * chunk_p;
[[maybe_unused]]
constexpr uint64_t chunk_bits_log_2 = 6;
[[maybe_unused]]
constexpr uint64_t chunk_bits = 64;

STRUCT(num)
{
    uint64_t size;
    uint64_t count;
    chunk_p chunk;
    bool cannot_expand;
};

STRUCT(num_ssm)
{
    uint64_t count;
    num_p num_fft;
};

STRUCT(ssm_params)
{
    uint64_t count;
    uint64_t M;
    uint64_t K;
    uint64_t Q;
    uint64_t n;
};

#endif
