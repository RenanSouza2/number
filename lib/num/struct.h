#ifndef __NUM_STRUCT_H__
#define __NUM_STRUCT_H__

#include "header.h"

// NOTE: NULL num pointer represents zero

STRUCT(num)
{
    uint64_t value;
    num_p next;
};

#endif
