#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../utils/assert.h"
#include "../../utils/U64.h"
#include "../../utils/clu/header.h"



#ifdef DEBUG

num_p num_create_variadic(uint64_t n, va_list *args)
{
    if(n == 0)
        return num_create(0, NULL, NULL);

    uint64_t value = va_arg(*args, uint64_t);
    chunk_p tail = chunk_create(value, NULL, NULL);

    chunk_p chunk = tail;
    for(uint64_t i=1; i<n; i++)
    {
        uint64_t value = va_arg(*args, uint64_t);
        chunk = chunk_create(value, chunk, NULL);
    }
    return num_create(n, chunk, tail);
}

num_p num_create_variadic_n(va_list *args)
{
    uint64_t n = va_arg(*args, uint64_t);
    return num_create_variadic(n, args);
}

num_p num_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return num_create_variadic(n, &args);
}

void num_create_immed_vec(num_p out_num[], uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    for(uint64_t i=0; i<n; i++)
        out_num[i] = num_create_variadic_n(&args);
}



bool int64(int64_t u1, int64_t u2)
{
    if(u1 != u2)
    {
        printf("\n\n\tINT64 ASSERT ERROR\t| (" U64PX ") (" U64PX ")", u1, u2);
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



bool num_str_inner(num_p num_1, num_p num_2)
{
    if(!uint64(num_1->count, num_2->count))
    {
        printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN LENGTH");
        return false;
    }

    if(num_1->count == 0)
    {
        if(num_1->head != NULL)
        {
            printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS ZERO BUT IT HAS HEAD");
            return false;
        }

        if(num_1->tail != NULL)
        {
            printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS ZERO BUT IT HAS TAIL");
            return false;
        }

        return true;
    }

    if(num_1->head == NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS " U64P() " BUT IT HAS NO HEAD", num_1->count);
        return false;
    }

    if(num_1->head->prev != NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| HEAD HAS PREV");
        return false;
    }

    if(num_1->tail == NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS " U64P() " BUT IT HAS NO TAIL", num_1->count);
        return false;
    }

    if(num_1->tail->next != NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| TAIL HAS NEXT");
        return false;
    }

    chunk_p chunk_1 = num_1->tail;
    chunk_p chunk_2 = num_2->tail;
    for(uint64_t count = num_1->count; count > 0; count--)
    {
        if(count > 1)
        {
            if(chunk_1->prev == NULL)
            {
                printf("\n\n\tchunk VALIDITY ERROR\t| NUMBER SHORTER THAN EXPECTED");
                return false;
            }

            if(chunk_1->prev->next != chunk_1)
            {
                printf("\n\n\tchunk VALIDITY ERROR\t| INVALID LINKAGE");
                return false;
            }
        }
        else
        {
            if(chunk_1 != num_1->head)
            {
                printf("\n\n\tchunk VALIDITY ERROR\t| INVALID HEAD");
                return false;
            }
        }

        if(!uint64(chunk_1->value, chunk_2->value))
        {
            printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN VALUE " U64P() "", count - 1);
            return false;
        }

        chunk_1 = chunk_1->prev;
        chunk_2 = chunk_2->prev;
    }

    if(chunk_1 != NULL)
    {
        printf("\n\tchunk VALIDITY ERROR\t| NUMBER LONGER THAN EXPECTED");
        return false;
    }

    return true;
}

bool num_str(num_p num_1, num_p num_2)
{
    bool res = num_str_inner(num_1, num_2);
    num_free(num_1);
    num_free(num_2);

    if(!res)
    {
        printf("\n");
        num_display_full("\tnum_1", num_1);
        num_display_full("\tnum_2", num_2);
        return false;
    }

    return true;
}

bool num_immed(num_p num, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    num_p num_2 = num_create_variadic(n, &args);
    return num_str(num, num_2);
}



chunk_p num_get_chunk(num_p num, uint64_t count)
{
    chunk_p chunk = num->head;
    for(uint64_t i=0; i<count && chunk; i++, chunk = chunk->next);
    return chunk;
}

#endif



#define COALESCE(PTR_A, PTR_B) (PTR_A) ? (PTR_A) : (PTR_B);



uint64_t uint_from_char(char c)
{
    switch (c)
    {
        case '0' ... '9': return c - '0';
        case 'a' ... 'f': return c - 'a' + 10;
        case 'A' ... 'F': return c - 'A' + 10;
    }
    assert(false);
}

uint64_t uint_from_str(char str[], uint64_t size, uint64_t base) // TODO test
{
    uint64_t value = 0;
    for(uint64_t i=0; i<size; i++)
    {
        uint64_t aux = uint_from_char(str[i]);
        assert(aux < base);
        value = value * base + uint_from_char(str[i]);
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



void num_display_dec(num_p num)
{
    DBG_CHECK_PTR(num);

    if(num->count == 0)
    {
        printf("0");
        return;
    }

    num = num_base_to(num_copy(num), 1000000000000000000);
    printf(U64P(), num->tail->value);
    for(chunk_p chunk = num->tail->prev; chunk; chunk = chunk->prev)
        printf(U64P(018), chunk->value);

    num_free(num);
}

void num_display_opts(num_p num, bool length, bool full)
{
    DBG_CHECK_PTR(num);

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

    uint64_t max = full ? UINT64_MAX : 4;
    chunk_p chunk = num->tail;
    for(uint64_t i=0; i<max && chunk != NULL; i++, chunk = chunk->prev)
        printf("" U64PX " ", chunk->value);

    if(!full && num->count > 4)
        printf("...");
}

void num_display(num_p num)
{
    DBG_CHECK_PTR(num);

    num_display_opts(num, true, false);
}

void num_display_tag(char *tag, num_p num)
{
    DBG_CHECK_PTR(num);

    printf("\n%s: ", tag);
    num_display(num);
}

void num_display_full(char *tag, num_p num)
{
    DBG_CHECK_PTR(num);

    printf("\n%s: ", tag);
    num_display_opts(num, true, true);
}



chunk_p chunk_create(uint64_t value, chunk_p next, chunk_p prev)
{
    DBG_CHECK_PTR(next);
    DBG_CHECK_PTR(prev);

    chunk_p chunk = malloc(sizeof(chunk_t));
    assert(chunk);

    if(next) next->prev = chunk;
    if(prev) prev->next = chunk;

    *chunk = (chunk_t)
    {
        .value = value,
        .next = next,
        .prev = prev
    };
    return chunk;
}

chunk_p chunk_consume(chunk_p chunk)
{
    DBG_CHECK_PTR(chunk);

    if(chunk == NULL)
        return NULL;

    chunk_p chunk_next = chunk->next;
    free(chunk);

    if(chunk_next) chunk_next->prev = NULL;
    return chunk_next;
}

void chunk_free(chunk_p chunk)
{
    DBG_CHECK_PTR(chunk);

    while(chunk)
        chunk = chunk_consume(chunk);
}



num_p num_create(uint64_t count, chunk_p head, chunk_p tail)
{
    DBG_CHECK_PTR(head);
    DBG_CHECK_PTR(tail);

    num_p num = malloc(sizeof(num_t));
    assert(num);

    *num = (num_t)
    {
        .count = count,
        .head = head,
        .tail = tail
    };
    return num;
}

chunk_p num_insert(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num->tail = chunk_create(value, NULL, num->tail);

    num->count++;
    num->head = COALESCE(num->head, num->tail);

    return num->tail;
}

chunk_p num_insert_head(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num->head = chunk_create(value, num->head, NULL);

    num->count++;
    num->tail = COALESCE(num->tail, num->head);

    return num->head;
}

void num_remove_head(num_p num)
{
    DBG_CHECK_PTR(num);

    if(num->count == 0)
        return;

    chunk_p chunk_head = num->head;
    num->head = chunk_head->next;
    free(chunk_head);

    num->count--;
    if(num->count == 0)
        num->tail = NULL;
    else
        num->head->prev = NULL;
}

void num_insert_list(num_p num, chunk_p head, chunk_p tail, uint64_t cnt)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(head);
    DBG_CHECK_PTR(tail);

    if(cnt == 0)
        return;

    if(num->count == 0)
    {
        *num = (num_t)
        {
            .count = cnt,
            .head = head,
            .tail = tail
        };
        return;
    }

    num->tail->next = head;
    head->prev = num->tail;

    num->count += cnt;
    num->tail = tail;
}

chunk_p num_denormalize(num_p num, chunk_p chunk)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(chunk);

    return COALESCE(chunk, num_insert(num, 0));
}

bool num_normalize(num_p num)
{
    DBG_CHECK_PTR(num);

    if(num->count == 0)
        return false;

    chunk_p chunk_tail = num->tail;
    if(chunk_tail->value != 0)
        return false;

    num->tail = chunk_tail->prev;
    free(chunk_tail);

    num->count--;
    if(num->count == 0)
        num->head = NULL;
    else
        num->tail->next = NULL;

    return true;
}

void num_break(num_p *out_num_h, num_p *out_num_l, num_p num, uint64_t count) // TODO test
{
    DBG_CHECK_PTR(num);

    if(num->count <= count)
    {
        *out_num_h = num_create(0, NULL, NULL);
        *out_num_l = num;
        return;
    }

    chunk_p chunk = num->head;
    for(uint64_t i=0; i<count; i++)
        chunk = chunk->next;

    chunk->prev = NULL;
    num_p num_h = num_create(num->count - count, chunk, num->tail);

    num->count = count;
    num->tail = chunk->prev;
    num->tail->next = NULL;

    *out_num_h = num_h;
    *out_num_l = num;
}



num_p num_wrap(uint64_t value)
{
    if(value == 0)
        return num_create(0, NULL, NULL);

    chunk_p chunk = chunk_create(value, NULL, NULL);
    return num_create(1, chunk,  chunk);
}

num_p num_wrap_dec(char str[])
{
    uint64_t len = strlen(str);

    uint64_t value = uint_from_str(str, len % 18, 10);
    num_p num = num_wrap(value);
    for(uint64_t i=len % 18; i<len; i+=18)
    {
        value = uint_from_str(&str[i], 18, 10);
        num_insert_head(num, value);
    }
    return num_base_from(num, 1000000000000000000);
}

num_p num_wrap_hex(char str[])
{
    assert(str[0] == '0' && str[1] == 'x');

    uint64_t len = strlen(str);
    uint64_t size = (len - 2) % 16;
    uint64_t value = uint_from_str(&str[2], size, 16);
    num_p num = num_wrap(value);
    for(uint64_t i = 2 + size; i < len; i += 16)
    {
        value = uint_from_str(&str[i], 16, 16);
        num_insert_head(num, value);
    }
    return num;
}

num_p num_wrap_str(char str[])
{
    return str[0] == '0' && str[1] == 'x' ?
        num_wrap_hex(str) : num_wrap_dec(str);
}

num_p num_read_dec(char file_name[])
{
    FILE *fp = fopen(file_name, "r");
    assert(fp);

    fseek(fp, 0, SEEK_END);
    uint64_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint64_t value = uint_read(fp, size % 18, 10);
    num_p num = num_wrap(value);

    while(ftell(fp) < size)
    {
        uint64_t value = uint_read(fp, 18, 10);
        num_insert_head(num, value);
    }

    fclose(fp);
    return num_base_from(num, 1000000000000000000);
}

uint64_t num_unwrap(num_p num) // TODO test
{
    DBG_CHECK_PTR(num);

    assert(num->count < 2);

    uint64_t value = num->count ? num->head->value : 0;
    num_free(num);

    return value;
}

num_p num_copy(num_p num)
{
    DBG_CHECK_PTR(num);

    num_p num_res = num_create(0, NULL, NULL);
    for(chunk_p chunk = num->head; chunk; chunk = chunk->next)
        num_insert(num_res, chunk->value);

    return num_res;
}

void num_free(num_p num)
{
    DBG_CHECK_PTR(num);

    chunk_free(num->head);
    free(num);
}



num_p num_base_to(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num_p num_res = num_create(0, NULL, NULL);
    while (num->count)
    {
        num_p num_q, num_r;
        num_div_mod(&num_q, &num_r, num, num_wrap(value));
        num_insert(num_res, num_unwrap(num_r));
        num = num_q;
    }
    num_free(num);
    return num_res;
}

num_p num_base_from(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num_p num_res = num_create(0, NULL, NULL);
    for(chunk_p chunk = num->tail; chunk; chunk = chunk->prev)
    {
        assert(chunk->value < value);
        num_p num_aux = num_mul_uint(num_wrap(chunk->value), num_res, value);
        num_free(num_res);
        num_res = num_aux;
    }
    num_free(num);
    return num_res;
}



chunk_p num_add_uint_offset(num_p num, chunk_p chunk, uint64_t value)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(chunk);

    chunk_p chunk_0 = chunk;
    for(;value && chunk; chunk = chunk->next)
    {
        chunk->value += value;
        value = chunk->value < value;
    }

    if(value)
        num_insert(num, value);

    return chunk_0 ? chunk_0 : value ? num->tail : NULL;
}

/* returns TRUE if passed offset is TAIL and ELIMINATED */
bool num_sub_uint_offset(num_p num, chunk_p chunk, uint64_t value)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(chunk);

    bool is_tail = chunk == num->tail;

    for(; value; chunk = chunk->next)
    {
        assert(chunk);

        uint64_t next = chunk->value < value;
        chunk->value -= value;
        value = next;
    }

    return num_normalize(num) && is_tail;
}

