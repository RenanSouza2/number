#include "../debug.h"
#include "../../../utils/U64.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_num_create(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create(1, NULL);
    assert(num->value == 1);
    assert(num->next  == NULL);
    free(num);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create(2, (num_p)1);
    assert(num->value == 2);
    assert(num->next  == (num_p)1);
    free(num);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create(0, NULL);
    assert(num->value == 0);
    assert(num->next  == NULL);
    free(num);

    assert(clu_mem_empty());
}

void test_num_wrap(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_wrap(0);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_wrap(1);
    assert(num_immed(num, 1, 1));
    num_free(num);

    assert(clu_mem_empty());
}

void test_num_create_immed(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    assert(num == NULL);
    num_free(num);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(1, 1);
    assert(num->value == 1);
    assert(num->next  == NULL);
    num_free(num);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(2, 2, 1);
    assert(num->value == 1);
    assert(num->next  != NULL);
    assert(num->next->value == 2);
    assert(num->next->next  == NULL);
    num_free(num);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(2, 2, 0);
    assert(num->value == 0);
    assert(num->next  != NULL);
    assert(num->next->value == 2);
    assert(num->next->next  == NULL);
    num_free(num);

    assert(clu_mem_empty());
}

void test_num_consume(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_res = num_consume(NULL, false);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num_p num = num_create_immed(1, 1);
    num_res = num_consume(num, false);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(2, 1, 2);
    num_res = num_consume(num, false);
    assert(num_immed(num_res, 1, 1));
    num_free(num_res);

    if(show) printf("\n\t\t%s 4", __func__);
    num_res = num_consume(NULL, true);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 5", __func__);
    num = num_create_immed(1, 1);
    num_res = num_consume(num, true);
    assert(num_immed(num_res, 0));
    num_free(num);

    if(show) printf("\n\t\t%s 6", __func__);
    num = num_create_immed(2, 1, 2);
    num_res = num_consume(num, true);
    assert(num_immed(num_res, 1, 1));
    num_free(num);

    assert(clu_mem_empty());
}

void test_num_denormalize(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_denormalize(NULL);
    assert(num_immed(num, 1, 0));
    num_free(num);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create(1, NULL);
    num = num_denormalize(num);
    assert(num_immed(num, 1, 1));
    num_free(num);

    assert(clu_mem_empty());
}

void test_num_normalize(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_normalize(NULL);
    assert(num == NULL);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(1, 1);
    num = num_normalize(num);
    assert(num_immed(num, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(2, 1, 0);
    num = num_normalize(num);
    assert(num_immed(num, 2, 1, 0));
    num_free(num);

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 0);
    num = num_normalize(num);
    assert(num_immed(num, 0));

    assert(clu_mem_empty());
}

void test_num_copy(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_res = num_copy(NULL);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num_p num = num_create_immed(1, 1);
    num_res = num_copy(num);
    assert(num_immed(num_res, 1, 1));
    num_free(num);
    num_free(num_res);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(2, 1, 2);
    num_res = num_copy(num);
    assert(num_immed(num_res, 2, 1, 2));
    num_free(num);
    num_free(num_res);

    assert(clu_mem_empty());
}



void test_num_add_uint(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_res = num_add_uint(NULL, 0);
    assert(num_immed(num_res, 0));
    num_free(num_res);

    if(show) printf("\n\t\t%s 2", __func__);
    num_res = num_add_uint(NULL, 1);
    assert(num_immed(num_res, 1, 1));
    num_free(num_res);

    if(show) printf("\n\t\t%s 3", __func__);
    num_p num = num_create_immed(1, 1);
    num_res = num_add_uint(num, 2);
    assert(num_immed(num_res, 1, 3));
    num_free(num_res);

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, UINT64_MAX);
    num_res = num_add_uint(num, 3);
    assert(num_immed(num_res, 2, 1, 2));
    num_free(num_res);

    assert(clu_mem_empty());
}

