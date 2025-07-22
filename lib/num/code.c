#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbit.h>

#include "debug.h"
#include "fft.c"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/uint.h"
#include "../../mods/clu/header.h"



#ifdef DEBUG

num_p num_create_variadic(uint64_t n, va_list *args)
{
    num_p num = num_create(n, n);
    for(uint64_t i=0; i<n; i++)
        num->chunk[n-1-i] = va_arg(*args, uint64_t);

    return num;
}

num_p num_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return num_create_variadic(n, &args);
}



uint16_t rand_16()
{
    return rand();
}

uint32_t rand_32()
{
    return ((uint32_t)rand_16() << 16) | rand_16();
}

uint64_t rand_64()
{
    return (U64(rand_32()) << 32) | rand_32();
}

num_p num_create_rand(uint64_t count)
{
    num_p num = num_wrap(1);
    num = num_head_grow(num, count - 1);
    for(uint64_t i=0; i<count; i++)
        num->chunk[i] = rand_64();
    
    if(count)
        while(num->chunk[count - 1] == 0)
            num->chunk[count - 1] = rand();
    
    return num;
}



bool int64(int64_t i1, int64_t i2)
{
    if(i1 != i2)
    {
        printf("\n\n\tINT64 ASSERT ERROR\t| (" D64P() ") (" D64P() ")", i1, i2);
        return false;
    }

    return true;
}

bool uint64(uint64_t u1, uint64_t u2)
{
    if(u1 != u2)
    {
        printf("\n\n\tUINT64 ASSERT ERROR\t| (" U64PX ") (" U64PX ")", u1, u2);
        return false;
    }

    return true;
}

bool uint128_immed(uint128_t u1, uint64_t v2h, uint64_t v2l)
{
    if(!uint64(HIGH(u1), v2h))
    {
        printf("\n\tUINT128 ASSERT ERROR\t| HIGH");
        return false;
    }

    if(!uint64(LOW(u1), v2l))
    {
        printf("\n\tUINT128 ASSERT ERROR\t| LOW");
        return false;
    }

    return true;
}



bool num_inner(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    if(num_1->count > num_1->size)
    {
        printf("\n\n\tNUMBER ASSERT ERROR\t| COUNT BIGGER THAN SIZE | " U64P() " " U64P() "", num_1->count, num_1->size);
        return false;
    }

    if(num_1->chunk == NULL)
    {
        printf("\n\n\tNUMBER ASSERT ERROR\t| CHUNK IS NULL");
        return false;
    }

    if(!uint64(num_1->count, num_2->count))
    {
        printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN LENGTH");
        return false;
    }

    for(uint64_t i=0; i<num_1->count; i++)
    {
        if(!uint64(num_1->chunk[i], num_2->chunk[i]))
        {
            printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN VALUE | " U64P() "", i);
            return false;
        }
    }

    return true;
}

bool num_eq_dbg(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    if(!num_inner(num_1, num_2))
    {
        printf("\n");
        num_display_full("\tnum_1", num_1);
        num_display_full("\tnum_2", num_2);
        return false;
    }

    num_free(num_1);
    num_free(num_2);
    return true;
}

bool num_immed(num_p num, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    num_p num_2 = num_create_variadic(n, &args);
    return num_eq_dbg(num, num_2);
}

#endif



uint64_t uint_from_char(char c)
{
    switch (c)
    {
        case '0' ... '9': return c - '0';
        case 'a' ... 'f': return c - 'a' + 10;
        case 'A' ... 'F': return c - 'A' + 10;
    }
    exit(EXIT_FAILURE);
}

uint64_t uint_from_str(char str[], uint64_t size, uint64_t base) // TODO test
{
    uint64_t value = 0;
    for(uint64_t i=0; i<size; i++)
    {
        uint64_t aux = uint_from_char(str[i]);
        assert(aux < base);
        value = value * base + aux;
    }

    return value;
}

uint64_t uint_read(FILE *fp, uint64_t size, uint64_t base)
{
    char str[size];
    for(uint64_t i=0; i<size; i++)
        str[i] = fgetc(fp);

    return uint_from_str(str, size, base);
}

uint64_t uint_inv(uint64_t value, uint64_t q) // TODO TEST
{
    uint64_t res = 0;
    for(uint64_t i=0; i<q; i++)
    {
        res <<= 1;
        res |= value & 1;
        value >>= 1;
    }
    return res;
}



void num_display_dec(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
    {
        printf("0");
        return;
    }

    num = num_base_to(num_copy(num), 1000000000000000000);
    printf(U64P(), num->chunk[num->count-1]);
    for(uint64_t i=num->count-2; i!=UINT64_MAX; i--)
        printf(U64P(018), num->chunk[i]);

    num_free(num);
}

void num_display_opts(num_p num, char *tag, bool length, bool full)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(tag)
        printf("\n%s\t: ", tag);

    if(length)
    {
        if(num->count == 0)
        {
            printf("(    0) | ");
        }
        else
        {
            printf("(" U64P(5) ") | ", num->count);
        }
    }

    if(num->count == 0)
    {
        printf("0");
        return;
    }

    uint64_t max = full ?
        num->count :
        num->count > 4 ?
            4 : num->count;

    for(uint64_t i=0; i<max; i++)
        printf("" U64PX " ", num->chunk[num->count-1-i]);

    if(!full && num->count > 4)
        printf("...");
}

void num_display(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_display_opts(num, NULL, true, false);
}

void num_display_tag(char *tag, num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_display_opts(num, tag, true, false);
}

void num_display_full(char *tag, num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_display_opts(num, tag, true, true);
}



num_p num_create(uint64_t size, uint64_t count)
{
    assert(size >= count);
    size = size ? size : 1;
    num_p num = calloc(1, sizeof(num_t) + size * sizeof(uint64_t));
    assert(num);

    *num = (num_t)
    {
        .size = size,
        .count = count,
        .chunk = (chunk_p)&num[1]
    };
    return num;
}

// num_p num_create_delete(uint64_t size, uint64_t count, char const func[], uint64_t line)
// {
//     num_p num = num_create(size, count);
//     clu_handler_unregister(num, __func__, __LINE__);
//     clu_handler_register(num, "f|%s|l|%d", func, line);
//     return num;
// }

// #define num_create(size, count) num_create_delete(size, count, __func__, __LINE__)

