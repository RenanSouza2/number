#ifndef __FLT_H__
#define __FLT_H__

#include <stdio.h>
#include <stdbool.h>

#include "../fxd/header.h"

#include "struct.h"

void flt_num_display(flt_num_t flt);
void flt_num_display_dec(flt_num_t flt);

flt_num_t flt_num_wrap(int64_t value, uint64_t size);
flt_num_t flt_num_wrap_num(num_p num, uint64_t size);
flt_num_t flt_num_wrap_sig(sig_num_t sig, uint64_t size);
flt_num_t flt_num_copy(flt_num_t flt);
void flt_num_free(flt_num_t flt);

fxd_num_t fxd_num_wrap_flt(flt_num_t flt, uint64_t pos);

void flt_num_file_write(FILE *fp, flt_num_t flt);
void flt_num_save(char file_path[], flt_num_t flt);
flt_num_t flt_num_file_read(FILE *fp);
flt_num_t flt_num_load(char file_path[]);

int64_t flt_num_cmp(flt_num_t flt_1, flt_num_t flt_2);
bool flt_num_safe_add(flt_num_t flt_1, flt_num_t flt_2);

flt_num_t flt_num_shr(flt_num_t flt, uint64_t bits);

flt_num_t flt_num_add(flt_num_t flt_1, flt_num_t flt_2);
flt_num_t flt_num_sub(flt_num_t flt_1, flt_num_t flt_2);
flt_num_t flt_num_mul(flt_num_t flt_1, flt_num_t flt_2);
flt_num_t flt_num_pow(flt_num_t flt, int64_t value);
flt_num_t flt_num_sqr(flt_num_t flt);
flt_num_t flt_num_div(flt_num_t flt_1, flt_num_t flt_2);

flt_num_t flt_num_mul_sig(flt_num_t flt, sig_num_t sig);
flt_num_t flt_num_div_sig(flt_num_t flt, sig_num_t sig);

void del();

#endif
