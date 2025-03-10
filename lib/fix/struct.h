#ifndef __FIX_STRUCT_H__
#define __FIX_STRUCT_H__

#include <stdint.h>

#include "header.h"

#include "../snum/struct.h"

STRUCT(fix)
{
    snum_t snum;
    uint64_t pos;
};

#endif
