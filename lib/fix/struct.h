#ifndef __FIX_STRUCT_H__
#define __FIX_STRUCT_H__

#include "../snum/struct.h"

STRUCT(fix)
{
    sig_num_t sig;
    uint64_t pos;
};

#endif
