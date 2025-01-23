#ifndef __NUMBER_STRUCT_H__
#define __NUMBER_STRUCT_H__

#include "header.h"

#include <stdint.h>

// NOTE: NULL number pointer represents zero

STRUCT(number)
{
    uint64_t value;
    number_p next;
};

#endif