num_p num_expand_to(num_p num, uint64_t target)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(target <= num->size)
        return num;

    assert(!num->cannot_expand);

    uint64_t size = target * 2;
    num = realloc(num, sizeof(num_t) + size * sizeof(uint64_t));
    assert(num);
    uint64_t size_old = num->size;
    *num = (num_t)
    {
        .size = size,
        .count = num->count,
        .chunk = (chunk_p)&num[1]
    };

    memset(&num->chunk[size_old], 0, (size - size_old) * sizeof(uint64_t));
    return num;
}

num_p num_expand(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    return num_expand_to(num, num->size + 1);
}

num_p num_reset(num_p num) // TODO TEST
{
    memset(num->chunk, 0, num->count * sizeof(uint64_t));
    num->count = 0;
    return num;
}

uint64_t num_chunk_get(num_p num, uint64_t pos)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    return pos < num->count ? num->chunk[pos] : 0;
}

num_p num_chunk_set(num_p num, uint64_t pos, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(pos >= num->count && value == 0)
        return num;

    if(pos >= num->size)
        num = num_expand_to(num, pos + 1);

    num->chunk[pos] = value;
    if(pos >= num->count)
        num->count = pos + 1;

    return num;
}

// returns true if NUM had to be corrected
bool num_normalize(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return false;

    if(num->chunk[num->count-1] != 0)
        return false;

    num->count--;
    return true;
}

num_p num_head_grow(num_p num, uint64_t count) // TODO test
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return num;

    if(count == 0)
        return num;

    uint64_t count_res = num->count + count;
    num = num_expand_to(num, count_res);
    for(uint64_t i=num->count-1; i!=UINT64_MAX; i--)
        num->chunk[i+count] = num->chunk[i];
    

    memset(num->chunk, 0, count * sizeof(uint64_t));
    num->count = count_res;
    return num;
}

num_p num_head_trim(num_p num, uint64_t count) // TODO test
{
    if(num->count == 0 || count == 0)
        return num;

    if(count >= num->count)
        return num_reset(num);

    uint64_t count_res = num->count - count;
    for(uint64_t i=count; i<num->count; i++)
        num->chunk[i-count] = num->chunk[i];

    memset(&num->chunk[num->count - count], 0, count * sizeof(uint64_t));
    num->count = count_res;
    return num;
}

void num_break(num_p *out_num_hi, num_p *out_num_lo, num_p num, uint64_t count)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(count == 0)
    {
        *out_num_hi = num;
        *out_num_lo = num_create(0, 0);
        return;
    }

    if(num->count <= count)
    {
        *out_num_hi = num_create(0, 0);
        *out_num_lo = num;
        return;
    }

    uint64_t size = num->count - count;
    num_p num_hi = num_create(size, size);
    memcpy(num_hi->chunk, &num->chunk[count], size * sizeof(uint64_t));

    memset(&num->chunk[count], 0, (num->size - count) * sizeof(uint64_t));
    num->count = count;
    while(num_normalize(num));

    *out_num_hi = num_hi;
    *out_num_lo = num;
}

num_p num_join(num_p num_hi, num_p num_lo, uint64_t count) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_hi);
    CLU_HANDLER_IS_SAFE(num_lo);
    assert(num_hi);
    assert(num_lo);

    if(num_hi->count == 0)
    {
        num_free(num_hi);
        return num_lo;
    }

    if(num_lo->count == 0 && count == 0)
    {
        num_free(num_lo);
        return num_hi;
    }

    count = count > num_lo->count ? count : num_lo->count;
    uint64_t count_res = count + num_hi->count;
    num_lo = num_expand_to(num_lo, count_res);
    memcpy(&num_lo->chunk[count], num_hi->chunk, num_hi->count * sizeof(uint64_t));
    num_lo->count = count_res;

    num_free(num_hi);
    return num_lo;
}

num_t num_span(num_p num, uint64_t pos_init, uint64_t pos_max) // TODO TEST
{
    pos_max = pos_max > num->count ? num->count : pos_max;
    pos_max = pos_max > pos_init   ? pos_max    : pos_init;
    uint64_t size = pos_max - pos_init;

    num_t num_res = (num_t)
    {
        .size = size,
        .count = size,
        .chunk = &num->chunk[pos_init],
        .cannot_expand = true
    };

    CLU_HANDLER_REGISTER_STATIC(&num_res);
    while(num_normalize(&num_res));
    return num_res;
}



num_p num_wrap(uint64_t value)
{
    if(value == 0)
        return num_create(0, 0);

    num_p num = num_create(1, 1);
    num->chunk[0] = value;
    return num;
}

num_p num_wrap_dec(char str[])
{
    uint64_t len = strlen(str);
    uint64_t pos = len / 18;
    uint64_t extra = len %18 != 0;
    num_p num = num_create(pos + extra, 0);

    if(extra)
    {
        uint64_t value = uint_from_str(str, len % 18, 10);
        num = num_chunk_set(num, pos, value);
    }

    for(uint64_t i=len % 18; i<len; i+=18)
    {
        uint64_t value = uint_from_str(&str[i], 18, 10);
        pos--;
        num = num_chunk_set(num, pos, value);
    }

    while(num_normalize(num));
    return num_base_from(num, 1000000000000000000);
}

num_p num_wrap_hex(char str[])
{
    uint64_t len = strlen(str);
    assert(len > 1 && str[0] == '0' && str[1] == 'x');

    uint64_t pos = (len - 2) / 16;
    uint64_t extra = (len - 2) % 16 != 0;
    num_p num = num_create(pos + extra, 0);

    if(extra)
    {
        uint64_t value = uint_from_str(&str[2], (len - 2) % 16, 16);
        num = num_chunk_set(num, pos, value);
    }

    for(uint64_t i = 2 + (len - 2) % 16; i < len; i += 16)
    {
        uint64_t value = uint_from_str(&str[i], 16, 16);
        pos--;
        num = num_chunk_set(num, pos, value);
    }

    while(num_normalize(num));
    return num;
}

num_p num_wrap_str(char str[])
{
    return str[0] == '0' && str[1] == 'x' ?
        num_wrap_hex(str) : num_wrap_dec(str);
}

uint64_t get_ftell(FILE* fp)
{
    int64_t res = ftell(fp);
    assert(res >= 0);
    return res;
}

