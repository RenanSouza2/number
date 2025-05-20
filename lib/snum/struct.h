#ifndef __SIG_STRUCT_H__
#define __SIG_STRUCT_H__

#include "../num/struct.h"

#define POSITIVE 1
#define NEGATIVE 2
#define ZERO 3

STRUCT(snum)
{
    uint64_t signal;
    num_t num;
};

#endif
