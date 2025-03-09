#ifndef __NUM_STRUCT_H__
#define __NUM_STRUCT_H__

#include "header.h"

STRUCT(chunk)
{
    uint64_t value;
    chunk_p next, prev;
};

STRUCT(num)
{
    uint64_t count;
    chunk_p head, tail;
};

#endif