num_p num_read_dec(char file_name[])
{
    FILE *fp = fopen(file_name, "r");
    assert(fp);

    assert(!fseek(fp, 0, SEEK_END));
    uint64_t size = get_ftell(fp);
    assert(!fseek(fp, 0, SEEK_SET));

    uint64_t pos = size / 18;
    uint64_t extra = size % 18;

    num_p num = num_create(pos + extra, 0);
    if(extra)
    {
        uint64_t value = uint_read(fp, size % 18, 10);
        num = num_chunk_set(num, pos, value);
    }

    while((uint64_t)get_ftell(fp) < size)
    {
        uint64_t value = uint_read(fp, 18, 10);
        pos--;
        num = num_chunk_set(num, pos, value);
    }
    fclose(fp);

    while(num_normalize(num));
    return num_base_from(num, 1000000000000000000);
}

uint64_t num_unwrap(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    assert(num->count < 2);
    uint64_t value = num->count ? num->chunk[0] : 0;
    num_free(num);

    return value;
}

num_p num_copy(num_p num) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_p num_res = num_create(num->count, num->count);
    memcpy(num_res->chunk, num->chunk, num->count * sizeof(uint64_t));

    return num_res;
}

void num_free(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    free(num);
}



num_p num_add_uint_offset(num_p num, uint64_t pos, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    for(; value; pos++)
    {
        uint64_t cur = num_chunk_get(num, pos);
        cur += value;
        num = num_chunk_set(num, pos, cur);
        value = cur < value;
    }

    return num;
}

num_p num_sub_uint_offset(num_p num, uint64_t pos, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    for(; value; pos++)
    {
        if(pos >= num->count)
        {
            int *p = NULL;
            *p = 1;
        }
        assert(pos < num->count);

        uint64_t next = num->chunk[pos] < value;
        num->chunk[pos] -= value;
        value = next;
    }

    while(num_normalize(num));
    return num;
}

/* keeps NUM */
num_p num_add_mul_uint_offset(
    num_p num_res,
    uint64_t pos_res,
    num_p num,
    uint64_t pos,
    uint64_t value
)
{
    CLU_HANDLER_IS_SAFE(num_res);
    CLU_HANDLER_IS_SAFE(num);
    assert(num_res);
    assert(num);

    if(value == 0)
        return num_res;

    for(uint64_t i=pos; i<num->count; i++)
    {
        uint128_t u = MUL(num->chunk[i], value);
        num_res = num_add_uint_offset(num_res, pos_res + i - pos, LOW(u));
        num_res = num_add_uint_offset(num_res, pos_res + i - pos + 1, HIGH(u));
    }

    return num_res;
}

num_p num_shl_inner(num_p num, uint64_t bits) // TODO test
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
    assert(bits < 64); // TODO test

    if(bits == 0)
        return num;

    uint64_t carry = 0;
    for(uint64_t i=0; i<num->count; i++)
    {
        uint64_t value = num->chunk[i];
        num->chunk[i] = (value << bits) | carry;
        carry = value >> (64 - bits);
    }

    if(carry)
        num = num_chunk_set(num, num->count, carry);

    return num;
}

num_p num_shr_inner(num_p num, uint64_t bits) // TODO test
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
    assert(bits < 64); // TODO test

    if(bits == 0)
        return num;

    uint64_t carry = 0;
    for(uint64_t i=num->count-1; i!=UINT64_MAX; i--)
    {
        uint64_t value = num->chunk[i];
        num->chunk[i] = (value >> bits) | carry;
        carry = value << (64 - bits);
    }
    num_normalize(num);

    return num;
}



int64_t num_cmp_offset(num_p num_1, uint64_t pos_1, num_p num_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    if(num_1->count == 0)
    {
        if(num_2->count == 0)
            return 0;

        if(num_2->count != 0)
            return -1;
    }

    if(num_2->count == 0)
        return 1;

    if(num_1->count > num_2->count + pos_1)
        return 1;

    if(num_1->count < num_2->count + pos_1)
        return -1;

    for(uint64_t i = num_2->count-1; i != UINT64_MAX; i--)
    {
        uint64_t value_1 = num_1->chunk[pos_1 + i];
        uint64_t value_2 = num_2->chunk[i];

        if(value_1 > value_2)
            return 1;

        if(value_1 < value_2)
            return -1;
    }

    return 0;
}

/* keeps NUM_2 */
num_p num_add_offset(num_p num_1, uint64_t pos_1, num_p num_2, uint64_t pos_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    for(uint64_t i=pos_2; i<num_2->count; i++)
        num_1 = num_add_uint_offset(num_1, pos_1 + i - pos_2, num_2->chunk[i]);

    return num_1;
}

/* keeps NUM2 */
num_p num_sub_offset(num_p num_1, uint64_t pos_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    for(uint64_t i = num_2->count-1; i!=UINT64_MAX; i--)
        num_1 = num_sub_uint_offset(num_1, pos_1 + i, num_2->chunk[i]);

    return num_1;
}



/* preserves NUM */
num_p num_mul_uint_inner(num_p num_res, num_p num, uint64_t value) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    if(value == 0 || num->count == 0)
        return num_reset(num_res);

    num_res->count = num->count + 1;
    num_res->chunk[0] = 0;
    for(uint64_t i=0; i<num->count; i++)
    {
        uint128_t u = MUL(num->chunk[i], value);
        num_res->chunk[i+1] = HIGH(u);
        num_res = num_add_uint_offset(num_res, i, LOW(u));
    }
    num_normalize(num_res);
    return num_res;
}

// Keeps NUM_1
// Keeps NUM_2
// NUM_RES should be reseted
num_p num_mul_high_inner(num_p num_res, num_p num_1, num_p num_2, uint64_t pos) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    if(pos >= num_1->count + num_2->count)
        return num_res;

    uint64_t max = pos > num_2->count ? num_2->count : pos;
    for(uint64_t i=0; i<max; i++)
        num_res = num_add_mul_uint_offset(num_res, 0, num_1, pos-i, num_2->chunk[i]);

    for(uint64_t i=pos; i<num_2->count; i++)
        num_res = num_add_mul_uint_offset(num_res, i-pos, num_1, 0, num_2->chunk[i]);

    return num_res;
}

// Keeps NUM_1
// Keeps NUM_2
num_p num_mul_classic_inner(num_p num_res, num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    return num_mul_high_inner(num_res, num_1, num_2, 0);
}