void test_num_sub_uint(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_res = num_sub_uint(NULL, 0);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num_p num = num_create_immed(1, 1);
    num_res = num_sub_uint(num, 0);
    assert(num_immed(num_res, 1, 1));
    num_free(num_res);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 1);
    num_res = num_sub_uint(num, 1);
    assert(num_immed(num_res, 0));
    num_free(num_res);

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 2);
    num_res = num_sub_uint(num, 1);
    assert(num_immed(num_res, 1, 1));
    num_free(num_res);

    if(show) printf("\n\t\t%s 5", __func__);
    num = num_create_immed(2, 1, 0);
    num_res = num_sub_uint(num, 1);
    assert(num_immed(num_res, 1, UINT64_MAX));
    num_free(num_res);

    assert(clu_mem_empty());
}

void test_num_mul_uint(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_mul_uint(NULL, NULL, 0);
    assert(num == NULL);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_mul_uint(NULL, NULL, 1);
    assert(num == NULL);
    
    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 2);
    num_p num_res = num_mul_uint(NULL, num, 0);
    assert(num_immed(num_res, 0));
    num_free(num);
    num_free(num_res);
    
    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, 3);
    assert(num_immed(num_res, 1, 6));
    num_free(num);
    num_free(num_res);
    
    if(show) printf("\n\t\t%s 5", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, UINT64_MAX);
    assert(num_immed(num_res, 2, 1, UINT64_MAX - 1));
    num_free(num);
    num_free(num_res);
    
    if(show) printf("\n\t\t%s 6", __func__);
    num = num_create_immed(2, 2, 3);
    num_res = num_mul_uint(NULL, num, 4);
    assert(num_immed(num_res, 2, 8, 12));
    num_free(num);
    num_free(num_res);
    
    if(show) printf("\n\t\t%s 7", __func__);
    num = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_mul_uint(NULL, num, UINT64_MAX);
    assert(num_immed(num_res, 3, UINT64_MAX - 1, UINT64_MAX, 1));
    num_free(num);
    num_free(num_res);
    
    if(show) printf("\n\t\t%s 8", __func__);
    num = num_create_immed(2, 2, 0);
    num_res = num_mul_uint(NULL, num, 3);
    assert(num_immed(num_res, 2, 6, 0));
    num_free(num);
    num_free(num_res);

    assert(clu_mem_empty());
}

void test_num_shl(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_res = num_shl(NULL);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num_p num = num_create_immed(1, 1);
    num_res = num_shl(num);
    assert(num_immed(num_res, 1, 2));
    num_free(num);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 3);
    num_res = num_shl(num);
    assert(num_immed(num_res, 1, 6));
    num_free(num);

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, UINT64_MAX);
    num_res = num_shl(num);
    assert(num_immed(num_res, 2, 1, UINT64_MAX - 1));
    num_free(num);

    if(show) printf("\n\t\t%s 5", __func__);
    num = num_create_immed(2, 1, 2);
    num_res = num_shl(num);
    assert(num_immed(num_res, 2, 2, 4));
    num_free(num);

    if(show) printf("\n\t\t%s 6", __func__);
    num = num_create_immed(2, 1, UINT64_MAX);
    num_res = num_shl(num);
    assert(num_immed(num_res, 2, 3, UINT64_MAX - 1));
    num_free(num);

    assert(clu_mem_empty());
}

void test_num_shr(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_res = num_shr(NULL);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num_p num = num_create_immed(1, 2);
    num_res = num_shr(num);
    assert(num_immed(num_res, 1, 1));
    num_free(num_res);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 1);
    num_res = num_shr(num);
    assert(num_immed(num_res, 0));
    num_free(num_res);

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(2, 1, 0);
    num_res = num_shr(num);
    assert(num_immed(num_res, 1, 0x8000000000000000));
    num_free(num_res);

    if(show) printf("\n\t\t%s 5", __func__);
    num = num_create_immed(2, 2, 0);
    num_res = num_shr(num);
    assert(num_immed(num_res, 2, 1, 0));
    num_free(num_res);

    if(show) printf("\n\t\t%s 6", __func__);
    num = num_create_immed(2, 1, 2);
    num_res = num_shr(num);
    assert(num_immed(num_res, 1, 0x8000000000000001));
    num_free(num_res);

    if(show) printf("\n\t\t%s 7", __func__);
    num = num_create_immed(2, 2, 2);
    num_res = num_shr(num);
    assert(num_immed(num_res, 2, 1, 1));
    num_free(num_res);

    assert(clu_mem_empty());
}



