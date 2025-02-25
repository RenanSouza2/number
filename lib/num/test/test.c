#include "../debug.h"
#include "../../../utils/U64.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"

void test_uint_from_char(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s  1", __func__);
    assert(uint64(uint_from_char('0'), 0));

    if(show) printf("\n\t\t%s  2", __func__);
    assert(uint64(uint_from_char('1'), 1));

    if(show) printf("\n\t\t%s  3", __func__);
    assert(uint64(uint_from_char('9'), 9));

    if(show) printf("\n\t\t%s  4", __func__);
    assert(uint64(uint_from_char('a'), 10));

    if(show) printf("\n\t\t%s  5", __func__);
    assert(uint64(uint_from_char('b'), 11));

    if(show) printf("\n\t\t%s  6", __func__);
    assert(uint64(uint_from_char('f'), 15));

    if(show) printf("\n\t\t%s  4", __func__);
    assert(uint64(uint_from_char('A'), 10));

    if(show) printf("\n\t\t%s  5", __func__);
    assert(uint64(uint_from_char('B'), 11));

    if(show) printf("\n\t\t%s  6", __func__);
    assert(uint64(uint_from_char('F'), 15));

    assert(clu_mem_empty());
}

void test_uint128(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s  1", __func__);
    uint128_t u = 1;
    assert(uint128_immed(u, 0, 1));

    if(show) printf("\n\t\t%s  2", __func__);
    u = UINT64_MAX;
    assert(uint128_immed(u, 0, UINT64_MAX));

    if(show) printf("\n\t\t%s  3", __func__);
    u = U128_IMMED(1, 0);
    assert(uint128_immed(u, 1, 0));

    if(show) printf("\n\t\t%s  4", __func__);
    u = U128_IMMED(1, 2);
    assert(uint128_immed(u, 1, 2));

    if(show) printf("\n\t\t%s  5", __func__);
    u = U128_IMMED(UINT64_MAX, UINT64_MAX);
    assert(uint128_immed(u, UINT64_MAX, UINT64_MAX));

    if(show) printf("\n\t\t%s  6", __func__);
    u = UINT64_MAX;
    u += 1;
    assert(uint128_immed(u, 1, 0));

    if(show) printf("\n\t\t%s  7", __func__);
    u = U128_IMMED(1, 0);
    u -= 1;
    assert(uint128_immed(u, 0, UINT64_MAX));

    if(show) printf("\n\t\t%s  8", __func__);
    u = UINT64_MAX;
    u *= UINT64_MAX;
    assert(uint128_immed(u, UINT64_MAX - 1, 1));

    if(show) printf("\n\t\t%s  9", __func__);
    u = U128_IMMED(1, 0);
    u <<= 1;
    assert(uint128_immed(u, 2, 0));

    if(show) printf("\n\t\t%s 10", __func__);
    u = U128_IMMED(1, 0);
    u >>= 1;
    assert(uint128_immed(u, 0, 0x8000000000000000));

    if(show) printf("\n\t\t%s 11", __func__);
    u = U128_IMMED(1, 0);
    u /= 1;
    assert(uint128_immed(u, 1, 0));

    if(show) printf("\n\t\t%s 12", __func__);
    u = U128_IMMED(1, 0);
    u /= 2;
    assert(uint128_immed(u, 0, 0x8000000000000000));

    if(show) printf("\n\t\t%s 13", __func__);
    u = U128_IMMED(UINT64_MAX, UINT64_MAX);
    u /= 1;
    assert(uint128_immed(u, UINT64_MAX, UINT64_MAX));

    if(show) printf("\n\t\t%s 14", __func__);
    u = U128_IMMED(UINT64_MAX / 2, UINT64_MAX);
    u /= 2;
    assert(uint128_immed(u, UINT64_MAX >> 2, UINT64_MAX));

    if(show) printf("\n\t\t%s 15", __func__);
    u = U128_IMMED(UINT64_MAX, UINT64_MAX - 1);
    u /= 2;
    assert(uint128_immed(u, UINT64_MAX >> 1, UINT64_MAX));

    assert(clu_mem_empty());
}



