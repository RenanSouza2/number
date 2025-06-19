#ifndef __NUM_DEBUG_H__
#define __NUM_DEBUG_H__

#include "header.h"

typedef __uint128_t uint128_t;

#ifdef DEBUG

#include <stdarg.h>

num_t num_create_variadic(uint64_t n, va_list *args);
num_t num_create_immed(uint64_t n, ...);

bool int64(int64_t i1, int64_t i2);
bool uint64(uint64_t u1, uint64_t u2);
bool uint128_immed(uint128_t u1, uint64_t v2h, uint64_t v2l);

bool num_inner(num_t num_1, num_t num_2);
bool num_eq_dbg(num_t num_1, num_t num_2);
bool num_immed(num_t num, uint64_t n, ...);

#endif

#define U128(V) ((uint128_t)(V))
#define U128_IMMED(V1, V2) ((U128(V1) << 64) | (V2))
#define MUL(V1, V2) U128(V1) * U128(V2)
#define LOW(V) U64(V)
#define HIGH(V) U64((V) >> 64)

uint64_t uint_from_char(char c);

num_t num_create(uint64_t size, uint64_t count);
num_t num_expand_to(num_t num, uint64_t target);
num_t num_expand(num_t num);
uint64_t num_chunk_get(num_t num, uint64_t pos);
num_t num_chunk_set(num_t num, uint64_t pos, uint64_t value);

chunk_p num_insert_tail(num_p num, uint64_t value);
num_t num_remove_head(num_t num);
chunk_p num_denormalize(num_p num, chunk_p chunk);
bool num_normalize(num_p num);

num_t num_wrap_dec(char str[]);
num_t num_wrap_hex(char str[]);

int64_t num_cmp_offset(num_t num_1, uint64_t pos_1, num_t num_2);
num_t num_add_uint_offset(num_t num, uint64_t pos, uint64_t value);
num_t num_sub_uint_offset(num_t num, uint64_t pos, uint64_t value);
num_t num_cmp_mul_uint_offset(
    num_t num_res,
    num_t num_1,
    uint64_t pos_1,
    num_t num_2,
    uint64_t r
);

num_t num_shl_uint(num_t num, uint64_t bits);
num_t num_shr_uint(num_t num, uint64_t bits);
num_t num_add_uint(num_t num, uint64_t value);
num_t num_sub_uint(num_t num, uint64_t value);
num_t num_mul_uint(num_t num, uint64_t value);
num_t num_add_mul_uint(num_t num_res, num_t num, uint64_t value);

num_t num_sub_offset(num_t num_1, uint64_t pos_1, num_t num_2);

#endif
