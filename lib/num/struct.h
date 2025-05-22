#ifndef __NUM_STRUCT_H__
#define __NUM_STRUCT_H__

#include "../../mods/macros/struct.h"
#include "../../mods/macros/U64.h"

STRUCT(chunk)
{
    uint64_t value;
    chunk_p next, prev;
};

// HEAD is the least significant chunk
// if TAIL is not null, is never zero
STRUCT(num)
{
    uint64_t count;
    chunk_p head, tail;
};

#endif
