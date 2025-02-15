#include "../debug.h"
#include "../../../utils/U64.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



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

void test_node_denormalize(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    node_p node = node_denormalize(NULL);
    assert(node != NULL);
    assert(node->value == 0);
    assert(node->next == NULL);
    assert(node->prev == NULL);
    node_free(node);

    if(show) printf("\n\t\t%s 2", __func__);
    node = node_create(1, NULL, NULL);
    node = node_denormalize(node);
    assert(node->value == 1);
    node_free(node);

    assert(clu_mem_empty());
}

void test_node_normalize(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    node_p node = node_normalize(NULL);
    assert(node == NULL);

    if(show) printf("\n\t\t%s 2", __func__);
    node = node_create(1, NULL, NULL);
    node = node_normalize(node);
    assert(node != NULL);
    assert(node->value == 1);
    node_free(node);

    if(show) printf("\n\t\t%s 3", __func__);
    node = node_create(1, NULL, NULL);
    node = node_create(0, node, NULL);
    node = node_normalize(node);
    assert(node != NULL);
    assert(node->value == 0);
    assert(node->next != NULL);
    assert(node->next->value == 1);
    node_free(node);

    if(show) printf("\n\t\t%s 4", __func__);
    node = node_create(0, NULL, NULL);
    node = node_normalize(node);
    assert(node == NULL);

    if(show) printf("\n\t\t%s 5", __func__);
    node_p node_prev = node_create(1, NULL, NULL);
    node = node_create(0, NULL, node_prev);
    node = node_normalize(node);
    assert(node == node_prev);
    assert(node_prev->next == NULL);
    node_free(node_prev);

    assert(clu_mem_empty());
}