/* num_res CANNOT be NULL */
chunk_p num_mul_uint_offset(num_p num_res, chunk_p chunk_res, chunk_p chunk_2, uint64_t value)
{
    DBG_CHECK_PTR(num_res);
    DBG_CHECK_PTR(chunk_res);
    DBG_CHECK_PTR(chunk_2);

    if(chunk_2 == NULL)
        return chunk_res;

    chunk_p chunk_0 = chunk_res = num_denormalize(num_res, chunk_res);
    while(chunk_2)
    {
        uint128_t u = MUL(chunk_2->value, value);
        chunk_res = num_add_uint_offset(num_res, chunk_res, LOW(u));
        chunk_res = num_denormalize(num_res, chunk_res);
        num_add_uint_offset(num_res, chunk_res->next, HIGH(u));

        chunk_res = chunk_res->next;
        chunk_2 = chunk_2->next;
    }

    return chunk_0;
}



num_p num_shl_uint(num_p num, uint64_t bits)
{
    DBG_CHECK_PTR(num);

    if(bits == 0)
        return num;

    uint64_t carry = 0;
    for(chunk_p chunk = num->head; chunk; chunk = chunk->next)
    {
        uint64_t value = chunk->value;
        chunk->value = (value << bits) | carry;
        carry = value >> (64 - bits);
    }

    if(carry)
        num_insert(num, carry);

    return num;
}

