#ifndef __FLOAT_STRUCT_H__
#define __FLOAT_STRUCT_H__

#include "../sig/struct.h"

STRUCT(float_num)
{
    uint64_t exponent;
    uint64_t size_cap;
    sig_num_t sig;
};

#endif
