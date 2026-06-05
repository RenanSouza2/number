#ifndef FLT_STRUCT_H
#define FLT_STRUCT_H

#include "../sig/struct.h"

STRUCT(flt_num)
{
    int64_t exponent;
    uint64_t size;
    sig_num_t sig;
};

STRUCT(flt_num_ssm)
{
    int64_t exponent;
    uint64_t size;
    sig_num_ssm_t sig_ssm;
};

#endif