void test_num_cmp(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    int64_t cmp = num_cmp(NULL, NULL);
    assert(cmp == 0);

    if(show) printf("\n\t\t%s 2", __func__);
    num_p num_1 = num_create_immed(1, 1);
    cmp = num_cmp(num_1, NULL);
    assert(cmp > 0);
    num_free(num_1);

    if(show) printf("\n\t\t%s 3", __func__);
    num_p num_2 = num_create_immed(1, 1);
    cmp = num_cmp(NULL, num_2);
    assert(cmp < 0);
    num_free(num_2);

    if(show) printf("\n\t\t%s 4", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(1, 2);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s 5", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 2);
    cmp = num_cmp(num_1, num_2);
    assert(cmp == 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s 6", __func__);
    num_1 = num_create_immed(1, 3);
    num_2 = num_create_immed(1, 2);
    cmp = num_cmp(num_1, num_2);
    assert(cmp > 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s 7", __func__);
    num_1 = num_create_immed(2, 2, 0);
    num_2 = num_create_immed(2, 1, 0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp > 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s 8", __func__);
    num_1 = num_create_immed(2, 2, 0);
    num_2 = num_create_immed(2, 2, 0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp == 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s 9", __func__);
    num_1 = num_create_immed(2, 2, 0);
    num_2 = num_create_immed(2, 3, 0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    assert(clu_mem_empty());
}



void test_num_add(bool show)
{
    printf("\n\t%s", __func__);

    for(uint64_t keep = 0; keep < 2; keep++)
    {
        if(show) printf("\n\n\t\t%s | keep: " U64P "", __func__, keep);

        if(show) printf("\n\t\t\t%s 1", __func__);
        num_p num_res = num_add(NULL, NULL, keep);
        assert(num_immed(num_res, 0));
        num_free(num_res);
        
        if(show) printf("\n\t\t\t%s 2", __func__);
        num_p num_1 = num_create_immed(1, 1);
        num_res = num_add(num_1, NULL, keep);
        assert(num_immed(num_res, 1, 1));
        num_free(num_res);
        
        if(show) printf("\n\t\t\t%s 3", __func__);
        num_p num_2 = num_create_immed(1, 1);
        num_res = num_add(NULL, num_2, keep);
        assert(num_immed(num_res, 1, 1));
        num_free(num_res);
        if(keep) num_free(num_2);
        
        if(show) printf("\n\t\t\t%s 4", __func__);
        num_1 = num_create_immed(1, 1);
        num_2 = num_create_immed(1, 2);
        num_res = num_add(num_1, num_2, keep);
        assert(num_immed(num_res, 1, 3));
        num_free(num_res);
        if(keep) num_free(num_2);
        
        if(show) printf("\n\t\t\t%s 5", __func__);
        num_1 = num_create_immed(2, 2, 1);
        num_2 = num_create_immed(1, 2);
        num_res = num_add(num_1, num_2, keep);
        assert(num_immed(num_res, 2, 2, 3));
        num_free(num_res);
        if(keep) num_free(num_2);
        
        if(show) printf("\n\t\t\t%s 6", __func__);
        num_1 = num_create_immed(1, 1);
        num_2 = num_create_immed(2, 2, 3);
        num_res = num_add(num_1, num_2, keep);
        assert(num_immed(num_res, 2, 2, 4));
        num_free(num_res);
        if(keep) num_free(num_2);
        
        if(show) printf("\n\t\t\t%s 7", __func__);
        num_1 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
        num_2 = num_create_immed(1, 1);
        num_res = num_add(num_1, num_2, keep);
        assert(num_immed(num_res, 3, 1, 0, 0));
        num_free(num_res);
        if(keep) num_free(num_2);
        
        if(show) printf("\n\t\t\t%s 8", __func__);
        num_1 = num_create_immed(1, 1);
        num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
        num_res = num_add(num_1, num_2, keep);
        assert(num_immed(num_res, 3, 1, 0, 0));
        num_free(num_res);
        if(keep) num_free(num_2);
    }

    assert(clu_mem_empty());
}

void test_num_sub(bool show)
{
    printf("\n\t%s", __func__);

    for(uint64_t keep = 0; keep < 2; keep++)
    {        
        if(show) printf("\n\n\t\t%s | keep: " U64P "", __func__, keep);

        if(show) printf("\n\t\t\t%s 1", __func__);
        num_p num_res = num_sub(NULL, NULL, keep);
        assert(num_immed(num_res, 0));

        if(show) printf("\n\t\t\t%s 2", __func__);
        num_p num_1 = num_create_immed(1, 1);
        num_res = num_sub(num_1, NULL, keep);
        assert(num_immed(num_res, 1, 1));
        num_free(num_res);

        if(show) printf("\n\t\t\t%s 3", __func__);
        num_1 = num_create_immed(1, 2);
        num_p num_2 = num_create_immed(1, 1);
        num_res = num_sub(num_1, num_2, keep);
        assert(num_immed(num_res, 1, 1));
        num_free(num_res);
        if(keep) num_free(num_2);

        if(show) printf("\n\t\t\t%s 3", __func__);
        num_1 = num_create_immed(1, 2);
        num_2 = num_create_immed(1, 2);
        num_res = num_sub(num_1, num_2, keep);
        assert(num_immed(num_res, 0));
        num_free(num_res);
        if(keep) num_free(num_2);

        if(show) printf("\n\t\t\t%s 4", __func__);
        num_1 = num_create_immed(2, 1, 0);
        num_2 = num_create_immed(1, 1);
        num_res = num_sub(num_1, num_2, keep);
        assert(num_immed(num_res, 1, UINT64_MAX));
        num_free(num_res);
        if(keep) num_free(num_2);

        if(show) printf("\n\t\t\t%s 5", __func__);
        num_1 = num_create_immed(2, 4, 3);
        num_2 = num_create_immed(2, 1, 2);
        num_res = num_sub(num_1, num_2, keep);
        assert(num_immed(num_res, 2, 3, 1));
        num_free(num_res);
        if(keep) num_free(num_2);

        if(show) printf("\n\t\t\t%s 6", __func__);
        num_1 = num_create_immed(2, 1, 0);
        num_2 = num_create_immed(2, 1, 0);
        num_res = num_sub(num_1, num_2, keep);
        assert(num_immed(num_res, 0));
        num_free(num_res);
        if(keep) num_free(num_2);
    }

    assert(clu_mem_empty());
}

void test_num_mul(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_res = num_mul(NULL, NULL);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num_p num_1 = num_create_immed(1, 1);
    num_res = num_mul(num_1, NULL);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 3", __func__);
    num_p num_2 = num_create_immed(1, 1);
    num_res = num_mul(NULL, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 4", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 3);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 1, 6));
    num_free(num_res);

    if(show) printf("\n\t\t%s 5", __func__);
    num_1 = num_create_immed(1, UINT64_MAX);
    num_2 = num_create_immed(1, UINT64_MAX);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, UINT64_MAX - 1, 1));
    num_free(num_res);

    if(show) printf("\n\t\t%s 6", __func__);
    num_1 = num_create_immed(2, 2, 3);
    num_2 = num_create_immed(1, 4);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 8, 12));
    num_free(num_res);

    if(show) printf("\n\t\t%s 7", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(2, 3, 4);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 6, 8));
    num_free(num_res);

    if(show) printf("\n\t\t%s 8", __func__);
    num_1 = num_create_immed(2, 2, 3);
    num_2 = num_create_immed(2, 4, 5);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 3, 8, 22, 15));
    num_free(num_res);

    if(show) printf("\n\t\t%s 8", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(1, 1);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 1, 0));
    num_free(num_res);

    if(show) printf("\n\t\t%s 9", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, 1, 0);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 1, 0));
    num_free(num_res);

    if(show) printf("\n\t\t%s 10", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(2, 1, 0);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 3, 1, 0, 0));
    num_free(num_res);

    assert(clu_mem_empty());
}

