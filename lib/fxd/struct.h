#ifndef FXD_STRUCT_H
#define FXD_STRUCT_H

#include "../sig/struct.h"

STRUCT(fxd_num)
{
    uint64_t pos;
    sig_num_t sig;
};

STRUCT(fxd_num_ssm)
{
    uint64_t pos;
    sig_num_ssm_t sig_ssm;
};

#endif
