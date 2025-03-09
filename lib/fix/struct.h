#ifndef __FIX_STRUCT_H__
#define __FIX_STRUCT_H__

#include <stdint.h>

#include "header.h"

#include "../snum/header.h"

STRUCT(fix)
{
    snum_p snum;
    uint64_t pos;
};

#endif