void test_num_wrap(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_wrap(0);
    assert(num->count == 0);
    assert(num->head == NULL);
    assert(num->tail == NULL);
    num_free(num);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_wrap(2);
    assert(num->count == 1);
    assert(num->head != NULL);
    assert(num->head->value == 2);
    assert(num->head->next == NULL);
    assert(num->head->prev == NULL);
    assert(num->tail == num->head);
    num_free(num);

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

// void test_num_copy(bool show)
// {
//     printf("\n\t%s", __func__);
//
//     if(show) printf("\n\t\t%s 1", __func__);
//     num_p num_res = num_copy(NULL);
//     assert(num_immed(num_res, 0));
//
//     if(show) printf("\n\t\t%s 2", __func__);
//     num_p num = num_create_immed(1, 1);
//     num_res = num_copy(num);
//     assert(num_immed(num_res, 1, 1));
//     num_free(num);
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 3", __func__);
//     num = num_create_immed(2, 1, 2);
//     num_res = num_copy(num);
//     assert(num_immed(num_res, 2, 1, 2));
//     num_free(num);
//     num_free(num_res);
//
//     assert(clu_mem_empty());
// }



void test_num_add_uint(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    num_add_uint(num, 0);
    assert(num_immed(num, 0));
    num_free(num);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(0);
    num_add_uint(num, 1);
    assert(num_immed(num, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 1);
    num_add_uint(num, 2);
    assert(num_immed(num, 1, 3));
    num_free(num);

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, UINT64_MAX);
    num_add_uint(num, 3);
    assert(num_immed(num, 2, 1, 2));
    num_free(num);

    assert(clu_mem_empty());
}

void test_num_sub_uint(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    num_sub_uint(num, 0);
    assert(num_immed(num, 0));
    num_free(num);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(1, 1);
    num_sub_uint(num, 0);
    assert(num_immed(num, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 1);
    num_sub_uint(num, 1);
    assert(num_immed(num, 0));
    num_free(num);

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 2);
    num_sub_uint(num, 1);
    assert(num_immed(num, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 5", __func__);
    num = num_create_immed(2, 1, 0);
    num_sub_uint(num, 1);
    assert(num_immed(num, 1, UINT64_MAX));
    num_free(num);

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
    num_free(num_res);

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    num = num_create_immed(0);
    num_res = num_mul_uint(NULL, num, 1);
    assert(num_immed(num_res, 0));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s  3\t\t", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, 0);
    assert(num_immed(num_res, 0));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, 3);
    assert(num_immed(num_res, 1, 6));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, UINT64_MAX);
    assert(num_immed(num_res, 2, 1, UINT64_MAX - 1));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s  6\t\t", __func__);
    num = num_create_immed(2, 2, 3);
    num_res = num_mul_uint(NULL, num, 4);
    assert(num_immed(num_res, 2, 8, 12));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s  7\t\t", __func__);
    num = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_mul_uint(NULL, num, UINT64_MAX);
    assert(num_immed(num_res, 3, UINT64_MAX - 1, UINT64_MAX, 1));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    num = num_create_immed(2, 2, 0);
    num_res = num_mul_uint(NULL, num, 3);
    assert(num_immed(num_res, 2, 6, 0));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    num = num_create_immed(0);
    num_res = num_create_immed(1, 1);
    num_res = num_mul_uint(num_res, num, 0);
    assert(num_immed(num_res, 1, 1));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    num = num_create_immed(0);
    num_res = num_create_immed(1, 1);
    num_res = num_mul_uint(num_res, num, 1);
    assert(num_immed(num_res, 1, 1));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(num_res, num, 0);
    assert(num_immed(num_res, 1, 1));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s 12\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(num_res, num, 3);
    assert(num_immed(num_res, 1, 7));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s 13\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(num_res, num, UINT64_MAX);
    assert(num_immed(num_res, 2, 1, UINT64_MAX));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s 14\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(2, 2, 3);
    num_res = num_mul_uint(num_res, num, 4);
    assert(num_immed(num_res, 2, 8, 13));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s 15\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_mul_uint(num_res, num, UINT64_MAX);
    assert(num_immed(num_res, 3, UINT64_MAX - 1, UINT64_MAX, 2));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s 16\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(2, 2, 0);
    num_res = num_mul_uint(num_res, num, 3);
    assert(num_immed(num_res, 2, 6, 1));
    num_free(num);
    num_free(num_res);

    assert(clu_mem_empty());
}

// void test_num_shl(bool show)
// {
//     printf("\n\t%s", __func__);
//
//     if(show) printf("\n\t\t%s 1", __func__);
//     num_p num_res = num_shl(NULL);
//     assert(num_immed(num_res, 0));
//
//     if(show) printf("\n\t\t%s 2", __func__);
//     num_p num = num_create_immed(1, 1);
//     num_res = num_shl(num);
//     assert(num_immed(num_res, 1, 2));
//     num_free(num);
//
//     if(show) printf("\n\t\t%s 3", __func__);
//     num = num_create_immed(1, 3);
//     num_res = num_shl(num);
//     assert(num_immed(num_res, 1, 6));
//     num_free(num);
//
//     if(show) printf("\n\t\t%s 4", __func__);
//     num = num_create_immed(1, UINT64_MAX);
//     num_res = num_shl(num);
//     assert(num_immed(num_res, 2, 1, UINT64_MAX - 1));
//     num_free(num);
//
//     if(show) printf("\n\t\t%s 5", __func__);
//     num = num_create_immed(2, 1, 2);
//     num_res = num_shl(num);
//     assert(num_immed(num_res, 2, 2, 4));
//     num_free(num);
//
//     if(show) printf("\n\t\t%s 6", __func__);
//     num = num_create_immed(2, 1, UINT64_MAX);
//     num_res = num_shl(num);
//     assert(num_immed(num_res, 2, 3, UINT64_MAX - 1));
//     num_free(num);
//
//     assert(clu_mem_empty());
// }

// void test_num_shr(bool show)
// {
//     printf("\n\t%s", __func__);
//
//     if(show) printf("\n\t\t%s 1", __func__);
//     num_p num_res = num_shr(NULL);
//     assert(num_immed(num_res, 0));
//
//     if(show) printf("\n\t\t%s 2", __func__);
//     num_p num = num_create_immed(1, 2);
//     num_res = num_shr(num);
//     assert(num_immed(num_res, 1, 1));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 3", __func__);
//     num = num_create_immed(1, 1);
//     num_res = num_shr(num);
//     assert(num_immed(num_res, 0));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 4", __func__);
//     num = num_create_immed(2, 1, 0);
//     num_res = num_shr(num);
//     assert(num_immed(num_res, 1, 0x8000000000000000));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 5", __func__);
//     num = num_create_immed(2, 2, 0);
//     num_res = num_shr(num);
//     assert(num_immed(num_res, 2, 1, 0));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 6", __func__);
//     num = num_create_immed(2, 1, 2);
//     num_res = num_shr(num);
//     assert(num_immed(num_res, 1, 0x8000000000000001));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 7", __func__);
//     num = num_create_immed(2, 2, 2);
//     num_res = num_shr(num);
//     assert(num_immed(num_res, 2, 1, 1));
//     num_free(num_res);
//
//     assert(clu_mem_empty());
// }



// void test_num_cmp(bool show)
// {
//     printf("\n\t%s", __func__);
//
//     if(show) printf("\n\t\t%s 1", __func__);
//     int64_t cmp = num_cmp(NULL, NULL);
//     assert(cmp == 0);
//
//     if(show) printf("\n\t\t%s 2", __func__);
//     num_p num_1 = num_create_immed(1, 1);
//     cmp = num_cmp(num_1, NULL);
//     assert(cmp > 0);
//     num_free(num_1);
//
//     if(show) printf("\n\t\t%s 3", __func__);
//     num_p num_2 = num_create_immed(1, 1);
//     cmp = num_cmp(NULL, num_2);
//     assert(cmp < 0);
//     num_free(num_2);
//
//     if(show) printf("\n\t\t%s 4", __func__);
//     num_1 = num_create_immed(1, 1);
//     num_2 = num_create_immed(1, 2);
//     cmp = num_cmp(num_1, num_2);
//     assert(cmp < 0);
//     num_free(num_1);
//     num_free(num_2);
//
//     if(show) printf("\n\t\t%s 5", __func__);
//     num_1 = num_create_immed(1, 2);
//     num_2 = num_create_immed(1, 2);
//     cmp = num_cmp(num_1, num_2);
//     assert(cmp == 0);
//     num_free(num_1);
//     num_free(num_2);
//
//     if(show) printf("\n\t\t%s 6", __func__);
//     num_1 = num_create_immed(1, 3);
//     num_2 = num_create_immed(1, 2);
//     cmp = num_cmp(num_1, num_2);
//     assert(cmp > 0);
//     num_free(num_1);
//     num_free(num_2);
//
//     if(show) printf("\n\t\t%s 7", __func__);
//     num_1 = num_create_immed(2, 2, 0);
//     num_2 = num_create_immed(2, 1, 0);
//     cmp = num_cmp(num_1, num_2);
//     assert(cmp > 0);
//     num_free(num_1);
//     num_free(num_2);
//
//     if(show) printf("\n\t\t%s 8", __func__);
//     num_1 = num_create_immed(2, 2, 0);
//     num_2 = num_create_immed(2, 2, 0);
//     cmp = num_cmp(num_1, num_2);
//     assert(cmp == 0);
//     num_free(num_1);
//     num_free(num_2);
//
//     if(show) printf("\n\t\t%s 9", __func__);
//     num_1 = num_create_immed(2, 2, 0);
//     num_2 = num_create_immed(2, 3, 0);
//     cmp = num_cmp(num_1, num_2);
//     assert(cmp < 0);
//     num_free(num_1);
//     num_free(num_2);
//
//     assert(clu_mem_empty());
// }



// void test_num_add(bool show)
// {
//     printf("\n\t%s", __func__);
//
//     if(show) printf("\n\t\t%s 1", __func__);
//     num_p num_res = num_add(NULL, NULL);
//     assert(num_immed(num_res, 0));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 2", __func__);
//     num_p num_1 = num_create_immed(1, 1);
//     num_res = num_add(num_1, NULL);
//     assert(num_immed(num_res, 1, 1));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 3", __func__);
//     num_p num_2 = num_create_immed(1, 1);
//     num_res = num_add(NULL, num_2);
//     assert(num_immed(num_res, 1, 1));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 4", __func__);
//     num_1 = num_create_immed(1, 1);
//     num_2 = num_create_immed(1, 2);
//     num_res = num_add(num_1, num_2);
//     assert(num_immed(num_res, 1, 3));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 5", __func__);
//     num_1 = num_create_immed(2, 2, 1);
//     num_2 = num_create_immed(1, 2);
//     num_res = num_add(num_1, num_2);
//     assert(num_immed(num_res, 2, 2, 3));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 6", __func__);
//     num_1 = num_create_immed(1, 1);
//     num_2 = num_create_immed(2, 2, 3);
//     num_res = num_add(num_1, num_2);
//     assert(num_immed(num_res, 2, 2, 4));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 7", __func__);
//     num_1 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
//     num_2 = num_create_immed(1, 1);
//     num_res = num_add(num_1, num_2);
//     assert(num_immed(num_res, 3, 1, 0, 0));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 8", __func__);
//     num_1 = num_create_immed(1, 1);
//     num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
//     num_res = num_add(num_1, num_2);
//     assert(num_immed(num_res, 3, 1, 0, 0));
//     num_free(num_res);
//
//     assert(clu_mem_empty());
// }

// void test_num_sub(bool show)
// {
//     printf("\n\t%s", __func__);
//
//     if(show) printf("\n\t\t%s 1", __func__);
//     num_p num_res = num_sub(NULL, NULL);
//     assert(num_immed(num_res, 0));
//
//     if(show) printf("\n\t\t%s 2", __func__);
//     num_p num_1 = num_create_immed(1, 1);
//     num_res = num_sub(num_1, NULL);
//     assert(num_immed(num_res, 1, 1));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 3", __func__);
//     num_1 = num_create_immed(1, 2);
//     num_p num_2 = num_create_immed(1, 1);
//     num_res = num_sub(num_1, num_2);
//     assert(num_immed(num_res, 1, 1));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 3", __func__);
//     num_1 = num_create_immed(1, 2);
//     num_2 = num_create_immed(1, 2);
//     num_res = num_sub(num_1, num_2);
//     assert(num_immed(num_res, 0));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 4", __func__);
//     num_1 = num_create_immed(2, 1, 0);
//     num_2 = num_create_immed(1, 1);
//     num_res = num_sub(num_1, num_2);
//     assert(num_immed(num_res, 1, UINT64_MAX));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 5", __func__);
//     num_1 = num_create_immed(2, 4, 3);
//     num_2 = num_create_immed(2, 1, 2);
//     num_res = num_sub(num_1, num_2);
//     assert(num_immed(num_res, 2, 3, 1));
//     num_free(num_res);
//
//     if(show) printf("\n\t\t%s 6", __func__);
//     num_1 = num_create_immed(2, 1, 0);
//     num_2 = num_create_immed(2, 1, 0);
//     num_res = num_sub(num_1, num_2);
//     assert(num_immed(num_res, 0));
//     num_free(num_res);
//
//     assert(clu_mem_empty());
// }

// void test_num_mul(bool show)
// {
//     printf("\n\t%s", __func__);
//
//     bool test_mem = true;
//
//     if(show) printf("\n\t\t%s  1", __func__);
//     num_p num_res = num_mul(NULL, NULL);
//     assert(num_immed(num_res, 0));
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s  2", __func__);
//     num_p num_1 = num_create_immed(1, 1);
//     num_res = num_mul(num_1, NULL);
//     assert(num_immed(num_res, 0));
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s  3", __func__);
//     num_p num_2 = num_create_immed(1, 1);
//     num_res = num_mul(NULL, num_2);
//     assert(num_immed(num_res, 0));
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s  4", __func__);
//     num_1 = num_create_immed(1, 2);
//     num_2 = num_create_immed(1, 3);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 1, 6));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s  5", __func__);
//     num_1 = num_create_immed(1, UINT64_MAX);
//     num_2 = num_create_immed(1, UINT64_MAX);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 2, UINT64_MAX - 1, 1));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s  6", __func__);
//     num_1 = num_create_immed(2, 2, 3);
//     num_2 = num_create_immed(1, 4);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 2, 8, 12));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s  7", __func__);
//     num_1 = num_create_immed(1, 2);
//     num_2 = num_create_immed(2, 3, 4);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 2, 6, 8));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s  8", __func__);
//     num_1 = num_create_immed(2, 2, 3);
//     num_2 = num_create_immed(2, 4, 5);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 3, 8, 22, 15));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s  9", __func__);
//     num_1 = num_create_immed(2, 1, 0);
//     num_2 = num_create_immed(1, 1);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 2, 1, 0));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s 10", __func__);
//     num_1 = num_create_immed(1, 1);
//     num_2 = num_create_immed(2, 1, 0);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 2, 1, 0));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s 11", __func__);
//     num_1 = num_create_immed(2, 1, 0);
//     num_2 = num_create_immed(2, 1, 0);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 3, 1, 0, 0));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s 12", __func__);
//     num_1 = num_create_immed(1, 2);
//     num_2 = num_create_immed(2, 2, UINT64_MAX);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 2, 5, UINT64_MAX - 1));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s 13", __func__);
//     num_1 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
//     num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 4, UINT64_MAX, UINT64_MAX - 1, 0, 1));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s 14", __func__);
//     num_1 = num_create_immed(3, 2, 3, 4);
//     num_2 = num_create_immed(3, 5, 6, 7);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 5, 10, 27, 52, 45, 28));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     if(show) printf("\n\t\t%s 14", __func__);
//     num_1 = num_create_immed(3, 2, 3, 4);
//     num_2 = num_create_immed(3, 5, 6, 7);
//     num_res = num_mul(num_1, num_2);
//     assert(num_immed(num_res, 5, 10, 27, 52, 45, 28));
//     num_free(num_res);
//     if(test_mem) assert(clu_mem_empty());
//
//     assert(clu_mem_empty());
// }

// void test_num_div_mod(bool show)
// {
//     printf("\n\t%s", __func__);
//
//     num_p num_q, num_r;
//
//     if(show) printf("\n\t\t%s 1", __func__);
//     num_p num_2 = num_create_immed(1, 1);
//     num_div_mod(&num_q, &num_r, NULL, num_2);
//     assert(num_immed(num_q, 0));
//     assert(num_immed(num_r, 0));
//
//     if(show) printf("\n\t\t%s 2", __func__);
//     num_p num_1 = num_create_immed(1, 4);
//     num_2 = num_create_immed(1, 2);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 1, 2));
//     assert(num_immed(num_r, 0));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 3", __func__);
//     num_1 = num_create_immed(1, 5);
//     num_2 = num_create_immed(1, 2);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 1, 2));
//     assert(num_immed(num_r, 1, 1));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 4", __func__);
//     num_1 = num_create_immed(1, 5);
//     num_2 = num_create_immed(1, 5);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 1, 1));
//     assert(num_immed(num_r, 0));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 5", __func__);
//     num_1 = num_create_immed(1, 9);
//     num_2 = num_create_immed(1, 3);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 1, 3));
//     assert(num_immed(num_r, 0));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 6", __func__);
//     num_1 = num_create_immed(3, 1, 0, 0);
//     num_2 = num_create_immed(2, 1, 0);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 2, 1, 0));
//     assert(num_immed(num_r, 0));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 7", __func__);
//     num_1 = num_create_immed(1, 1);
//     num_2 = num_create_immed(2, 1, 0);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 0));
//     assert(num_immed(num_r, 1, 1));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 8", __func__);
//     num_1 = num_create_immed(2, 4, UINT64_MAX);
//     num_2 = num_create_immed(2, 2, 0);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 1, 2));
//     assert(num_immed(num_r, 1, UINT64_MAX));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 9", __func__);
//     num_1 = num_create_immed(2, 4, 0);
//     num_2 = num_create_immed(2, 2, UINT64_MAX);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 1, 1));
//     assert(num_immed(num_r, 2, 1, 1));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 10", __func__);
//     num_1 = num_create_immed(2, 1, 0);
//     num_2 = num_create_immed(1, UINT64_MAX);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 1, 1));
//     assert(num_immed(num_r, 1, 1));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 11", __func__);
//     num_1 = num_create_immed(2, UINT64_MAX, 0);
//     num_2 = num_create_immed(2, 1, UINT64_MAX);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 1, 0x7fffffffffffffff));
//     assert(num_immed(num_r, 2, 1, 0x7fffffffffffffff));
//     num_free(num_q);
//     num_free(num_r);
//
//     if(show) printf("\n\t\t%s 12", __func__);
//     num_1 = num_create_immed(2, 0xc929d7d593, 0xb7090a859117cfa4);
//     num_2 = num_create_immed(2, 6, 0xea7db545decb57a4);
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     assert(num_immed(num_q, 1, 0x0000001d1635b735));
//     assert(num_immed(num_r, 1, 0x88c80995d8646eb0));
//     num_free(num_q);
//     num_free(num_r);
//
//     assert(clu_mem_empty());
// }



void test_num()
{
    printf("\n%s", __func__);

    bool show = true;

    test_uint128(show);

    test_node_create(show);
    test_node_consume(show);
    test_node_denormalize(show);
    test_node_normalize(show);

    test_num_wrap(show);
    test_num_create_immed(show);
    // test_num_copy(show);

    test_num_add_uint(show);
    test_num_sub_uint(show);
    test_num_mul_uint(show);
    // test_num_shl(show);
    // test_num_shr(show);

    // test_num_cmp(show);

    // test_num_add(show);
    // test_num_sub(show);
    // test_num_mul(show);
    // test_num_div_mod(show);

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_num();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
