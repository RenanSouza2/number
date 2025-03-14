#ifndef __NUM_DEBUG_H__
#define __NUM_DEBUG_H__

#include "struct.h"

typedef __uint128_t uint128_t;

#ifdef DEBUG

#include <stdarg.h>

num_t num_create_immed(uint64_t n, ...);
num_t num_create_variadic(uint64_t n, va_list *args);
void num_create_immed_vec(num_t out_num[], uint64_t n, ...);

bool int64(int64_t u1, int64_t u2);
bool uint64(uint64_t u1, uint64_t u2);
bool uint128_immed(uint128_t u1, uint64_t v2h, uint64_t v2l);

bool num_str(num_t num_1, num_t num_2);
bool num_immed(num_t num, uint64_t n, ...);

chunk_p num_get_chunk(num_t num, uint64_t count);

#endif

#define U128(V) ((uint128_t)(V))
#define U128_IMMED(V1, V2) ((U128(V1) << 64) | (V2))
#define MUL(V1, V2) U128(V1) * U128(V2)
#define LOW(V) ((uint64_t)(V))
#define HIGH(V) LOW((V) >> 64)

uint64_t uint_from_char(char c);

chunk_p chunk_create(uint64_t value, chunk_p next, chunk_p prev);
void chunk_free_list(chunk_p head, chunk_p tail);

num_t num_create(uint64_t count, chunk_p head, chunk_p tail);
chunk_p num_insert_tail(num_p num, uint64_t value);
chunk_p num_insert_head(num_p num, uint64_t value);
num_t num_remove_head(num_t num);
chunk_p num_denormalize(num_p num, chunk_p chunk);
bool num_normalize(num_p num);

num_t num_wrap_dec(char str[]);
num_t num_wrap_hex(char str[]);

chunk_p num_sub_offset(num_p num_1, chunk_p chunk_1, num_t num_2);

num_t num_base_to(num_t num, uint64_t value);
num_t num_base_from(num_t num, uint64_t value);

bool num_sub_uint_offset(num_p num, chunk_p chunk, uint64_t value);

num_t num_shl_uint(num_t num, uint64_t bits);
num_t num_shr_uint(num_t num, uint64_t bits);
num_t num_add_uint(num_t num, uint64_t value);
num_t num_sub_uint(num_t num, uint64_t value);
num_t num_mul_uint(num_t num, uint64_t value);
num_t num_add_mul_uint(num_t num_res, num_t num, uint64_t value);

#endif
