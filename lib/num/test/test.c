#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_num_create()
{
    printf("\n\t%s", __func__);

    num_p num = num_create(1, NULL);
    assert(num->value == 1);
    assert(num->next  == NULL);
    free(num);

    num = num_create(2, (num_p)1);
    assert(num->value == 2);
    assert(num->next  == (num_p)1);
    free(num);

    assert(clu_mem_empty());
}

void test_num_create_immed()
{
    printf("\n\t%s", __func__);

    num_p num = num_create_immed(0);
    assert(num == NULL);
    num_free(num);

    num = num_create_immed(1, 1);
    assert(num->value == 1);
    assert(num->next  == NULL);
    num_free(num);

    num = num_create_immed(2, 2, 1);
    assert(num->value == 1);
    assert(num->next  != NULL);
    assert(num->next->value == 2);
    assert(num->next->next  == NULL);
    num_free(num);

    num = num_create_immed(2, 2, 0);
    assert(num->value == 0);
    assert(num->next  != NULL);
    assert(num->next->value == 2);
    assert(num->next->next  == NULL);
    num_free(num);

    assert(clu_mem_empty());
}

void test_num_consume()
{
    printf("\n\t%s", __func__);

    num_p num = num_consume(NULL);
    assert(num == NULL);

    num = num_create(1, NULL);
    num = num_consume(num);
    assert(num == NULL);

    assert(clu_mem_empty());
}

void test_num_denormalize()
{
    printf("\n\t%s", __func__);

    num_p num = num_denormalize(NULL);
    assert(num_immed(num, 1, 0));
    num_free(num);

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



void test_num_add_uint()
{
    printf("\n\t%s", __func__);

    num_p num = NULL;
    num = num_add_uint(num, 0);
    assert(num_immed(num, 0));
    num_free(num);

    num = NULL;
    num = num_add_uint(num, 1);
    assert(num_immed(num, 1, 1));
    num_free(num);

    num = num_create_immed(1, 1);
    num = num_add_uint(num, 2);
    assert(num_immed(num, 1, 3));
    num_free(num);

    num = num_create_immed(1, UINT64_MAX);
    num = num_add_uint(num, 3);
    assert(num_immed(num, 2, 1, 2));
    num_free(num);

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
    assert(num_immed(num_res, 0));

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



void test_num_add(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_1, num_2;
    num_1 = NULL;
    num_2 = NULL;
    num_1 = num_add(num_1, num_2);
    assert(num_immed(num_1, 0));
    num_free(num_1);
    
    if(show) printf("\n\t\t%s 2", __func__);
    num_1 = NULL;
    num_2 = num_create_immed(1, 1);
    num_1 = num_add(num_1, num_2);
    assert(num_immed(num_1, 1, 1));
    num_free(num_1);
    
    if(show) printf("\n\t\t%s 3", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = NULL;
    num_1 = num_add(num_1, num_2);
    assert(num_immed(num_1, 1, 1));
    num_free(num_1);
    
    if(show) printf("\n\t\t%s 4", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(1, 2);
    num_1 = num_add(num_1, num_2);
    assert(num_immed(num_1, 1, 3));
    num_free(num_1);
    
    if(show) printf("\n\t\t%s 5", __func__);
    num_1 = num_create_immed(2, 2, 1);
    num_2 = num_create_immed(1, 2);
    num_1 = num_add(num_1, num_2);
    assert(num_immed(num_1, 2, 2, 3));
    num_free(num_1);
    
    if(show) printf("\n\t\t%s 6", __func__);
    num_1 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_2 = num_create_immed(1, 1);
    num_1 = num_add(num_1, num_2);
    assert(num_immed(num_1, 3, 1, 0, 0));
    num_free(num_1);
    
    if(show) printf("\n\t\t%s 7", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_1 = num_add(num_1, num_2);
    assert(num_immed(num_1, 3, 1, 0, 0));
    num_free(num_1);

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



void test_num()
{
    printf("\n%s", __func__);

    test_num_create();
    test_num_create_immed();
    test_num_consume();
    test_num_denormalize();
    test_num_normalize(true);

    test_num_add_uint();
    test_num_sub_uint(true);
    test_num_mul_uint(false);

    test_num_add(false);
    test_num_mul(false);

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_num();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
