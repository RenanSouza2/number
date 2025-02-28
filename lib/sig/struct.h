#ifndef __SIG_STRUCT_H__
#define __SIG_STRUCT_H__

#include "header.h"

#include "../num/header.h"

STRUCT(sig)
{
    uint64_t signal;
    num_p num;
};

#endif