num_p num_sqr_classic(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return num;

    num_p num_res = num_create(2 * num->count + 1, 0);
    for(uint64_t i=0; i<num->count; i++)
    {
        uint64_t value = num->chunk[i];

        uint128_t u = MUL(value, value);
        num_res = num_add_uint_offset(num_res, 2 * i, LOW(u));
        num_res = num_add_uint_offset(num_res, 2 * i + 1, HIGH(u));

        num_res = num_add_mul_uint_offset(num_res, 2 * i + 1, num, i + 1, value << 1);

        if(value >= 0x8000000000000000)
            num_res = num_add_offset(num_res, 2 * i + 2, num, i + 1);
    }

    num_free(num);
    return num_res;
}




num_p num_pad(num_p num) // TODO TEST
{
    uint64_t count = num->count;
    for(uint64_t i=count-1; i!=UINT64_MAX; i--)
    {
        uint64_t value = num->chunk[i];
        for(uint64_t j=0; j<4; j++)
        {
            num = num_chunk_set(num, 4 * i + j, value & 0xffff);
            value >>= 16;
        }
    }
    return num;
}

num_p num_depad(num_p num) // TODO TEST
{
    uint64_t count = num->count;
    for(uint64_t i=1; i<count; i++)
    {
        uint64_t value = num->chunk[i];
        num->chunk[i] = 0;

        uint128_t tmp = U128(value) << (16 * (i%4));
        uint64_t value_1 = LOW(tmp);
        uint64_t value_2 = HIGH(tmp);

        num = num_add_uint_offset(num, i/4, value_1);
        num = num_add_uint_offset(num, i/4 + 1, value_2);
    }

    num->count = (num->count + 3) / 4;
    while(num_normalize(num));
    return num;
}

num_p num_shuffle(num_p num, uint64_t n) // TODO TEST
{
    num = num_expand_to(num, n);
    num->count = n;
    
    uint64_t q = stdc_trailing_zeros(n);
    for(uint64_t i=0; i<n; i++)
    {
        uint64_t j = uint_inv(i, q);
        if(j < i)
            continue;

        uint64_t value = num->chunk[i];
        num->chunk[i] = num->chunk[j];
        num->chunk[j] = value;
    }
    return num;
}

void fft_rec(uint64_t vec[], uint64_t n, uint64_t w)
{
    if(n > 2)
    {
        uint64_t w_next = uint_mod_mul(w, w, PRIME_1);
        fft_rec( vec     , n/2, w_next);
        fft_rec(&vec[n/2], n/2, w_next);
    }

    uint64_t wi = 1;
    for(uint64_t i=0; i<n/2; i++)
    {
        uint64_t i_a = i; 
        uint64_t i_b = i + n/2;

        uint64_t a = vec[i_a];
        uint64_t b = uint_mod_mul(wi, vec[i_b], PRIME_1);

        vec[i_a] = uint_mod_add(a, b, PRIME_1);
        vec[i_b] = uint_mod_sub(a, b, PRIME_1);

        wi = uint_mod_mul(wi, w, PRIME_1);
    }
}

num_p num_fft(num_p num, uint64_t n) // TODO TEST
{
    num = num_pad(num);
    num = num_shuffle(num, n);

    uint64_t q = (1UL << 57) / n;
    uint64_t w = uint_mod_pow(ROOT_W_1, q, PRIME_1);
    fft_rec(num->chunk, n, w);

    return num;
}

num_p num_fft_inv(num_p num, uint64_t n) // TODO TEST
{
    if(n == 1)
        return num;

    num = num_expand_to(num, n);

    uint64_t q = (1UL << 57) / n;
    uint64_t w = uint_mod_pow(ROOT_W_INV_1, q, PRIME_1);
    fft_rec(num->chunk, n, w);

    uint64_t n_inv = uint_mod_div(1, n, PRIME_1);
    for(uint64_t i=0; i<n; i++)
        num->chunk[i] = uint_mod_mul(num->chunk[i], n_inv, PRIME_1);

    return num;
}

num_p num_convolute(num_p num_res, num_p num_1, num_p num_2)
{
    uint64_t n = num_1->count;

    assert(num_res->size >= n);
    num_res->count = n;
    for(uint64_t i=0; i<n; i++)
        num_res->chunk[i] = uint_mod_mul(num_1->chunk[i], num_2->chunk[i], PRIME_1);

    num_res = num_shuffle(num_res, n);
    num_res = num_fft_inv(num_res, n);
    num_res = num_depad(num_res);
    return num_res;
}

num_p num_mul_fft_inner(num_p num_res, num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    uint64_t n = stdc_bit_ceil(4 * (num_1->count + num_2->count));
    num_1 = num_fft(num_1, n);
    num_2 = num_fft(num_2, n);

    num_res = num_convolute(num_res, num_1, num_2);
    num_free(num_1);
    num_free(num_2);
    return num_res;
}

num_p num_sqr_fft(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    uint64_t n = stdc_bit_ceil(8 * num->count);
    num = num_fft(num, n);
    return num_convolute(num, num, num);
}



void num_display_span(num_p num, uint64_t pos, uint64_t count)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)
    assert(num->count >= pos + count);

    for(uint64_t i=count-1; i!=UINT64_MAX; i--)
        printf("" U64PX " ", num->chunk[pos + i]);
}

void num_display_span_tag(char tag[], num_p num, uint64_t pos, uint64_t count)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    printf("\n%s\t: ", tag);
    num_display_span(num, pos, count);
}

// Separate number to a base 2^(64*b)
// Each place will be represented in n chunks
// the final vector is padded to k places
num_p num_ssm_pad(num_p num, uint64_t b, uint64_t n, uint64_t k)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    assert(num->count <= b * k);
    uint64_t count = k * n;
    num = num_expand_to(num, count);
    num->count = count;
    for(uint64_t i = k-1; i!=UINT64_MAX; i--) // TODO use a better starter
    {
        for(uint64_t j=n-1; j!=b-1; j--)
            num->chunk[i*n + j] = 0;

        for(uint64_t j=b-1; j!=UINT64_MAX; j--)
            num->chunk[i*n + j] = num->chunk[i*b + j];
    }
    return num;
}

bool num_is_span_zero(num_p num, uint64_t pos, uint64_t count)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    for(uint64_t i=0; i<count; i++)
        if(num->chunk[i + pos])
            return false;

    return true;
}

