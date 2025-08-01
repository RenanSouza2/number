#ifndef __FXD_H__
#define __FXD_H__

#include <stdbool.h>

#include "struct.h"

void fxd_num_display_dec(fxd_num_t fxd);
void fxd_num_display(fxd_num_t fxd);
void fxd_num_display_tag(char tag[], fxd_num_t fxd);
void fxd_num_display_full(char tag[], fxd_num_t fxd);

fxd_num_t fxd_num_reposition(fxd_num_t fxd, uint64_t pos);

fxd_num_t fxd_num_wrap(int64_t value, uint64_t pos);
fxd_num_t fxd_num_copy(fxd_num_t fxd);
void fxd_num_free(fxd_num_t fxd);

fxd_num_t fxd_num_base_to(fxd_num_t fxd, uint64_t base);

int64_t fxd_num_cmp(fxd_num_t fxd_1, fxd_num_t fxd_2);
bool fxd_num_is_zero(fxd_num_t fxd);

fxd_num_t fxd_num_shl(fxd_num_t fxd, uint64_t value);
fxd_num_t fxd_num_shr(fxd_num_t fxd, uint64_t value);

fxd_num_t fxd_num_add(fxd_num_t fxd_1, fxd_num_t fxd_2);
fxd_num_t fxd_num_sub(fxd_num_t fxd_1, fxd_num_t fxd_2);
fxd_num_t fxd_num_mul(fxd_num_t fxd_1, fxd_num_t fxd_2);
fxd_num_t fxd_num_sqr(fxd_num_t fxd);
fxd_num_t fxd_num_div(fxd_num_t fxd_1, fxd_num_t fxd_2);

fxd_num_t fxd_num_mul_sig(fxd_num_t fxd, sig_num_t sig);
fxd_num_t fxd_num_div_sig(fxd_num_t fxd, sig_num_t sig);

#endif
