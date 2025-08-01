#ifndef __FLT_STRUCT_H__
#define __FLT_STRUCT_H__

#include "../sig/struct.h"

STRUCT(flt_num)
{
    int64_t exponent;
    uint64_t size;
    sig_num_t sig;
};

#endif
