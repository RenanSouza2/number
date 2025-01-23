#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_number_create()
{
    printf("\n\t%s", __func__);

    number_p num = number_create(1);
    free(num);

    assert(clu_mem_empty());
}

void test_number_add()
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

    num = number_create(1);
    num = number_add_uint(num, 2);
    assert(number_immed(num, 1, 3));
    number_free(num);

    num = number_create(UINT64_MAX);
    num = number_add_uint(num, 3);
    assert(number_immed(num, 2, 2, 1));
    number_free(num);

    assert(clu_mem_empty());
}



void test_number()
{
    printf("\n%s", __func__);

    test_number_create();
    test_number_add();

    assert(clu_mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_number();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