void test_node_create(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    node_p node = node_create(0, NULL, NULL);
    assert(node->value == 0);
    assert(node->next == NULL);
    assert(node->prev == NULL);
    free(node);

    if(show) printf("\n\t\t%s 2", __func__);
    node = node_create(1, NULL, NULL);
    assert(node->value == 1);
    assert(node->next == NULL);
    assert(node->prev == NULL);
    free(node);

    if(show) printf("\n\t\t%s 3", __func__);
    node_p node_next = node_create(0, NULL, NULL);
    node = node_create(1, node_next, NULL);
    assert(node->value == 1);
    assert(node->next == node_next);
    assert(node->next->prev == node);
    assert(node->prev  == NULL);
    free(node);
    free(node_next);

    if(show) printf("\n\t\t%s 4", __func__);
    node_p node_prev = node_create(0, NULL, NULL);
    node = node_create(1, NULL, node_prev);
    assert(node->value == 1);
    assert(node->next == NULL);
    assert(node->prev == node_prev);
    assert(node->prev->next == node);
    free(node);
    free(node_prev);

    if(show) printf("\n\t\t%s 5", __func__);
    node_next = node_create(0, NULL, NULL);
    node_prev = node_create(0, NULL, NULL);
    node = node_create(1, node_next, node_prev);
    assert(node->value == 1);
    assert(node->next == node_next);
    assert(node->next->prev == node);
    assert(node->prev == node_prev);
    assert(node->prev->next == node);
    free(node);
    free(node_next);
    free(node_prev);

    assert(clu_mem_empty());
}

void test_node_consume(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    node_p node = node_consume(NULL);
    assert(node == NULL);

    if(show) printf("\n\t\t%s 2", __func__);
    node = node_create(1, NULL, NULL);
    node = node_consume(node);
    assert(node == NULL);

    if(show) printf("\n\t\t%s 2", __func__);
    node = node_create(1, NULL, NULL);
    node = node_create(2, node, NULL);
    node = node_consume(node);
    assert(node != NULL);
    assert(node->value == 1);
    assert(node->prev == NULL);
    node_free(node);

    assert(clu_mem_empty());
}



void test_num_create_immed(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    assert(num->count == 0);
    assert(num->head == NULL);
    assert(num->tail == NULL);
    num_free(num);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(1, 2);
    assert(num->count == 1);
    assert(num->head != NULL);
    assert(num->head->value == 2);
    assert(num->head->next == NULL);
    assert(num->head->prev == NULL);
    assert(num->tail == num->head);
    num_free(num);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(2, 2, 1);
    assert(num->count == 2);
    assert(num->head != NULL)
    assert(num->head->value == 1);
    assert(num->head->next != NULL);
    assert(num->head->prev == NULL);
    assert(num->head->next->value == 2);
    assert(num->head->next->next == NULL);
    assert(num->head->next->prev == num->head);
    assert(num->tail == num->head->next);
    num_free(num);

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(2, 2, 0);
    assert(num->count == 2);
    assert(num->head != NULL)
    assert(num->head->value == 0);
    assert(num->head->next != NULL);
    assert(num->head->prev == NULL);
    assert(num->head->next->value == 2);
    assert(num->head->next->next == NULL);
    assert(num->head->next->prev == num->head);
    assert(num->tail == num->head->next);
    num_free(num);

    assert(clu_mem_empty());
}

