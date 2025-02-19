#ifndef __NUM_STRUCT_H__
#define __NUM_STRUCT_H__

#include "header.h"

// NOTE: NULL num pointer represents zero

STRUCT(node)
{
    uint64_t value;
    node_p next, prev;
};

STRUCT(num)
{
    uint64_t count;
    node_p head, tail;
};

#endif
