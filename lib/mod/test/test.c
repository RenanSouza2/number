#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_mod_create(bool show)
{
    char offset[] = "\t";
    printf("\n%s%s", offset, __func__);

    num_p p = num_wrap(7);

    if(show) printf("\n%s\t%s 1", offset, __func__);
    mod_t mod = mod_create(NULL, p);
    assert(mod.num == NULL);
    assert(mod.p == p);

    if(show) printf("\n%s\t%s 2", offset, __func__);
    mod = mod_create((num_p)1, p);
    assert(mod.num == (num_p)1);
    assert(mod.p == p);

    num_free(p);

    assert(clu_mem_empty());
}

void test_mod_wrap(bool show)
{
    char offset[] = "\t";
    printf("\n%s%s", offset, __func__);

    num_p p = num_wrap(7);

    if(show) printf("\n%s\t%s 1", offset, __func__);
    mod_t mod = mod_wrap(0, p);
    assert(mod_immed(mod, 0));
    mod_free(mod);

    if(show) printf("\n%s\t%s 2", offset, __func__);
    mod = mod_wrap(2, p);
    assert(mod_immed(mod, 1, 2));
    mod_free(mod);

    num_free(p);

    assert(clu_mem_empty());
}



void test_mod_add(bool show)
{
    char offset[] = "\t";
    printf("\n%s%s", offset, __func__);

    num_p p = num_wrap(7);

    if(show) printf("\n%s\t%s 1", offset, __func__);
    mod_t mod_1 = mod_wrap(0, p);
    mod_t mod_2 = mod_wrap(0, p);
    mod_t mod_res = mod_add(mod_1, mod_2);
    assert(mod_immed(mod_res, 0));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 2", offset, __func__);
    mod_1 = mod_wrap(1, p);
    mod_2 = mod_wrap(2, p);
    mod_res = mod_add(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 3));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 3", offset, __func__);
    mod_1 = mod_wrap(1, p);
    mod_2 = mod_wrap(6, p);
    mod_res = mod_add(mod_1, mod_2);
    assert(mod_immed(mod_res, 0));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 4", offset, __func__);
    mod_1 = mod_wrap(6, p);
    mod_2 = mod_wrap(6, p);
    mod_res = mod_add(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 5));
    mod_free(mod_res);

    num_free(p);

    assert(clu_mem_empty());
}

void test_mod_sub(bool show)
{
    char offset[] = "\t";
    printf("\n%s%s", offset, __func__);

    num_p p = num_wrap(7);

    if(show) printf("\n%s\t%s 1", offset, __func__);
    mod_t mod_1 = mod_wrap(0, p);
    mod_t mod_2 = mod_wrap(0, p);
    mod_t mod_res = mod_sub(mod_1, mod_2);
    assert(mod_immed(mod_res, 0));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 2", offset, __func__);
    mod_1 = mod_wrap(0, p);
    mod_2 = mod_wrap(1, p);
    mod_res = mod_sub(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 6));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 3", offset, __func__);
    mod_1 = mod_wrap(2, p);
    mod_2 = mod_wrap(0, p);
    mod_res = mod_sub(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 2));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 4", offset, __func__);
    mod_1 = mod_wrap(3, p);
    mod_2 = mod_wrap(2, p);
    mod_res = mod_sub(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 1));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 5", offset, __func__);
    mod_1 = mod_wrap(2, p);
    mod_2 = mod_wrap(5, p);
    mod_res = mod_sub(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 4));
    mod_free(mod_res);

    num_free(p);

    assert(clu_mem_empty());
}

void test_mod_mul(bool show)
{
    char offset[] = "\t";
    printf("\n%s%s", offset, __func__);

    num_p p = num_wrap(7);

    if(show) printf("\n%s\t%s 1", offset, __func__);
    mod_t mod_1 = mod_wrap(0, p);
    mod_t mod_2 = mod_wrap(0, p);
    mod_t mod_res = mod_mul(mod_1, mod_2);
    assert(mod_immed(mod_res, 0));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 2", offset, __func__);
    mod_1 = mod_wrap(0, p);
    mod_2 = mod_wrap(1, p);
    mod_res = mod_mul(mod_1, mod_2);
    assert(mod_immed(mod_res,0));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 3", offset, __func__);
    mod_1 = mod_wrap(1, p);
    mod_2 = mod_wrap(0, p);
    mod_res = mod_mul(mod_1, mod_2);
    assert(mod_immed(mod_res, 0));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 4", offset, __func__);
    mod_1 = mod_wrap(2, p);
    mod_2 = mod_wrap(3, p);
    mod_res = mod_mul(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 6));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 4", offset, __func__);
    mod_1 = mod_wrap(5, p);
    mod_2 = mod_wrap(6, p);
    mod_res = mod_mul(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 2));
    mod_free(mod_res);

    num_free(p);
    p = num_wrap(6);

    if(show) printf("\n%s\t%s 5", offset, __func__);
    mod_1 = mod_wrap(3, p);
    mod_2 = mod_wrap(2, p);
    mod_res = mod_mul(mod_1, mod_2);
    assert(mod_immed(mod_res, 0));
    mod_free(mod_res);

    num_free(p);

    assert(clu_mem_empty());
}

void test_mod_div(bool show)
{
    char offset[] = "\t";
    printf("\n%s%s", offset, __func__);

    num_p p = num_wrap(7);

    if(show) printf("\n%s\t%s 1", offset, __func__);
    mod_t mod_1 = mod_wrap(0, p);
    mod_t mod_2 = mod_wrap(2, p);
    mod_t mod_res = mod_div(mod_1, mod_2);
    assert(mod_immed(mod_res, 0));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 2", offset, __func__);
    mod_1 = mod_wrap(6, p);
    mod_2 = mod_wrap(2, p);
    mod_res = mod_div(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 3));
    mod_free(mod_res);

    if(show) printf("\n%s\t%s 3", offset, __func__);
    mod_1 = mod_wrap(6, p);
    mod_2 = mod_wrap(4, p);

    mod_res = mod_div(mod_1, mod_2);
    assert(mod_immed(mod_res, 1, 5));
    mod_free(mod_res);

    num_free(p);

    assert(clu_mem_empty());
}



void test_mod()
{
    printf("\n%s", __func__);

    bool show = false;

    test_mod_create(show);
    test_mod_wrap(show);

    test_mod_add(show);
    test_mod_sub(show);
    test_mod_mul(show);
    test_mod_div(show);

    assert(clu_mem_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_mod();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