void test_num_insert(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    node_p node = num_insert(num, 0);
    assert(uint64(node->value, 0));
    assert(num->head == node);
    assert(num->tail == node);
    assert(num_immed(num, 1, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(0);
    node = num_insert(num, 1);
    assert(uint64(node->value, 1));
    assert(num->head == node);
    assert(num->tail == node);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 2);
    node = num_insert(num, 0);
    assert(uint64(node->value, 0));
    assert(num->head != node);
    assert(num->tail == node);
    assert(num_immed(num, 2, 0, 2));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 2);
    node = num_insert(num, 1);
    assert(uint64(node->value, 1));
    assert(num->head != node);
    assert(num->tail == node);
    assert(num_immed(num, 2, 1, 2));

    assert(clu_mem_empty());
}

void test_num_insert_head(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    node_p node = num_insert_head(num, 0);
    assert(uint64(node->value, 0));
    assert(num->head == node);
    assert(num->tail == node);
    assert(num_immed(num, 1, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(0);
    node = num_insert_head(num, 1);
    assert(uint64(node->value, 1));
    assert(num->head == node);
    assert(num->tail == node);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 2);
    node = num_insert_head(num, 0);
    assert(uint64(node->value, 0));
    assert(num->head == node);
    assert(num->tail != node);
    assert(num_immed(num, 2, 2, 0));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 2);
    node = num_insert_head(num, 1);
    assert(uint64(node->value, 1));
    assert(num->head == node);
    assert(num->tail != node);
    assert(num_immed(num, 2, 2, 1));

    assert(clu_mem_empty());
}

void test_num_remove_head(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(0);
    num_remove_head(num);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 1);
    num_remove_head(num);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(2, 1, 2);\
    num_remove_head(num);
    assert(num_immed(num, 1, 1));

    assert(clu_mem_empty());
}

void test_num_insert_list(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(0);
    num_insert_list(num_1, num_2->head, num_2->tail, num_2->count);
    assert(num_immed(num_1, 0));
    free(num_2);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num_1 = num_create_immed(0);
    num_2 = num_create_immed(1, 2);
    num_insert_list(num_1, num_2->head, num_2->tail, num_2->count);
    assert(num_immed(num_1, 1, 2));
    free(num_2);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(0);
    num_insert_list(num_1, num_2->head, num_2->tail, num_2->count);
    assert(num_immed(num_1, 1, 1));
    free(num_2);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(1, 2);
    num_insert_list(num_1, num_2->head, num_2->tail, num_2->count);
    assert(num_immed(num_1, 2, 2, 1));
    free(num_2);

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    num_1 = num_create_immed(2, 1, 2);
    num_2 = num_create_immed(2, 3, 4);
    num_insert_list(num_1, num_2->head, num_2->tail, num_2->count);
    assert(num_immed(num_1, 4, 3, 4, 1, 2));
    free(num_2);

    assert(clu_mem_empty());
}

