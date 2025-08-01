#ifndef __FIX_STRUCT_H__
#define __FIX_STRUCT_H__

#include "../sig/struct.h"

STRUCT(fix_num)
{
    uint64_t pos;
    sig_num_t sig;
};

#endif
