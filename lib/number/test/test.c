#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_number_create()
{
    printf("\n\t%s", __func__);

    number_p num = number_create(1, NULL);
    assert(num->value == 1);
    assert(num->next  == NULL);
    free(num);

    num = number_create(2, (number_p)1);
    assert(num->value == 2);
    assert(num->next  == (number_p)1);
    free(num);

    assert(clu_mem_empty());
}

void test_number_create_immed()
{
    printf("\n\t%s", __func__);

    number_p num = number_create_immed(0);
    assert(num == NULL);
    number_free(num);

    num = number_create_immed(1, 1);
    assert(num->value == 1);
    assert(num->next  == NULL);
    number_free(num);

    num = number_create_immed(2, 2, 1);
    assert(num->value == 1);
    assert(num->next  != NULL);
    assert(num->next->value == 2);
    assert(num->next->next  == NULL);
    number_free(num);

    assert(clu_mem_empty());
}



void test_number_add_uint()
{
    printf("\n\t%s", __func__);

    number_p num = NULL;
    num = number_add_uint(num, 0);
    assert(number_immed(num, 0));
    number_free(num);

    num = NULL;
    num = number_add_uint(num, 1);
    assert(number_immed(num, 1, 1));
    number_free(num);

    num = number_create_immed(1, 1);
    num = number_add_uint(num, 2);
    assert(number_immed(num, 1, 3));
    number_free(num);

    num = number_create_immed(1, UINT64_MAX);
    num = number_add_uint(num, 3);
    assert(number_immed(num, 2, 1, 2));
    number_free(num);

    assert(clu_mem_empty());
}

void test_number_add(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    number_p num_1, num_2;
    num_1 = NULL;
    num_2 = NULL;
    num_1 = number_add(num_1, num_2);
    assert(number_immed(num_1, 0));
    number_free(num_1);
    
    if(show) printf("\n\t\t%s 2", __func__);
    num_1 = NULL;
    num_2 = number_create_immed(1, 1);
    num_1 = number_add(num_1, num_2);
    assert(number_immed(num_1, 1, 1));
    number_free(num_1);
    
    if(show) printf("\n\t\t%s 3", __func__);
    num_1 = number_create_immed(1, 1);
    num_2 = NULL;
    num_1 = number_add(num_1, num_2);
    assert(number_immed(num_1, 1, 1));
    number_free(num_1);
    
    if(show) printf("\n\t\t%s 4", __func__);
    num_1 = number_create_immed(1, 1);
    num_2 = number_create_immed(1, 2);
    num_1 = number_add(num_1, num_2);
    assert(number_immed(num_1, 1, 3));
    number_free(num_1);
    
    if(show) printf("\n\t\t%s 5", __func__);
    num_1 = number_create_immed(2, 2, 1);
    num_2 = number_create_immed(1, 2);
    num_1 = number_add(num_1, num_2);
    assert(number_immed(num_1, 2, 2, 3));
    number_free(num_1);
    
    if(show) printf("\n\t\t%s 6", __func__);
    num_1 = number_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_2 = number_create_immed(1, 1);
    num_1 = number_add(num_1, num_2);
    assert(number_immed(num_1, 3, 1, 0, 0));
    number_free(num_1);

    assert(clu_mem_empty());
}



void test_number()
{
    printf("\n%s", __func__);

    test_number_create();
    test_number_create_immed();
    test_number_add_uint();
    test_number_add(false);

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_number();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
