#ifndef __FLOAT_STRUCT_H__
#define __FLOAT_STRUCT_H__

#include "../sig/struct.h"

STRUCT(float_num)
{
    int64_t exponent;
    uint64_t size;
    sig_num_t sig;
};

#endif
