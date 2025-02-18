#ifndef __NUM_DEBUG_H__
#define __NUM_DEBUG_H__

#include "struct.h"

typedef __uint128_t uint128_t;

#ifdef DEBUG

#include <stdarg.h>

num_p num_create_immed(uint64_t n, ...);
num_p num_create_variadic(uint64_t n, va_list args);

bool uint64(uint64_t u1, uint64_t u2);
bool uint128_immed(uint128_t u1, uint64_t v2h, uint64_t v2l);

bool num_str(num_p num_1, num_p num_2);
bool num_immed(num_p num, uint64_t n, ...);

node_p num_get_node(num_p num, uint64_t count);

#endif

#define U128(V) ((uint128_t)(V))
#define U128_IMMED(V1, V2) ((U128(V1) << 64) | (V2))
#define MUL(V1, V2) U128(V1) * U128(V2)
#define LOW(V) ((uint64_t)(V))
#define HIGH(V) LOW((V) >> 64)

node_p node_create(uint64_t value, node_p next, node_p prev);
node_p node_consume(node_p node);
void node_free(node_p node);
node_p node_denormalize(node_p node);
node_p node_normalize(node_p node);

num_p num_create(node_p head, node_p tail);

void num_add_uint(num_p num, uint64_t value);
void num_sub_uint(num_p num, uint64_t value);
bool num_sub_uint_offset(num_p num, node_p node, uint64_t value);
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value);
num_p num_shl(num_p num);
num_p num_shr(num_p num);

#endif