// normalizes coeficient if it is less than 2 modulus
void num_ssm_normalize(num_p num, uint64_t pos, uint64_t n)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)
    assert(num->chunk[pos + n - 1] <= 2)

    if(
        num->chunk[pos + n - 1] == 2 ||
        (num->chunk[pos + n - 1] && !num_is_span_zero(num, pos, n - 1))
    )
    {
        uint64_t num_count = num->count;
        num_sub_uint_offset(num, pos        , 1);
        num_sub_uint_offset(num, pos + n - 1, 1);
        num->count = num_count;
    }
}

int64_t num_ssm_cmp(
    num_p num_1,
    uint64_t pos_1,
    num_p num_2,
    uint64_t pos_2,
    uint64_t n
)
{
    for(uint64_t i=n-1; i!=UINT64_MAX; i--)
    {
        if(num_1->chunk[pos_1 + i] < num_2->chunk[pos_2 + i])
            return -1;
            
        if(num_1->chunk[pos_1 + i] > num_2->chunk[pos_2 + i])
            return 1;
    }
    return 0;
}

void num_ssm_add_mod(
    num_p num_res,
    uint64_t pos_res,
    num_p num,
    uint64_t pos_1,
    uint64_t pos_2,
    uint64_t n
)
{
    CLU_HANDLER_IS_SAFE(num_res)
    CLU_HANDLER_IS_SAFE(num)
    assert(num_res)
    assert(num)

    uint128_t carry = 0;
    for(uint64_t i=0; i<n; i++)
    {
        carry += (uint128_t)num->chunk[pos_1 + i] + num->chunk[pos_2 + i];
        num_res->chunk[pos_res + i] = LOW(carry);
        carry >>= 64;
    }

    num_ssm_normalize(num_res, pos_res, n);
}

void num_ssm_sub_mod(
    num_p num_res,
    uint64_t pos_res,
    num_p num_1,
    uint64_t pos_1,
    num_p num_2,
    uint64_t pos_2,
    uint64_t n
)
{
    CLU_HANDLER_IS_SAFE(num_res)
    CLU_HANDLER_IS_SAFE(num_1)
    CLU_HANDLER_IS_SAFE(num_2)
    assert(num_res)
    assert(num_1)
    assert(num_2)

    if(num_ssm_cmp(num_1, pos_1, num_2, pos_2, n) < 0)
    {
        num_add_uint_offset(num_1, pos_1        , 1);
        num_add_uint_offset(num_1, pos_1 + n - 1, 1);
    }

    uint128_t carry = 0;
    for(uint64_t i=0; i<n; i++)
    {
        carry += (uint128_t)num_1->chunk[pos_1 + i] - num_2->chunk[pos_2 + i];
        num_res->chunk[pos_res + i] = LOW(carry);
        carry = (int128_t)carry >> 64;
    }

    num_ssm_normalize(num_1, pos_1, n);
}

// operation can be done in place if num_res is the same as num and pos_res is pos
void num_ssm_shl(
    num_p num_res,
    uint64_t pos_res,
    num_p num,
    uint64_t pos,
    uint64_t n,
    uint64_t bits
)
{
    uint64_t count = bits >> 6;
    bits &=0x3f;
    if((bits) == 0)
    {
        for(uint64_t i=n-1; i!=count-1; i--)
            num_res->chunk[pos_res + i] = num->chunk[pos + i - count];

        memset(&num_res->chunk[pos_res], 0, count * sizeof(uint64_t));
        return;
    }

    for(uint64_t i=n-1; i!=count; i--)
    {
        uint64_t value_1 = num->chunk[pos + i - count];
        uint64_t value_0 = num->chunk[pos + i - count - 1];
        num_res->chunk[pos_res + i] = (value_1 << bits) | (value_0 >> (64 - bits));
    }
    num_res->chunk[pos_res + count] = num->chunk[pos] << bits;

    memset(&num_res->chunk[pos_res], 0, count * sizeof(uint64_t));
}

// operation can be done in place if num_res is the same as num and pos_res is pos
void num_ssm_shr(
    num_p num_res,
    uint64_t pos_res,
    num_p num,
    uint64_t pos,
    uint64_t n,
    uint64_t bits
)
{
    uint64_t count = bits >> 6;
    bits &=0x3f;
    if(bits == 0)
    {
        for(uint64_t i=0; i<n-count; i++)
            num_res->chunk[pos_res + i] = num->chunk[pos + i + count];

        memset(&num_res->chunk[pos_res + n - count], 0, count * sizeof(uint64_t));
        return;
    }

    for(uint64_t i=0; i!=n-count-1; i++)
    {
        uint64_t value_0 = num->chunk[pos + i + count];
        uint64_t value_1 = num->chunk[pos + i + count + 1];
        num_res->chunk[pos_res + i] = (value_0 >> bits) | (value_1 << (64 - bits));
    }
    num_res->chunk[pos_res + n - count - 1] = num->chunk[pos + n - 1] >> bits;
    memset(&num_res->chunk[pos_res + n - count], 0, count * sizeof(uint64_t));
}

// Shifts a number by BITS and returns the module
// Index is the begining of the chunk span
// n is the size of the number in chunks, the module is 2^(64*(n-1)) + 1
void num_ssm_shl_mod(
    num_p num_aux,
    num_p num,
    uint64_t pos,
    uint64_t n,
    uint64_t bits
)
{
    CLU_HANDLER_IS_SAFE(num_aux)
    CLU_HANDLER_IS_SAFE(num)
    assert(num_aux)
    assert(num)

    if(bits == 0 || num_is_span_zero(num, pos, n))
        return;

    num_ssm_shr(num_aux, 0, num, pos, n, 64 * n - 64 - bits);
    num_ssm_shl(num, pos, num, pos, n, bits);
    num->chunk[pos + n - 1] = 0;
    num_ssm_sub_mod(num, pos, num, pos, num_aux, 0, n);
}

void num_ssm_shr_mod(
    num_p num_aux,
    num_p num,
    uint64_t pos,
    uint64_t n,
    uint64_t bits
)
{
    CLU_HANDLER_IS_SAFE(num_aux)
    CLU_HANDLER_IS_SAFE(num)
    assert(num_aux)
    assert(num)

    if(bits == 0 || num_is_span_zero(num, pos, n))
        return;

    num_ssm_shl(num_aux, 0, num, pos, n, 64 * n - 64 - bits);
    num_ssm_shr(num, pos, num, pos, n, bits);
    num_aux->chunk[n - 1] = 0;
    num_ssm_sub_mod(num, pos, num, pos, num_aux, 0, n);
}

