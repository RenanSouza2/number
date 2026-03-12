#ifndef __NUM_STRUCT_H__
#define __NUM_STRUCT_H__

#include <stdbool.h>

#include "../../mods/macros/struct.h"
#include "../../mods/macros/uint.h"

typedef uint64_t * chunk_p;

STRUCT(num)
{
    uint64_t size;
    uint64_t count;
    chunk_p chunk;
    bool cannot_expand;
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
