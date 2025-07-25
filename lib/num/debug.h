#ifndef __NUM_DEBUG_H__
#define __NUM_DEBUG_H__

#include "header.h"
#include "struct.h"

#ifdef DEBUG

#include <stdarg.h>

num_p num_create_variadic(uint64_t n, va_list *args);
num_p num_create_immed(uint64_t n, ...);
num_p num_create_rand(uint64_t count);

bool int64(int64_t i1, int64_t i2);
bool uint64(uint64_t u1, uint64_t u2);
bool uint128_immed(uint128_t u1, uint64_t v2h, uint64_t v2l);

bool num_inner(num_p num_1, num_p num_2);
bool num_eq_dbg(num_p num_1, num_p num_2);
bool num_immed(num_p num, uint64_t n, ...);

#endif

#define U128(V) ((uint128_t)(V))
#define U128_IMMED(V1, V2) ((U128(V1) << 64) | (V2))
#define MUL(V1, V2) U128(V1) * U128(V2)
#define LOW(V) U64(V)
#define HIGH(V) U64((V) >> 64)

uint64_t uint_from_char(char c);
uint64_t uint_inv(uint64_t value, uint64_t q);

num_p num_create(uint64_t size, uint64_t count);
num_p num_expand_to(num_p num, uint64_t target);
num_p num_expand(num_p num);
uint64_t num_chunk_get(num_p num, uint64_t pos);
num_p num_chunk_set(num_p num, uint64_t pos, uint64_t value);

bool num_normalize(num_p num);

num_p num_wrap_dec(char str[]);
num_p num_wrap_hex(char str[]);

num_p num_shl_inner(num_p num, uint64_t bits);
num_p num_shr_inner(num_p num, uint64_t bits);

num_p num_add_uint_offset(num_p num, uint64_t pos, uint64_t value);
num_p num_sub_uint_offset(num_p num, uint64_t pos, uint64_t value);

int64_t num_cmp_offset(num_p num_1, uint64_t pos_1, num_p num_2);
num_p num_sub_offset(num_p num_1, uint64_t pos_1, num_p num_2);

num_p num_ssm_pad(num_p num, uint64_t b, uint64_t n, uint64_t k);
void num_ssm_shl_mod(
    num_p num_aux,
    num_p num,
    uint64_t pos,
    uint64_t n,
    uint64_t bits
);
void num_ssm_add_mod(
    num_p num_res,
    uint64_t pos_res,
    num_p num,
    uint64_t pos_1,
    uint64_t pos_2,
    uint64_t n
);
void num_ssm_sub_mod(
    num_p num_res,
    uint64_t pos_res,
    num_p num_1,
    uint64_t pos_1,
    num_p num_2,
    uint64_t pos_2,
    uint64_t n
);
void num_ssm_shl(
    num_p num_res,
    uint64_t pos_res,
    num_p num,
    uint64_t pos,
    uint64_t n,
    uint64_t bits
);
void num_ssm_shr(
    num_p num_res,
    uint64_t pos_res,
    num_p num,
    uint64_t pos,
    uint64_t n,
    uint64_t bits
);
void num_ssm_shr_mod(
    num_p num_aux,
    num_p num,
    uint64_t pos,
    uint64_t n,
    uint64_t bits
);
void num_ssm_fft_fwd(
    num_p num_aux,
    num_p num,
    uint64_t n,
    uint64_t k,
    uint64_t bits
);
void num_ssm_mul_tmp(
    num_p num_res,
    num_p num_1,
    num_p num_2,
    uint64_t pos,
    uint64_t n
);

num_p num_mul_classic(num_p num_1, num_p num_2);
num_p num_sqr_classic(num_p num);

uint64_t num_div_normalize(num_p *num_1, num_p *num_2);

#endif