void num_ssm_fft_fwd_rec(
    num_p num_aux,
    num_p num,
    uint64_t pos,
    uint64_t step,
    uint64_t n,
    uint64_t k,
    uint64_t bits
)
{
    CLU_HANDLER_IS_SAFE(num_aux)
    CLU_HANDLER_IS_SAFE(num)
    assert(num_aux)
    assert(num)

    if(k > 2)
    {
        num_ssm_fft_fwd_rec(num_aux, num, pos     , 2*step, n, k/2, 2*bits);
        num_ssm_fft_fwd_rec(num_aux, num, pos+step, 2*step, n, k/2, 2*bits);
    }

    for(uint64_t i=0; i<k/2; i++)
    {
        uint64_t pos_1 = (pos + step * (2 * i)) * n;
        uint64_t pos_2 = (pos + step * (2 * i + 1)) * n;

        num_ssm_shl_mod(num_aux, num, pos_2, n, i * bits);
    
        num_ssm_add_mod(num_aux, 0, num, pos_1, pos_2, n);
        num_ssm_sub_mod(num_aux, n, num, pos_1, num, pos_2, n);

        memcpy(&num->chunk[pos_1],  num_aux->chunk   , n * sizeof(uint64_t));
        memcpy(&num->chunk[pos_2], &num_aux->chunk[n], n * sizeof(uint64_t));
    }
}

void ssm_calculate_parametrs(uint64_t res[], uint64_t count)
{
    uint64_t M = 1 << (stdc_bit_width(count) / 2);
    uint64_t K = stdc_bit_ceil(2 * count / M);
    uint64_t P = 2 * M + 1;
    uint64_t Q = 16 * P;
    uint64_t n = P + 1;

    res[0] = M;
    res[1] = K;
    res[2] = Q;
    res[3] = n;
}

void num_ssm_fft_fwd(
    num_p num_aux,
    num_p num,
    uint64_t n,
    uint64_t k,
    uint64_t bits
)
{
    CLU_HANDLER_IS_SAFE(num_aux)
    CLU_HANDLER_IS_SAFE(num)
    assert(num_aux)
    assert(num)

    for(uint64_t i=0; i<k; i++)
        num_ssm_shl_mod(num_aux, num, n * i, n, bits * i);

    num_ssm_fft_fwd_rec(num_aux, num, 0, 1, n, k, 2 * bits);
}

void num_ssm_prepare(num_p num_aux, num_p num)
{
    uint64_t M = 1 << (stdc_bit_width(num->count) / 2);
    uint64_t K = stdc_bit_ceil(2 * num->count / M);
    uint64_t P = 2 * M + 1;
    uint64_t Q = 16 * P;
    uint64_t n = P + 1;
    
    num = num_ssm_pad(num, M, n, K);

    for(uint64_t i=0; ; i++)
    {
        num_ssm_fft_fwd(num_aux, num, n, K, Q);

        printf("\n");
        // printf("\nres:");
        for(uint64_t i=0; i<K; i++)
        {
            printf("\ncoeficient[%lu]: ", i);
            num_display_span(num, n * i, n);
        }

        getchar();
    }
}

void del()
{
    num_p num = num_create_immed(4, 3, 5, 9, 4);

    uint64_t params[4];
    ssm_calculate_parametrs(params, num->count);
    num_p num_aux = num_create(2 * params[3], 2 * params[3]);

    num_ssm_prepare(num_aux, num);
}



// Keeps NUM_1
// Keeps NUM_2
num_p num_mul_inner(num_p num_res, num_p num_1, num_p num_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1)
    CLU_HANDLER_IS_SAFE(num_2)
    assert(num_1)
    assert(num_2)

    num_reset(num_res);
    if(num_1->count == 0 || num_2->count == 0)
        return num_res;

    if(num_1->count < 1500 || num_2->count < 1500)
        return num_mul_classic_inner(num_res, num_1, num_2);

    return num_mul_fft_inner(num_res, num_copy(num_1), num_copy(num_2));
}

STRUCT(num_fft_cache)
{
    bool memoized;
    num_p num;

    uint64_t n;
    num_p fft;
};

// Keeps NUM_1
// Keeps NUM_2
num_p num_mul_memoized(num_p num_res, num_p num_1, num_fft_cache_p num_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1)
    assert(num_1)
    assert(num_2)

    num_reset(num_res);
    if(num_1->count == 0 || num_2->num->count == 0)
        return num_res;

    if(num_1->count < 1500 || num_2->num->count < 1500)
        return num_mul_classic_inner(num_res, num_1, num_2->num);

    uint64_t n = stdc_bit_ceil(4 * (num_1->count + num_2->num->count));
    assert(num_2->n >= n);
    num_1 = num_fft(num_copy(num_1), num_2->n);
    
    if(num_2->memoized == false)
    {
        num_2->memoized = true;
        num_2->fft = num_fft(num_copy(num_2->num), num_2->n);
    }

    num_res = num_convolute(num_res, num_1, num_2->fft);
    num_free(num_1);
    return num_res;
}

num_p num_mul_classic(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1)
    CLU_HANDLER_IS_SAFE(num_2)
    assert(num_1)
    assert(num_2)

    num_p num_res = num_create(num_1->count + num_2->count + 1, 0);
    num_res->cannot_expand = true;
    num_mul_classic_inner(num_res, num_1, num_2);
    num_free(num_1);
    num_free(num_2);
    return num_res;    
}

num_p num_mul_fft(num_p num_1, num_p num_2)
{

    CLU_HANDLER_IS_SAFE(num_1)
    CLU_HANDLER_IS_SAFE(num_2)
    assert(num_1)
    assert(num_2)
    
    uint64_t count = stdc_bit_ceil(4 * (num_1->count + num_2->count));
    num_p num_res = num_create(count, 0);
    num_res->cannot_expand = true;
    return num_mul_fft_inner(num_res, num_1, num_2);    
}



