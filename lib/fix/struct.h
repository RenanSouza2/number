#ifndef __FIX_STRUCT_H__
#define __FIX_STRUCT_H__

#include <stdint.h>

#include "header.h"

#include "../snum/header.h"

STRUCT(fix)
{
    uint64_t size; // TODO change name
    snum_p snum;
};

#endif
