#ifndef __SIG_STRUCT_H__
#define __SIG_STRUCT_H__

#include "header.h"

#include "../num/header.h"

STRUCT(snum)
{
    uint64_t signal;
    num_p num;
};

#endif