// Returns quocient
// NUM becomes remainder
num_p num_div_mod_uint(num_p num, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
    assert(value);

    num_p num_q = num_create(num->count, num->count);
    for(uint64_t i = num->count - 1; i != UINT64_MAX; i--)
    {
        if((num->count < i) || ((num->count - 1 == i) && (num->chunk[i] < value)))
        {
            num_q->chunk[i] = 0;
            continue;
        }

        if(num->count - 1 == i)
        {
            uint64_t r = num->chunk[i] / value;
            num_q->chunk[i] = r;
            num = num_sub_uint_offset(num, i, r * value);
            continue;
        }

        uint128_t value_1 = U128_IMMED(num->chunk[i + 1], num->chunk[i]);
        uint64_t r = value_1 / value;
        value_1 = MUL(r, value);
        num = num_sub_uint_offset(num, i+1, HIGH(value_1));
        num = num_sub_uint_offset(num, i  , LOW(value_1));
        num_q->chunk[i] = r;
    }
    num_normalize(num_q);
    return num_q;
}

// Input expected to be normalized
// Returns quocient
// NUM_1 becomes remainder
// Keeps NUM_2
num_p num_div_mod_classic(num_p num_aux, num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    uint64_t count = num_1->count - num_2->count + 1;
    num_p num_q = num_create(count, count);
    uint64_t val_2 = num_2->chunk[num_2->count-1];
    for(uint64_t i = count - 1; i != UINT64_MAX; i--)
    {
        if(num_cmp_offset(num_1, i, num_2) < 0)
        {
            num_q->chunk[i] = 0;
            continue;
        }

        if(num_1->count == num_2->count + i)
        {
            num_q->chunk[i] = 1;
            num_1 = num_sub_offset(num_1, i, num_2);
            continue;
        }

        if(num_1->chunk[num_1->count-1] == val_2)
        {
            num_q->chunk[i] = UINT64_MAX;
            num_aux = num_mul_uint_inner(num_aux, num_2, UINT64_MAX);
            num_1 = num_sub_offset(num_1, i, num_aux);
            continue;
        }

        uint128_t val_1 = U128_IMMED(num_1->chunk[num_1->count-1], num_1->chunk[num_1->count-2]);
        uint64_t r = val_1 / val_2;
        num_aux = num_mul_uint_inner(num_aux, num_2, r);
        while(num_cmp_offset(num_1, i, num_aux) < 0)
        {
            r--;
            num_aux = num_sub_offset(num_aux, 0, num_2);
        }

        num_q->chunk[i] = r;
        num_1 = num_sub_offset(num_1, i, num_aux);
    }

    num_normalize(num_q);
    return num_q;
}

// Input expected to be normalized
// Returns quocient
// NUM_1 becomes remainder
// Keeps NUM_2
num_p num_div_mod_fallback(num_p num_aux, num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);
    assert(num_2->count);

    if(num_cmp(num_1, num_2) < 0)
        return num_create(0, 0);

    if(num_2->count == 1)
        return num_div_mod_uint(num_1, num_2->chunk[0]);
    
    return num_div_mod_classic(num_aux, num_1, num_2);
}

STRUCT(bz_frame)
{
    bool memoized;
    num_t num_2_1, num_2_0;
    num_fft_cache_t num_2_0_cache;
};

// Input expected to be normalized
// Returns quocient
// NUM_1 becomes remainder
// Keeps NUM_2
num_p num_div_mod_bz_rec(num_p num_aux, num_p num_1, num_p num_2, bz_frame_t f[])
{

    CLU_HANDLER_IS_SAFE(num_1)
    CLU_HANDLER_IS_SAFE(num_2)
    assert(num_1)
    assert(num_2)
    
    if(num_1->count < num_2->count + 2 || num_2->count == 1)
        return num_div_mod_fallback(num_aux, num_1, num_2);

    uint64_t k = num_2->count / 2;
    if(f->memoized == false)
    {
        f->memoized = true;
    
        f->num_2_0 = num_span(num_2, 0, k);
        f->num_2_1 = num_span(num_2, k, num_2->count);
        CLU_HANDLER_REGISTER_STATIC(&f->num_2_0)
        CLU_HANDLER_REGISTER_STATIC(&f->num_2_1)

        uint64_t n = stdc_bit_ceil(8 * k);
        f->num_2_0_cache = (num_fft_cache_t)
        {
            .num = &f->num_2_0,
            .n = n,
        };
    }

    num_p num_q[2];
    for(uint64_t i=1; i!=UINT64_MAX; i--)
    {
        num_t num_1_1 = num_span(num_1, k * (i + 1), num_1->count);
        CLU_HANDLER_REGISTER_STATIC(&num_1_1)

        num_p num_q_tmp = num_div_mod_bz_rec(num_aux, &num_1_1, &f->num_2_1, &f[1]);
        while(num_normalize(num_1));

        num_mul_memoized(num_aux, num_q_tmp, &f->num_2_0_cache);
        // num_mul_inner(num_aux, num_q_tmp, &f->num_2_0);
        while(num_cmp_offset(num_1, k * i, num_aux) < 0)
        {
            num_q_tmp = num_sub_uint(num_q_tmp, 1);
            num_1 = num_add_offset(num_1, k * i, num_2, 0);
        }
        num_1 = num_sub_offset(num_1, k * i, num_aux);

        num_q[i] = num_q_tmp;
    }

    // printf("\nA");
    num_p num_res = num_join(num_q[1], num_q[0], k);
    // printf("\nB");
    return num_res;
}

// Input expected to be normalized
// Returns quocient
// NUM_1 becomes remainder
// Keeps NUM_2
num_p num_div_mod_bz(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1)
    CLU_HANDLER_IS_SAFE(num_2)
    assert(num_1)
    assert(num_2)

    uint64_t n_2 = num_2->count;
    uint64_t n_1 = num_1->count;

    uint64_t frame_count = stdc_bit_width(num_2->count);
    bz_frame_t f[frame_count];
    memset(f, 0, sizeof(f));

    uint64_t count = stdc_bit_ceil(8 * num_2->count);
    num_p num_aux = num_create(count, 0);
    num_p num_q = num_create(num_1->count - num_2->count + 1, 0);
    num_aux->cannot_expand = true;
    for(uint64_t i=0; n_1 > 2 * n_2; i++)
    {
        uint64_t k_q = num_2->count;
        uint64_t k_1 = num_1->count - 2 * k_q;
        num_t num_1_1 = num_span(num_1, k_1, num_1->count);
        CLU_HANDLER_REGISTER_STATIC(&num_1_1)

        num_p num_q_tmp = num_div_mod_bz_rec(num_aux, &num_1_1, num_2, f);
        while(num_normalize(num_1));
        num_q = num_join(num_q, num_q_tmp, k_q);

        n_1 -= n_2;
    }

    num_p num_q_tmp = num_div_mod_bz_rec(num_aux, num_1, num_2, f);
    num_q = num_join(num_q, num_q_tmp, n_1 - n_2);
    num_free(num_aux);

    for(uint64_t i=0; i<frame_count && f[i].memoized; i++)
        if(f[i].num_2_0_cache.memoized)
            num_free(f[i].num_2_0_cache.fft);

    return num_q;
}