num_p num_shr_uint(num_p num, uint64_t bits)
{
    DBG_CHECK_PTR(num);

    uint64_t carry = 0;
    for(chunk_p chunk = num->tail; chunk; chunk = chunk->prev)
    {
        uint64_t value = chunk->value;
        chunk->value = (value >> bits) | carry;
        carry = value << (64 - bits);
    }
    num_normalize(num);

    return num;
}

void num_add_uint(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num_add_uint_offset(num, num->head, value);
}

void num_sub_uint(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num_sub_uint_offset(num, num->head, value);
}

/* NUM_RES can be NULL, preserves NUM */
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num_res);
    DBG_CHECK_PTR(num);

    if(num_res == NULL)
        num_res = num_create(0, NULL, NULL);

    if(value)
        num_mul_uint_offset(num_res, num_res->head, num->head, value);

    return num_res;
}



int64_t num_cmp_offset(num_p num_1, num_p num_2, uint64_t offset)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    if(num_1->count > num_2->count + offset)
        return 1;

    if(num_1->count < num_2->count + offset)
        return -1;

    chunk_p chunk_1 = num_1->tail;
    chunk_p chunk_2 = num_2->tail;
    while(chunk_2)
    {
        if(chunk_1->value > chunk_2->value)
            return 1;

        if(chunk_1->value < chunk_2->value)
            return -1;

        chunk_1 = chunk_1->prev;
        chunk_2 = chunk_2->prev;
    }

    return 0;
}

