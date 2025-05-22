#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"

#include "../../num/debug.h"



void test_mod_num_create(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        num_t num = num_create_immed(0);
        mod_num_t mod = mod_num_create(num, p);
        assert(mod_num_immed(mod, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        CLU_HANDLER_IS_SAFE(p.head);
        num_t num = num_create_immed(1, 1);
        mod_num_t mod = mod_num_create(num, p);
        assert(mod_num_immed(mod, 1, 1));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        num_t num = num_create_immed(1, 7);
        mod_num_t mod = mod_num_create(num, p);
        assert(mod_num_immed(mod, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        num_t num = num_create_immed(1, 8);
        mod_num_t mod = mod_num_create(num, p);
        assert(mod_num_immed(mod, 1, 1));
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}

void test_mod_num_wrap(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_num_t mod = mod_num_wrap(0, p);
        assert(mod_num_immed(mod, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_num_t mod = mod_num_wrap(2, p);
        assert(mod_num_immed(mod, 1, 2));
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}



void test_mod_num_add(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_num_t mod_1 = mod_num_wrap(0, p);
        mod_num_t mod_2 = mod_num_wrap(0, p);
        mod_num_t mod_res = mod_num_add(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_num_t mod_1 = mod_num_wrap(1, p);
        mod_num_t mod_2 = mod_num_wrap(2, p);
        mod_num_t mod_res = mod_num_add(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 3));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        mod_num_t mod_1 = mod_num_wrap(1, p);
        mod_num_t mod_2 = mod_num_wrap(6, p);
        mod_num_t mod_res = mod_num_add(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        mod_num_t mod_1 = mod_num_wrap(6, p);
        mod_num_t mod_2 = mod_num_wrap(6, p);
        mod_num_t mod_res = mod_num_add(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 5));
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}

void test_mod_num_sub(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_num_t mod_1 = mod_num_wrap(0, p);
        mod_num_t mod_2 = mod_num_wrap(0, p);
        mod_num_t mod_res = mod_num_sub(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_num_t mod_1 = mod_num_wrap(0, p);
        mod_num_t mod_2 = mod_num_wrap(1, p);
        mod_num_t mod_res = mod_num_sub(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 6));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        mod_num_t mod_1 = mod_num_wrap(2, p);
        mod_num_t mod_2 = mod_num_wrap(0, p);
        mod_num_t mod_res = mod_num_sub(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 2));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        mod_num_t mod_1 = mod_num_wrap(3, p);
        mod_num_t mod_2 = mod_num_wrap(2, p);
        mod_num_t mod_res = mod_num_sub(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 1));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        mod_num_t mod_1 = mod_num_wrap(2, p);
        mod_num_t mod_2 = mod_num_wrap(5, p);
        mod_num_t mod_res = mod_num_sub(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 4));
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}

void test_mod_num_mul(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_num_t mod_1 = mod_num_wrap(0, p);
        mod_num_t mod_2 = mod_num_wrap(0, p);
        mod_num_t mod_res = mod_num_mul(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_num_t mod_1 = mod_num_wrap(0, p);
        mod_num_t mod_2 = mod_num_wrap(1, p);
        mod_num_t mod_res = mod_num_mul(mod_1, mod_2);
        assert(mod_num_immed(mod_res,0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        mod_num_t mod_1 = mod_num_wrap(1, p);
        mod_num_t mod_2 = mod_num_wrap(0, p);
        mod_num_t mod_res = mod_num_mul(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        mod_num_t mod_1 = mod_num_wrap(2, p);
        mod_num_t mod_2 = mod_num_wrap(3, p);
        mod_num_t mod_res = mod_num_mul(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 6));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        mod_num_t mod_1 = mod_num_wrap(5, p);
        mod_num_t mod_2 = mod_num_wrap(6, p);
        mod_num_t mod_res = mod_num_mul(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 2));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(6)
    {
        num_t p_6 = num_wrap(6);

        mod_num_t mod_1 = mod_num_wrap(3, p_6);
        mod_num_t mod_2 = mod_num_wrap(2, p_6);
        mod_num_t mod_res = mod_num_mul(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 0));

        num_free(p_6);
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}

void test_mod_num_div(bool show)
{
    TEST_FN_OPEN

    num_t p = num_wrap(7);

    TEST_CASE_OPEN(1)
    {
        mod_num_t mod_1 = mod_num_wrap(0, p);
        mod_num_t mod_2 = mod_num_wrap(2, p);
        mod_num_t mod_res = mod_num_div(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        mod_num_t mod_1 = mod_num_wrap(6, p);
        mod_num_t mod_2 = mod_num_wrap(2, p);
        mod_num_t mod_res = mod_num_div(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 3));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        mod_num_t mod_1 = mod_num_wrap(6, p);
        mod_num_t mod_2 = mod_num_wrap(4, p);
        mod_num_t mod_res = mod_num_div(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 5));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        num_t p_29 = num_wrap(29);

        mod_num_t mod_1 = mod_num_wrap(1, p_29);
        mod_num_t mod_2 = mod_num_wrap(9, p_29);
        mod_num_t mod_res = mod_num_div(mod_1, mod_2);
        assert(mod_num_immed(mod_res, 1, 13));

        num_free(p_29);
    }
    TEST_CASE_CLOSE

    num_free(p);

    TEST_FN_CLOSE
}



void test_mod()
{
    TEST_LIB

    bool show = false;

    test_mod_num_create(show);
    test_mod_num_wrap(show);

    test_mod_num_add(show);
    test_mod_num_sub(show);
    test_mod_num_mul(show);
    test_mod_num_div(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_mod();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