uint64_t num_div_normalize(num_p *num_1, num_p *num_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(*num_1);
    CLU_HANDLER_IS_SAFE(*num_2);
    assert(*num_1);
    assert(*num_2);

    uint64_t bits = 64 - stdc_bit_width((*num_2)->chunk[(*num_2)->count-1]);
    *num_1 = num_shl_inner((*num_1), bits);
    *num_2 = num_shl_inner((*num_2), bits);
    return bits;
}

// out_num_q and out_num_r can be NULL
void num_div_mod_finalize(
    num_p *out_num_q,
    num_p *out_num_r,
    num_p num_q,
    num_p num_1,
    num_p num_2,
    uint64_t bits
)
{
    num_free(num_2);

    if(out_num_q)
    {
        *out_num_q = num_q;
    }
    else
    {
        num_free(num_q);
    }

    if(out_num_r)
    {
        num_1 = num_shr_inner(num_1, bits);
        *out_num_r = num_1;
    }
    else
    {
        num_free(num_1);
    }
}

// out_num_q and out_num_r can be NULL
void num_div_mod_inner(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    assert(num_2->count);

    if(num_cmp(num_1, num_2) < 0)
    {
        num_p num_q = num_create(0, 0);
        num_div_mod_finalize(out_num_q, out_num_r, num_q, num_1, num_2, 0);
        return;
    }

    if(num_2->count == 1)
    {
        num_p num_q = num_div_mod_uint(num_1, num_2->chunk[0]);
        num_div_mod_finalize(out_num_q, out_num_r, num_q, num_1, num_2, 0);
        return;
    }

    uint64_t bits = num_div_normalize(&num_1, &num_2);
    num_p num_q = num_div_mod_bz(num_1, num_2);
    num_div_mod_finalize(out_num_q, out_num_r, num_q, num_1, num_2, bits);
}



bool num_is_zero(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    return num->count == 0;
}

int64_t num_cmp(num_p num_1, num_p num_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);

    return num_cmp_offset(num_1, 0, num_2);
}



num_p num_shl(num_p num, uint64_t bits) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return num;

    num = num_shl_inner(num, bits & 0x3f);
    return num_head_grow(num, bits >> 6);
}

num_p num_shr(num_p num, uint64_t bits) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return num;

    num = num_head_trim(num, bits >> 6);
    return num_shr_inner(num, bits & 0x3f);
}

num_p num_add_uint(num_p num, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    return num_add_uint_offset(num, 0, value);
}

num_p num_sub_uint(num_p num, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    return num_sub_uint_offset(num, 0, value);
}

num_p num_mul_uint(num_p num, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    num_p num_res = num_create(num->count + 1, 0);
    num_res = num_add_mul_uint_offset(num_res, 0, num, 0, value);
    num_free(num);
    return num_res;
}



num_p num_add(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_1 = num_add_offset(num_1, 0, num_2, 0);
    num_free(num_2);
    return num_1;
}

num_p num_sub(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_1 = num_sub_offset(num_1, 0, num_2);
    num_free(num_2);
    return num_1;
}

num_p num_mul_high(num_p num_1, num_p num_2, uint64_t pos)
{
    CLU_HANDLER_IS_SAFE(num_1)
    CLU_HANDLER_IS_SAFE(num_2)
    assert(num_1)
    assert(num_2)

    num_p num_res = num_create(num_1->count + num_2->count + 1 - pos, 0);
    num_mul_high_inner(num_res, num_1, num_2, pos);
    num_free(num_1);
    num_free(num_2);
    return num_res;    
}

num_p num_mul(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1)
    CLU_HANDLER_IS_SAFE(num_2)
    assert(num_1)
    assert(num_2)

    uint64_t count = stdc_bit_ceil(4 * (num_1->count + num_2->count));
    num_p num_res = num_create(count, 0);
    num_mul_inner(num_res, num_1, num_2);
    num_free(num_1);
    num_free(num_2);
    return num_res;    
}

num_p num_sqr(num_p num)
{
    if(num->count == 0)
        return num;

    if(num->count < 1500)
        return num_sqr_classic(num);

    return num_sqr_fft(num);
}

void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_div_mod_inner(out_num_q, out_num_r, num_1, num_2);
}

num_p num_div(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_p num_q;
    num_div_mod_inner(&num_q, NULL, num_1, num_2);
    return num_q;
}

num_p num_mod(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_p num_r;
    num_div_mod_inner(NULL, &num_r, num_1, num_2);
    return num_r;
}

num_p num_gcd(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    while(num_2->count != 0)
    {
        num_p num_aux = num_mod(num_1, num_copy(num_2));
        num_1 = num_2;
        num_2 = num_aux;
    }
    num_free(num_2);
    return(num_1);
}



num_p num_base_to(num_p num, uint64_t base)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
    assert(base > 1);

    num_p num_res = num_create(num->count, 0);
    for(uint64_t i=0; num->count; i++)
    {
        num_p num_q = num_div_mod_uint(num, base);

        uint64_t value = num_unwrap(num);
        num_res = num_chunk_set(num_res, i, value);
        num = num_q;
    }
    num_free(num);
    return num_res;
}

num_p num_base_from(num_p num, uint64_t base)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
    assert(base > 1);

    num_p num_res = num_create(2 * num->count, 0);
    for(uint64_t i=num->count-1; i!=UINT64_MAX; i--)
    {
        assert(num->chunk[i] < base);
        num_res = num_mul_uint(num_res, base);
        num_res = num_add_uint(num_res, num->chunk[i]);
    }
    num_free(num);
    return num_res;
}