chunk_p num_sub_offset(num_p num_1, chunk_p chunk_1, chunk_p chunk_2) // TODO test
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(chunk_1);
    DBG_CHECK_PTR(chunk_2);

    if(chunk_2 == NULL)
        return chunk_1;

    chunk_p chunk_0 = chunk_1;
    chunk_p prev_0 = chunk_0->prev;

    while(chunk_2)
    {
        if(num_sub_uint_offset(num_1, chunk_1, chunk_2->value))
        {
            assert(chunk_2->next == NULL);
            free(chunk_2);

            while(num_1->tail != prev_0)
                if(!num_normalize(num_1))
                    break;

            return num_1->tail == prev_0 ? NULL : chunk_0;
        }

        chunk_1 = chunk_1->next;
        chunk_2 = chunk_consume(chunk_2);
    }

    return chunk_0;
}

/*
* returns NUM_B * R if less then NUM_A, returns NULL otherwise
*  keeps NUM_A and NUM_B
*/
num_p num_cmp_mul_uint(num_p num_a, num_p num_b, uint64_t r, uint64_t offset) // TODO test
{
    num_p num_aux = num_create(0, NULL, NULL);
    chunk_p chunk = num_b->tail;
    for(offset += num_b->count-1; chunk; chunk = chunk->prev, offset--)
    {
        uint128_t u = MUL(chunk->value, r);
        num_add_uint(num_aux, HIGH(u));
        num_insert_head(num_aux, LOW(u));

        if(num_cmp_offset(num_a, num_aux, offset) < 0)
        {
            num_free(num_aux);
            return NULL;
        }
    }

    return num_aux;
}

