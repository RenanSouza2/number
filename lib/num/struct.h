#ifndef __NUM_STRUCT_H__
#define __NUM_STRUCT_H__

#include "../../mods/macros/struct.h"
#include "../../mods/macros/U64.h"

typedef uint64_t * chunk_p;

STRUCT(num)
{
    uint64_t size;
    uint64_t count;
    chunk_p chunk;
};

#endif
