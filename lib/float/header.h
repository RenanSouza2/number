#ifndef __FLOAT_H__
#define __FLOAT_H__

#include <stdio.h>
#include <stdbool.h>

#include "../fix/header.h"

#include "struct.h"

void float_num_display(float_num_t flt);
void float_num_display_dec(float_num_t flt);

float_num_t float_num_wrap(int64_t value, uint64_t size);
float_num_t float_num_wrap_num(num_p num, uint64_t size);
float_num_t float_num_wrap_sig(sig_num_t sig, uint64_t size);
float_num_t float_num_copy(float_num_t flt);
void float_num_free(float_num_t flt);

fix_num_t fix_num_wrap_float(float_num_t flt, uint64_t pos);

void float_num_file_write(FILE *fp, float_num_t flt);
void float_num_save(char file_path[], float_num_t flt);
float_num_t float_num_file_read(FILE *fp);
float_num_t float_num_load(char file_path[]);

int64_t float_num_cmp(float_num_t flt_1, float_num_t flt_2);
bool float_num_safe_add(float_num_t flt_1, float_num_t flt_2);

float_num_t float_num_shr(float_num_t flt, uint64_t bits);

float_num_t float_num_add(float_num_t flt_1, float_num_t flt_2);
float_num_t float_num_sub(float_num_t flt_1, float_num_t flt_2);
float_num_t float_num_mul(float_num_t flt_1, float_num_t flt_2);
float_num_t float_num_pow(float_num_t flt, int64_t value);
float_num_t float_num_sqr(float_num_t flt);
float_num_t float_num_div(float_num_t flt_1, float_num_t flt_2);

float_num_t float_num_mul_sig(float_num_t flt, sig_num_t sig);
float_num_t float_num_div_sig(float_num_t flt, sig_num_t sig);

void del();

#endif