void num_div_mod_offset(
    num_p num_q,
    num_p num_r,
    chunk_p chunk_r,
    uint64_t offset_r,
    num_p num_2
)
{
    DBG_CHECK_PTR(num_q);
    DBG_CHECK_PTR(num_r);
    DBG_CHECK_PTR(chunk_r);
    DBG_CHECK_PTR(num_2);

    bool bool_2 = num_2->count == 1;
    uint128_t val_2_1 = num_2->tail->value;
    uint128_t val_2_2 = bool_2 ? val_2_1 : U128_IMMED(val_2_1, num_2->tail->prev->value);

    for(; chunk_r; offset_r--)
    {
        if(num_normalize(num_r))
        {
            num_insert_head(num_q, 0);
            chunk_r = num_r->tail;
            continue;
        }

        uint64_t r =  0;
        while(num_cmp_offset(num_r, num_2, offset_r) >= 0)
        {
            bool bool_1 = num_r->count > num_2->count + offset_r;

            uint128_t val_2 = bool_1 ||  bool_2 ? val_2_1 : val_2_2;
            uint128_t val_1 = bool_1 || !bool_2 ?
                U128_IMMED(num_r->tail->value, num_r->tail->prev->value) :
                num_r->tail->value;

            uint128_t tmp = val_1 / val_2;
            uint64_t r_aux = tmp > UINT64_MAX ? UINT64_MAX : tmp;

            num_p num_aux = num_cmp_mul_uint(num_r, num_2, r_aux, offset_r);
            if(num_aux == NULL)
            {
                r_aux = val_1 / (val_2 + 1);
                num_aux = num_mul_uint(NULL, num_2, r_aux);
            }
            r += r_aux;
            chunk_r = num_sub_offset(num_r, chunk_r, num_aux->head);

            free(num_aux);
        }

        num_insert_head(num_q, r);
        chunk_r = chunk_r ? chunk_r->prev : num_r->tail;
    }
}




bool num_is_zero(num_p num)
{
    DBG_CHECK_PTR(num);

    return num->count == 0;
}

int64_t num_cmp(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    return num_cmp_offset(num_1, num_2, 0);
}



num_p num_shl(num_p num, uint64_t bits)
{
    DBG_CHECK_PTR(num);

    for(; bits > 63; bits -= 64)
        num_insert_head(num, 0);

    num_shl_uint(num, bits);

    return num;
}

