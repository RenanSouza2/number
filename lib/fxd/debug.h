#ifndef __FXD_DEBUG_H__
#define __FXD_DEBUG_H__

#include "header.h"

#ifdef DEBUG

fxd_num_t fxd_num_create_immed(
    uint64_t pos,
    uint64_t signal,
    uint64_t n,
    ...
);

bool fxd_num_immed(
    fxd_num_t fxd,
    uint64_t pos,
    uint64_t signal,
    uint64_t n,
    ...
);

#endif

fxd_num_t fxd_num_create(sig_num_t sig, uint64_t pos);

#endif
