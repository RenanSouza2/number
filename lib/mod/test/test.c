#include "../debug.h"
#include "../../num/debug.h"

#include "../../../mods/macros/test.h"
#include "../../../mods/macros/assert.h"
#include "../../../mods/clu/header.h"



#define TEST_ASSERT_EMPTY assert(clu_mem_is_empty());

void test_mod_create(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        num_t num = num_create_immed(0);
        mod_t mod = mod_create(num, p);
        assert(mod_immed(mod, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        num_t num = num_create_immed(1, 1);
        mod_t mod = mod_create(num, p);
        assert(mod_immed(mod, 1, 1));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        num_t num = num_create_immed(1, 7);
        mod_t mod = mod_create(num, p);
        assert(mod_immed(mod, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        num_t num = num_create_immed(1, 8);
        mod_t mod = mod_create(num, p);
        assert(mod_immed(mod, 1, 1));
    }
    TEST_CASE_CLOSE

    num_free(p);
    
    TEST_FN_CLOSE
}

void test_mod_wrap(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_t mod = mod_wrap(0, p);
        assert(mod_immed(mod, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_t mod = mod_wrap(2, p);
        assert(mod_immed(mod, 1, 2));
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}



void test_mod_add(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_t mod_1 = mod_wrap(0, p);
        mod_t mod_2 = mod_wrap(0, p);
        mod_t mod_res = mod_add(mod_1, mod_2);
        assert(mod_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_t mod_1 = mod_wrap(1, p);
        mod_t mod_2 = mod_wrap(2, p);
        mod_t mod_res = mod_add(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 3));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        mod_t mod_1 = mod_wrap(1, p);
        mod_t mod_2 = mod_wrap(6, p);
        mod_t mod_res = mod_add(mod_1, mod_2);
        assert(mod_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        mod_t mod_1 = mod_wrap(6, p);
        mod_t mod_2 = mod_wrap(6, p);
        mod_t mod_res = mod_add(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 5));
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}

void test_mod_sub(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_t mod_1 = mod_wrap(0, p);
        mod_t mod_2 = mod_wrap(0, p);
        mod_t mod_res = mod_sub(mod_1, mod_2);
        assert(mod_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_t mod_1 = mod_wrap(0, p);
        mod_t mod_2 = mod_wrap(1, p);
        mod_t mod_res = mod_sub(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 6));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        mod_t mod_1 = mod_wrap(2, p);
        mod_t mod_2 = mod_wrap(0, p);
        mod_t mod_res = mod_sub(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 2));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        mod_t mod_1 = mod_wrap(3, p);
        mod_t mod_2 = mod_wrap(2, p);
        mod_t mod_res = mod_sub(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 1));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        mod_t mod_1 = mod_wrap(2, p);
        mod_t mod_2 = mod_wrap(5, p);
        mod_t mod_res = mod_sub(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 4));
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}

void test_mod_mul(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_t mod_1 = mod_wrap(0, p);
        mod_t mod_2 = mod_wrap(0, p);
        mod_t mod_res = mod_mul(mod_1, mod_2);
        assert(mod_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_t mod_1 = mod_wrap(0, p);
        mod_t mod_2 = mod_wrap(1, p);
        mod_t mod_res = mod_mul(mod_1, mod_2);
        assert(mod_immed(mod_res,0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        mod_t mod_1 = mod_wrap(1, p);
        mod_t mod_2 = mod_wrap(0, p);
        mod_t mod_res = mod_mul(mod_1, mod_2);
        assert(mod_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        mod_t mod_1 = mod_wrap(2, p);
        mod_t mod_2 = mod_wrap(3, p);
        mod_t mod_res = mod_mul(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 6));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        mod_t mod_1 = mod_wrap(5, p);
        mod_t mod_2 = mod_wrap(6, p);
        mod_t mod_res = mod_mul(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 2));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(6)
    {
        num_t p_6 = num_wrap(6);

        mod_t mod_1 = mod_wrap(3, p_6);
        mod_t mod_2 = mod_wrap(2, p_6);
        mod_t mod_res = mod_mul(mod_1, mod_2);
        assert(mod_immed(mod_res, 0));

        num_free(p_6);
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}

void test_mod_div(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_t mod_1 = mod_wrap(0, p);
        mod_t mod_2 = mod_wrap(2, p);
        mod_t mod_res = mod_div(mod_1, mod_2);
        assert(mod_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_t mod_1 = mod_wrap(6, p);
        mod_t mod_2 = mod_wrap(2, p);
        mod_t mod_res = mod_div(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 3));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        mod_t mod_1 = mod_wrap(6, p);
        mod_t mod_2 = mod_wrap(4, p);
        mod_t mod_res = mod_div(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 5));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        num_t p_29 = num_wrap(29);

        mod_t mod_1 = mod_wrap(1, p_29);
        mod_t mod_2 = mod_wrap(9, p_29);
        mod_t mod_res = mod_div(mod_1, mod_2);
        assert(mod_immed(mod_res, 1, 13));

        num_free(p_29);
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}



void test_mod()
{
    TEST_LIB

    bool show = true;

    test_mod_create(show);
    test_mod_wrap(show);

    test_mod_add(show);
    test_mod_sub(show);
    test_mod_mul(show);
    test_mod_div(show);

    TEST_ASSERT_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    TEST_TIMEOUT_OPEN_DEFAULT
    test_mod();
    TEST_TIMEOUT_CLOSE
    printf("\n\n\tTest successful\n\n");
    return 0;
}
