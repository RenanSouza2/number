#ifndef __SIG_STRUCT_H__
#define __SIG_STRUCT_H__

#include "header.h"

#include "../num/header.h"

STRUCT(sig)
{
    num_p num;
    uint64_t signal;
};

#endif