void test_num_num_denormalize(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(0);
    node_p node = num_denormalize(num, NULL);
    assert(num->tail == node);
    assert(num_immed(num, 1, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 1);
    node = num_denormalize(num, NULL);
    assert(num->tail == node);
    assert(num_immed(num, 2, 0, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(1, 1);
    node = num_denormalize(num, num->head);
    assert(num_immed(num, 1, 1));

    assert(clu_mem_empty());
}

void test_num_num_normalize(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(0);
    bool res = num_normalize(num);
    assert(res == false);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 1);
    res = num_normalize(num);
    assert(res == false);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(1, 0);
    res = num_normalize(num);
    assert(res == true);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num = num_create_immed(2, 0, 1);
    res = num_normalize(num);
    assert(res == true);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num = num_create_immed(2, 0, 0);
    res = num_normalize(num);
    assert(res == true);
    assert(num_immed(num, 1, 0));

    assert(clu_mem_empty());
}



void test_num_wrap(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_wrap(0);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_wrap(2);
    assert(num_immed(num, 1, 2));

    assert(clu_mem_empty());
}

void test_num_wrap_dec(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_wrap_dec("0");
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_wrap_dec("1");
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_wrap_dec("9");
    assert(num_immed(num, 1, 9));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_wrap_dec("10");
    assert(num_immed(num, 1, 10));

    if(show) printf("\n\t\t%s 5", __func__);
    num = num_wrap_dec("18446744073709551615");
    assert(num_immed(num, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 6", __func__);
    num = num_wrap_dec("18446744073709551616");
    assert(num_immed(num, 2, 1, 0));

    if(show) printf("\n\t\t%s 7", __func__);
    num = num_wrap_dec("0000");
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 8", __func__);
    num = num_wrap_dec("00001");
    assert(num_immed(num, 1, 1));

    assert(clu_mem_empty());
}

void test_num_wrap_hex(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s  1", __func__);
    num_p num = num_wrap_hex("0x0");
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s  2", __func__);
    num = num_wrap_hex("0x1");
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s  3", __func__);
    num = num_wrap_hex("0x9");
    assert(num_immed(num, 1, 9));

    if(show) printf("\n\t\t%s  4", __func__);
    num = num_wrap_hex("0xa");
    assert(num_immed(num, 1, 10));

    if(show) printf("\n\t\t%s  5", __func__);
    num = num_wrap_hex("0xA");
    assert(num_immed(num, 1, 10));

    if(show) printf("\n\t\t%s  6", __func__);
    num = num_wrap_hex("0x10");
    assert(num_immed(num, 1, 16));

    if(show) printf("\n\t\t%s  7", __func__);
    num = num_wrap_hex("0xffffffffffffffff");
    assert(num_immed(num, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s  8", __func__);
    num = num_wrap_hex("0x10000000000000000");
    assert(num_immed(num, 2, 1, 0));

    if(show) printf("\n\t\t%s  9", __func__);
    num = num_wrap_hex("0x0000");
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 10", __func__);
    num = num_wrap_hex("0x00001");
    assert(num_immed(num, 1, 1));

    assert(clu_mem_empty());
}

void test_num_wrap_str(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s  1", __func__);
    num_p num = num_wrap_str("0");
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s  2", __func__);
    num = num_wrap_str("1");
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s  3", __func__);
    num = num_wrap_str("9");
    assert(num_immed(num, 1, 9));

    if(show) printf("\n\t\t%s  4", __func__);
    num = num_wrap_str("10");
    assert(num_immed(num, 1, 10));

    if(show) printf("\n\t\t%s  5", __func__);
    num = num_wrap_str("18446744073709551615");
    assert(num_immed(num, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s  6", __func__);
    num = num_wrap_str("18446744073709551616");
    assert(num_immed(num, 2, 1, 0));

    if(show) printf("\n\t\t%s  7", __func__);
    num = num_wrap_str("0000");
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s  8", __func__);
    num = num_wrap_str("00001");
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s  9", __func__);
    num = num_wrap_str("0x0");
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 10", __func__);
    num = num_wrap_str("0x1");
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 11", __func__);
    num = num_wrap_str("0x9");
    assert(num_immed(num, 1, 9));

    if(show) printf("\n\t\t%s 12", __func__);
    num = num_wrap_str("0xa");
    assert(num_immed(num, 1, 10));

    if(show) printf("\n\t\t%s 13", __func__);
    num = num_wrap_str("0xA");
    assert(num_immed(num, 1, 10));

    if(show) printf("\n\t\t%s 14", __func__);
    num = num_wrap_str("0x10");
    assert(num_immed(num, 1, 16));

    if(show) printf("\n\t\t%s 15", __func__);
    num = num_wrap_str("0xffffffffffffffff");
    assert(num_immed(num, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 16", __func__);
    num = num_wrap_str("0x10000000000000000");
    assert(num_immed(num, 2, 1, 0));

    if(show) printf("\n\t\t%s 17", __func__);
    num = num_wrap_str("0x0000");
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 18", __func__);
    num = num_wrap_str("0x00001");
    assert(num_immed(num, 1, 1));

    assert(clu_mem_empty());
}

void test_num_copy(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_wrap(0);
    num_p num_res = num_copy(num);
    assert(num_immed(num_res, 0));
    num_free(num);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(1, 1);
    num_res = num_copy(num);
    assert(num_immed(num_res, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(2, 1, 2);
    num_res = num_copy(num);
    assert(num_immed(num_res, 2, 1, 2));
    num_free(num);

    assert(clu_mem_empty());
}



void test_num_add_uint(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    num_add_uint(num, 0);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(0);
    num_add_uint(num, 1);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 1);
    num_add_uint(num, 2);
    assert(num_immed(num, 1, 3));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, UINT64_MAX);
    num_add_uint(num, 3);
    assert(num_immed(num, 2, 1, 2));

    assert(clu_mem_empty());
}

void test_num_sub_uint(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(0);
    num_sub_uint(num, 0);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 1);
    num_sub_uint(num, 0);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(1, 1);
    num_sub_uint(num, 1);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num = num_create_immed(1, 2);
    num_sub_uint(num, 1);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    num = num_create_immed(2, 1, 0);
    num_sub_uint(num, 1);
    assert(num_immed(num, 1, UINT64_MAX));

    assert(clu_mem_empty());
}

void test_num_sub_uint_offset(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(2, 2, 3);
    node_p node = num_get_node(num, 0);
    bool eliminated = num_sub_uint_offset(num, node, 1);
    assert(num_immed(num, 2, 2, 2));
    assert(eliminated == false);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(2, 2, 3);
    node = num_get_node(num, 1);
    eliminated = num_sub_uint_offset(num, node, 1);
    assert(num_immed(num, 2, 1, 3));
    assert(eliminated == false);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(2, 1, 3);
    node = num_get_node(num, 1);
    eliminated = num_sub_uint_offset(num, node, 1);
    assert(num_immed(num, 1, 3));
    assert(eliminated == true);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num = num_create_immed(2, 1, 0);
    node = num_get_node(num, 1);
    eliminated = num_sub_uint_offset(num, node, 1);
    assert(num_immed(num, 1, 0));
    assert(eliminated == true);

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    num = num_create_immed(3, 1, 0, 1);
    node = num_get_node(num, 2);
    eliminated = num_sub_uint_offset(num, node, 1);
    assert(num_immed(num, 2, 0, 1));
    assert(eliminated == true);

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    num = num_create_immed(3, 1, 0, 1);
    node = num_get_node(num, 1);
    eliminated = num_sub_uint_offset(num, node, 1);
    assert(num_immed(num, 2, UINT64_MAX, 1));
    assert(eliminated == false);

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    num = num_create_immed(2, 1, 0);
    node = num_get_node(num, 1);
    eliminated = num_sub_uint_offset(num, node, 1);
    assert(num_immed(num, 1, 0));
    assert(eliminated == true);

    assert(clu_mem_empty());
}

void test_num_mul_uint(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    num_p num = num_create_immed(0);
    num_p num_res = num_mul_uint(NULL, num, 0);
    assert(num_immed(num_res, 0));
    num_free(num);

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    num = num_create_immed(0);
    num_res = num_mul_uint(NULL, num, 1);
    assert(num_immed(num_res, 0));
    num_free(num);

    if(show) printf("\n\t\t%s  3\t\t", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, 0);
    assert(num_immed(num_res, 0));
    num_free(num);

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, 3);
    assert(num_immed(num_res, 1, 6));
    num_free(num);

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, UINT64_MAX);
    assert(num_immed(num_res, 2, 1, UINT64_MAX - 1));
    num_free(num);

    if(show) printf("\n\t\t%s  6\t\t", __func__);
    num = num_create_immed(2, 2, 3);
    num_res = num_mul_uint(NULL, num, 4);
    assert(num_immed(num_res, 2, 8, 12));
    num_free(num);

    if(show) printf("\n\t\t%s  7\t\t", __func__);
    num = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_mul_uint(NULL, num, UINT64_MAX);
    assert(num_immed(num_res, 3, UINT64_MAX - 1, UINT64_MAX, 1));
    num_free(num);

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    num = num_create_immed(2, 2, 0);
    num_res = num_mul_uint(NULL, num, 3);
    assert(num_immed(num_res, 2, 6, 0));
    num_free(num);

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    num = num_create_immed(0);
    num_res = num_create_immed(1, 1);
    num_res = num_mul_uint(num_res, num, 0);
    assert(num_immed(num_res, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    num = num_create_immed(0);
    num_res = num_create_immed(1, 1);
    num_res = num_mul_uint(num_res, num, 1);
    assert(num_immed(num_res, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(num_res, num, 0);
    assert(num_immed(num_res, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 12\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(num_res, num, 3);
    assert(num_immed(num_res, 1, 7));
    num_free(num);

    if(show) printf("\n\t\t%s 13\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(num_res, num, UINT64_MAX);
    assert(num_immed(num_res, 2, 1, UINT64_MAX));
    num_free(num);

    if(show) printf("\n\t\t%s 14\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(2, 2, 3);
    num_res = num_mul_uint(num_res, num, 4);
    assert(num_immed(num_res, 2, 8, 13));
    num_free(num);

    if(show) printf("\n\t\t%s 15\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_mul_uint(num_res, num, UINT64_MAX);
    assert(num_immed(num_res, 3, UINT64_MAX - 1, UINT64_MAX, 2));
    num_free(num);

    if(show) printf("\n\t\t%s 16\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(2, 2, 0);
    num_res = num_mul_uint(num_res, num, 3);
    assert(num_immed(num_res, 2, 6, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 17\t\t", __func__);
    num = num_create_immed(2, 6, 0xea7db545decb57a4);
    num_res = num_mul_uint(NULL, num, 123427205323);
    assert(num_immed(num_res, 2, 0xc6bfba060d, 0xdf4c110fd2f33f0c));
    num_free(num);

    assert(clu_mem_empty());
}



void test_num_cmp(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(0);
    int64_t cmp = num_cmp(num_1, num_2);
    assert(cmp == 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp > 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  3\t\t", __func__);
    num_1 = num_create_immed(0);
    num_2 = num_create_immed(1, 1);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(1, 2);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 2);
    cmp = num_cmp(num_1, num_2);
    assert(cmp == 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  6\t\t", __func__);
    num_1 = num_create_immed(1, 3);
    num_2 = num_create_immed(1, 2);
    cmp = num_cmp(num_1, num_2);
    assert(cmp > 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  7\t\t", __func__);
    num_1 = num_create_immed(2, 2, 0);
    num_2 = num_create_immed(2, 1, 0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp > 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    num_1 = num_create_immed(2, 2, 0);
    num_2 = num_create_immed(2, 2, 0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp == 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    num_1 = num_create_immed(2, 2, 0);
    num_2 = num_create_immed(2, 3, 0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    num_1 = num_create_immed(2, 0x8000000000000000, 0);
    num_2 = num_create_immed(2, 0x8000000000000000, 0x7fffffffffffffff);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    num_1 = num_create_immed(3, 0x8000000000000000, 0, 0);
    num_2 = num_create_immed(3, 0x8000000000000000, 0x7fffffffffffffff, 0x8000000000000000);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    assert(clu_mem_empty());
}



void test_num_add(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(0);
    num_p num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(0);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num_1 = num_create_immed(0);
    num_2 = num_create_immed(1, 1);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 1, 1));

    if(show) printf("\n\t\t%s 4", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(1, 2);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 1, 3));

    if(show) printf("\n\t\t%s 5", __func__);
    num_1 = num_create_immed(2, 2, 1);
    num_2 = num_create_immed(1, 2);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 2, 2, 3));

    if(show) printf("\n\t\t%s 6", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, 2, 3);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 2, 2, 4));

    if(show) printf("\n\t\t%s 7", __func__);
    num_1 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_2 = num_create_immed(1, 1);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 3, 1, 0, 0));

    if(show) printf("\n\t\t%s 8", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 3, 1, 0, 0));

    assert(clu_mem_empty());
}

void test_num_sub(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(0);
    num_p num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(0);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 1, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 1);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 1, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 2);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(1, 1);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    num_1 = num_create_immed(2, 4, 3);
    num_2 = num_create_immed(2, 1, 2);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 2, 3, 1));

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(2, 1, 0);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 0));

    assert(clu_mem_empty());
}

void test_num_mul(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s  1", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(0);
    num_p num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s  2", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(0);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s  3", __func__);
    num_1 = num_create_immed(0);
    num_2 = num_create_immed(1, 1);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s  4", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 3);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 1, 6));

    if(show) printf("\n\t\t%s  5", __func__);
    num_1 = num_create_immed(1, UINT64_MAX);
    num_2 = num_create_immed(1, UINT64_MAX);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, UINT64_MAX - 1, 1));

    if(show) printf("\n\t\t%s  6", __func__);
    num_1 = num_create_immed(2, 2, 3);
    num_2 = num_create_immed(1, 4);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 8, 12));

    if(show) printf("\n\t\t%s  7", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(2, 3, 4);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 6, 8));

    if(show) printf("\n\t\t%s  8", __func__);
    num_1 = num_create_immed(2, 2, 3);
    num_2 = num_create_immed(2, 4, 5);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 3, 8, 22, 15));

    if(show) printf("\n\t\t%s  9", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(1, 1);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 1, 0));

    if(show) printf("\n\t\t%s 10", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, 1, 0);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 1, 0));

    if(show) printf("\n\t\t%s 11", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(2, 1, 0);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 3, 1, 0, 0));

    if(show) printf("\n\t\t%s 12", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(2, 2, UINT64_MAX);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 5, UINT64_MAX - 1));

    if(show) printf("\n\t\t%s 13", __func__);
    num_1 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 4, UINT64_MAX, UINT64_MAX - 1, 0, 1));

    if(show) printf("\n\t\t%s 14", __func__);
    num_1 = num_create_immed(3, 2, 3, 4);
    num_2 = num_create_immed(3, 5, 6, 7);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 5, 10, 27, 52, 45, 28));

    if(show) printf("\n\t\t%s 14", __func__);
    num_1 = num_create_immed(3, 2, 3, 4);
    num_2 = num_create_immed(3, 5, 6, 7);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 5, 10, 27, 52, 45, 28));

    assert(clu_mem_empty());
}