void test_num_div(bool show)
{
    printf("\n\t%s", __func__);

    for(uint64_t keep_1 = 0; keep_1 < 2; keep_1++)
    {
        if(show) printf("\n\n\t\t%s | keep_1: " U64P "", __func__, keep_1);
        for(uint64_t keep_2 = 0; keep_2 < 2; keep_2++)
        {
            if(show) printf("\n\t\t\t%s | keep_2: " U64P "", __func__, keep_2);

            if(show) printf("\n\t\t\t\t%s 1", __func__);
            num_p num_2 = num_create_immed(1, 1);
            num_p num_res = num_div(NULL, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 0));
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t\t%s 2", __func__);
            num_p num_1 = num_create_immed(1, 4);
            num_2 = num_create_immed(1, 2);
            num_res = num_div(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 1, 2));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t\t%s 3", __func__);
            num_1 = num_create_immed(1, 5);
            num_2 = num_create_immed(1, 2);
            num_res = num_div(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 1, 2));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t\t%s 4", __func__);
            num_1 = num_create_immed(1, 5);
            num_2 = num_create_immed(1, 5);
            num_res = num_div(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 1, 1));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t\t%s 5", __func__);
            num_1 = num_create_immed(1, 9);
            num_2 = num_create_immed(1, 3);
            num_res = num_div(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 1, 3));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t\t%s 6", __func__);
            num_1 = num_create_immed(3, 1, 0, 0);
            num_2 = num_create_immed(2, 1, 0);
            num_res = num_div(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 2, 1, 0));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t\t%s 7", __func__);
            num_1 = num_create_immed(1, 1);
            num_2 = num_create_immed(2, 1, 0);
            num_res = num_div(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 0));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);
        }
    }

    assert(clu_mem_empty());
}