num_p num_shr(num_p num, uint64_t bits)
{
    DBG_CHECK_PTR(num);

    for(; bits > 63 && num->count; bits -= 64)
        num_remove_head(num);

    if(bits && num->count)
        num_shr_uint(num, bits);

    return num;
}

num_p num_exp(num_p num, uint64_t value) // TODO test
{
    DBG_CHECK_PTR(num);

    if(num->count == 0)
    {
        assert(value);
        return num;
    }

    num_p num_res = num_wrap(1);
    for(uint64_t mask = (uint64_t)1 << 63; mask; mask >>= 1)
    {
        num_res = num_mul(num_res, num_copy(num_res));
        if(value & mask)
            num_res = num_mul(num_res, num_copy(num));
    }
    free(num);
    return num_res;
}



num_p num_add(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2)

    chunk_p chunk_1 = num_1->head;
    chunk_p chunk_2 = num_2->head;
    uint64_t count = num_2->count;
    for(; chunk_1 && chunk_2; count--)
    {
        num_add_uint_offset(num_1, chunk_1, chunk_2->value);

        chunk_1 = chunk_1->next;
        chunk_2 = chunk_consume(chunk_2);
    }

    if(chunk_1 == NULL && chunk_2)
        num_insert_list(num_1, chunk_2, num_2->tail, count);

    free(num_2);
    return num_1;
}

num_p num_sub(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    num_sub_offset(num_1, num_1->head, num_2->head);

    free(num_2);
    return num_1;
}

num_p num_mul(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    if(num_1->count == 0)
    {
        num_free(num_1);
        num_free(num_2);
        return num_create(0, NULL, NULL);
    }

    num_p num_res = num_create(0, NULL, NULL);
    chunk_p chunk_1 = num_1->head;
    chunk_p chunk_2 = num_2->head;

    for(chunk_p chunk_res = NULL; chunk_2; chunk_res = chunk_res->next)
    {
        chunk_res = num_mul_uint_offset(num_res, chunk_res, chunk_1, chunk_2->value);
        chunk_res = num_denormalize(num_res, chunk_res);

        chunk_2 = chunk_consume(chunk_2);
    }

    num_free(num_1);
    free(num_2);
    return num_res;
}

uint64_t num_div_mod_inner(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    assert(num_2->count);

    num_p num_q = num_create(0, NULL, NULL);

    if(num_cmp(num_1, num_2) < 0)
    {
        *out_num_q = num_q;
        *out_num_r = num_1;
        num_free(num_2);
        return 0;
    }

    if(num_2->count == 1)
    {
        num_div_mod_offset(
            num_q,
            num_1,
            num_1->tail,
            num_1->count - num_2->count,
            num_2
        );

        *out_num_q = num_q;
        *out_num_r = num_1;
        num_free(num_2);
        return 0;
    }

    uint64_t bits = 0;
    for(uint64_t l = num_2->tail->value; l >> 63 == 0; l <<= 1)
        bits++;

    num_1 = num_shl_uint(num_1, bits);
    num_2 = num_shl_uint(num_2, bits);

    chunk_p chunk_r = num_1->tail;
    for(uint64_t i=1; i<num_2->count; i++)
        chunk_r = chunk_r->prev;

    num_div_mod_offset(
        num_q,
        num_1,
        chunk_r,
        num_1->count - num_2->count,
        num_2
    );
    num_free(num_2);

    *out_num_q = num_q;
    *out_num_r = num_1;
    return bits;
}

void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    uint64_t bits = num_div_mod_inner(out_num_q, out_num_r, num_1, num_2);
    num_normalize(*out_num_q);
    num_shr_uint(*out_num_r, bits);
}

num_p num_div(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    num_p num_q, num_r;
    num_div_mod_inner(&num_q, &num_r, num_1, num_2);
    num_normalize(num_q);
    num_free(num_r);
    return num_q;
}

num_p num_mod(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    num_p num_q, num_r;
    uint64_t bits = num_div_mod_inner(&num_q, &num_r, num_1, num_2);
    num_shr_uint(num_r, bits);
    num_free(num_q);
    return num_r;
}