void test_num_div_mod(bool show)
{
    printf("\n\t%s", __func__);

    num_p num_q, num_r;

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(1, 1);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 0));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    num_1 = num_create_immed(1, 4);
    num_2 = num_create_immed(1, 2);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 2));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  3\t\t", __func__);
    num_1 = num_create_immed(1, 5);
    num_2 = num_create_immed(1, 2);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 2));
    assert(num_immed(num_r, 1, 1));

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    num_1 = num_create_immed(1, 5);
    num_2 = num_create_immed(1, 5);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 1));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    num_1 = num_create_immed(1, 9);
    num_2 = num_create_immed(1, 3);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 3));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  6\t\t", __func__);
    num_1 = num_create_immed(3, 1, 0, 0);
    num_2 = num_create_immed(2, 1, 0);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 2, 1, 0));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  7\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, 1, 0);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 0));
    assert(num_immed(num_r, 1, 1));

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    num_1 = num_create_immed(2, 4, UINT64_MAX);
    num_2 = num_create_immed(2, 2, 0);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 2));
    assert(num_immed(num_r, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    num_1 = num_create_immed(2, 4, 0);
    num_2 = num_create_immed(2, 2, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 1));
    assert(num_immed(num_r, 2, 1, 1));

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 1));
    assert(num_immed(num_r, 1, 1));

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    num_1 = num_create_immed(2, UINT64_MAX, 0);
    num_2 = num_create_immed(2, 1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 0x7fffffffffffffff));
    assert(num_immed(num_r, 2, 1, 0x7fffffffffffffff));

    if(show) printf("\n\t\t%s 12\t\t", __func__);
    num_1 = num_create_immed(2, 0xc929d7d593, 0xb7090a859117cfa4);
    num_2 = num_create_immed(2, 6, 0xea7db545decb57a4);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 0x0000001d1635b735));
    assert(num_immed(num_r, 1, 0x88c80995d8646eb0));

    if(show) printf("\n\t\t%s 13\t\t", __func__);
    num_1 = num_create_immed(3, UINT64_MAX, 0, UINT64_MAX);
    num_2 = num_create_immed(1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 3, 1, 0, 1));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s 14\t\t", __func__);
    num_1 = num_create_immed(5, UINT64_MAX, 0, 0, 0, UINT64_MAX);
    num_2 = num_create_immed(1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 5, 1, 0, 0, 0, 1));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s 15\t\t", __func__);
    num_1 = num_create_immed(6, 4, 0, 8, 4, 0, 0);
    num_2 = num_create_immed(4, 4, 0, 0, 4);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 3, 1, 0, 1));
    assert(num_immed(num_r, 4, 3, UINT64_MAX, UINT64_MAX, UINT64_MAX - 3));

    if(show) printf("\n\t\t%s 16\t\t", __func__);
    num_1 = num_create_immed(3, 1, 1, 0);
    num_2 = num_create_immed(2, 1, 2);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, UINT64_MAX));
    assert(num_immed(num_r, 1, 2));

    if(show) printf("\n\t\t%s 17\t\t", __func__);
    num_1 = num_create_immed(3, UINT64_MAX, UINT64_MAX, UINT64_MAX);
    num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 2, 1, 0));
    assert(num_immed(num_r, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 18\t\t", __func__);
    num_1 = num_create_immed(3, 1, 0, 0);
    num_2 = num_create_immed(2, 1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 0x8000000000000000));
    assert(num_immed(num_r, 1, 0x8000000000000000));

    if(show) printf("\n\t\t%s 19\t\t", __func__);
    num_1 = num_create_immed(4, 1, 0, 0, 0);
    num_2 = num_create_immed(3, 1, 0, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, UINT64_MAX));
    assert(num_immed(num_r, 2, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 20\t\t", __func__);
    num_1 = num_create_immed(3, 1, UINT64_MAX - 1, 0);
    num_2 = num_create_immed(2, 1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, UINT64_MAX));
    assert(num_immed(num_r, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 21\t\t", __func__);
    num_1 = num_create_immed(4, 1, UINT64_MAX, UINT64_MAX - 1, 0);
    num_2 = num_create_immed(3, 1, UINT64_MAX, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, UINT64_MAX));
    assert(num_immed(num_r, 3, 1, UINT64_MAX - 1, UINT64_MAX));

    assert(clu_mem_empty());
}



void test_num()
{
    printf("\n%s", __func__);

    bool show = true;

    test_uint_from_char(show);
    test_uint128(show);

    test_node_create(show);
    test_node_consume(show);

    test_num_create_immed(show);
    test_num_insert(show);
    test_num_insert_head(show);
    test_num_remove_head(show);
    test_num_insert_list(show);
    test_num_num_denormalize(show);
    test_num_num_normalize(show);

    test_num_wrap(show);
    test_num_wrap_dec(show);
    test_num_wrap_hex(show);
    test_num_wrap_str(show);
    test_num_copy(show);

    test_num_add_uint(show);
    test_num_sub_uint(show);
    test_num_sub_uint_offset(show);
    test_num_mul_uint(show);

    test_num_cmp(show);

    test_num_add(show);
    test_num_sub(show);
    test_num_mul(show);
    test_num_div_mod(show);

    assert(clu_mem_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_num();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