void test_num_mod(bool show)
{
    printf("\n\t%s", __func__);

    for(uint64_t keep_1 = 0; keep_1 < 2; keep_1++)
    {
        if(show) printf("\n\n\t\t%s | keep_1: " U64P "", __func__, keep_1);
        for(uint64_t keep_2 = 0; keep_2 < 2; keep_2++)
        {
            if(show) printf("\n\n\t\t%s | keep_2: " U64P "", __func__, keep_2);

            if(show) printf("\n\t\t\t%s 1", __func__);
            num_p num_2 = num_create_immed(1, 1);
            num_p num_res = num_mod(NULL, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 0));
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t%s 2", __func__);
            num_p num_1 = num_create_immed(1, 4);
            num_2 = num_create_immed(1, 2);
            num_res = num_mod(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 0));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t%s 3", __func__);
            num_1 = num_create_immed(1, 5);
            num_2 = num_create_immed(1, 2);
            num_res = num_mod(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 1, 1));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t%s 4", __func__);
            num_1 = num_create_immed(1, 5);
            num_2 = num_create_immed(1, 5);
            num_res = num_mod(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 0));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t%s 5", __func__);
            num_1 = num_create_immed(1, 9);
            num_2 = num_create_immed(1, 3);
            num_res = num_mod(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 0));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t%s 6", __func__);
            num_1 = num_create_immed(3, 1, 0, 0);
            num_2 = num_create_immed(2, 1, 0);
            num_res = num_mod(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 0));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);

            if(show) printf("\n\t\t\t%s 7", __func__);
            num_1 = num_create_immed(1, 1);
            num_2 = num_create_immed(2, 1, 0);
            num_res = num_mod(num_1, keep_1, num_2, keep_2);
            assert(num_immed(num_res, 1, 1));
            num_free(num_res);
            if(keep_1) num_free(num_1);
            if(keep_2) num_free(num_2);
        }
    }

    assert(clu_mem_empty());
}



void test_num()
{
    printf("\n%s", __func__);

    bool show = false;

    test_num_create(show);
    test_num_wrap(show);
    test_num_create_immed(show);
    test_num_consume(show);
    test_num_denormalize(show);
    test_num_normalize(show);
    test_num_copy(show);

    test_num_add_uint(show);
    test_num_sub_uint(show);
    test_num_mul_uint(show);
    test_num_shl(show);
    test_num_shr(show);
    
    test_num_cmp(show);

    test_num_add(show);
    test_num_sub(show);
    test_num_mul(show);
    test_num_div(show);
    test_num_mod(show);

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_num();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
