#ifndef __NUM_DEBUG_H__
#define __NUM_DEBUG_H__

#include "struct.h"

typedef __uint128_t uint128_t;

#ifdef DEBUG

#include <stdarg.h>

num_p num_create_immed(uint64_t n, ...);
num_p num_create_variadic(uint64_t n, va_list args);

bool int64(int64_t u1, int64_t u2);
bool uint64(uint64_t u1, uint64_t u2);
bool uint128_immed(uint128_t u1, uint64_t v2h, uint64_t v2l);

bool num_str(num_p num_1, num_p num_2);
bool num_immed(num_p num, uint64_t n, ...);

chunk_p num_get_chunk(num_p num, uint64_t count);

#endif

#define U128(V) ((uint128_t)(V))
#define U128_IMMED(V1, V2) ((U128(V1) << 64) | (V2))
#define MUL(V1, V2) U128(V1) * U128(V2)
#define LOW(V) ((uint64_t)(V))
#define HIGH(V) LOW((V) >> 64)

uint64_t uint_from_char(char c);

chunk_p chunk_create(uint64_t value, chunk_p next, chunk_p prev);
chunk_p chunk_consume(chunk_p chunk);
void chunk_free(chunk_p chunk);

num_p num_create(uint64_t count, chunk_p head, chunk_p tail);
chunk_p num_insert(num_p num, uint64_t value);
chunk_p num_insert_head(num_p num, uint64_t value);
void num_remove_head(num_p num);
void num_insert_list(num_p num, chunk_p head, chunk_p tail, uint64_t cnt);
chunk_p num_denormalize(num_p num, chunk_p chunk);
bool num_normalize(num_p num);

num_p num_wrap_dec(char str[]);
num_p num_wrap_hex(char str[]);

num_p num_rebase(num_p num, uint64_t value);
num_p num_base(num_p num, uint64_t value);

bool num_sub_uint_offset(num_p num, chunk_p chunk, uint64_t value);

num_p num_shl_uint(num_p num, uint64_t bits);
num_p num_shr_uint(num_p num, uint64_t bits);
void num_add_uint(num_p num, uint64_t value);
void num_sub_uint(num_p num, uint64_t value);
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value);

#endif
