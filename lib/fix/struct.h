#ifndef __FIX_STRUCT_H__
#define __FIX_STRUCT_H__

#include "../sig/struct.h"

STRUCT(fix_num)
{
    sig_num_t sig;
    uint64_t pos;
};

#endif
