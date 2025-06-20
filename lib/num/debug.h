#ifndef __NUM_DEBUG_H__
#define __NUM_DEBUG_H__

#include "header.h"
#include "struct.h"

typedef __uint128_t uint128_t;

#ifdef DEBUG

#include <stdarg.h>

num_p num_create_variadic(uint64_t n, va_list *args);
num_p num_create_immed(uint64_t n, ...);

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

num_p num_create(uint64_t size, uint64_t count);
num_p num_expand_to(num_p num, uint64_t target);
num_p num_expand(num_p num);
uint64_t num_chunk_get(num_p num, uint64_t pos);
num_p num_chunk_set(num_p num, uint64_t pos, uint64_t value);

bool num_normalize(num_p num);

num_p num_wrap_dec(char str[]);
num_p num_wrap_hex(char str[]);

int64_t num_cmp_offset(num_p num_1, uint64_t pos_1, num_p num_2);
num_p num_add_uint_offset(num_p num, uint64_t pos, uint64_t value);
num_p num_sub_uint_offset(num_p num, uint64_t pos, uint64_t value);
num_p num_cmp_mul_uint_offset(
    num_p num_res,
    num_p num_1,
    uint64_t pos_1,
    num_p num_2,
    uint64_t r
);

num_p num_shl_uint(num_p num, uint64_t bits);
num_p num_shr_uint(num_p num, uint64_t bits);
num_p num_add_uint(num_p num, uint64_t value);
num_p num_sub_uint(num_p num, uint64_t value);
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value);
num_p num_add_mul_uint(num_p num_res, num_p num, uint64_t value);

num_p num_sub_offset(num_p num_1, uint64_t pos_1, num_p num_2);

#endif
