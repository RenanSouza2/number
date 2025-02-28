#include "../debug.h"
#include "../../num/debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/header.h"



void test_sig_create(bool show)
{
    printf("\n\t%s\t\t", __func__);
    
    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(1, 1);
    sig_p sig = sig_create(POSITIVE, num);
    assert(sig->signal == POSITIVE);
    assert(sig->num == num);
    sig_free(sig);
    
    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 1);
    sig = sig_create(NEGATIVE, num);
    assert(sig->signal == NEGATIVE);
    assert(sig->num == num);
    sig_free(sig);
    
    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(0);
    sig = sig_create(POSITIVE, num);
    assert(sig->signal == ZERO);
    assert(sig->num == num);
    sig_free(sig);
    
    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num = num_create_immed(0);
    sig = sig_create(ZERO, num);
    assert(sig->signal == ZERO);
    assert(sig->num == num);
    sig_free(sig);
    
    if(show) printf("\n\t\t%s 5\t\t", __func__);
    num = num_create_immed(0);
    sig = sig_create(NEGATIVE, num);
    assert(sig->signal == ZERO);
    assert(sig->num == num);
    sig_free(sig);

    assert(clu_mem_empty());
}

void test_sig_create_immed(bool show)
{
    printf("\n\t%s\t\t", __func__);
    
    if(show) printf("\n\t\t%s 1\t\t", __func__);
    sig_p sig = sig_create_immed(POSITIVE, 1, 1);
    assert(sig->signal == POSITIVE);
    assert(num_immed(sig->num, 1, 1));
    free(sig);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    sig = sig_create_immed(NEGATIVE, 1, 1);
    assert(sig->signal == NEGATIVE);
    assert(num_immed(sig->num, 1, 1));
    free(sig);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    sig = sig_create_immed(POSITIVE, 0);
    assert(sig->signal == ZERO);
    assert(num_immed(sig->num, 0));
    free(sig);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    sig = sig_create_immed(ZERO, 0);
    assert(sig->signal == ZERO);
    assert(num_immed(sig->num, 0));
    free(sig);

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    sig = sig_create_immed(NEGATIVE, 0);
    assert(sig->signal == ZERO);
    assert(num_immed(sig->num, 0));
    free(sig);

    assert(clu_mem_empty());
}



void test_sig_wrap(bool show)
{
    printf("\n\t%s\t\t", __func__);
    
    if(show) printf("\n\t\t%s 1\t\t", __func__);
    sig_p sig = sig_wrap(0);
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 2\t\t", __func__);
    sig = sig_wrap(1);
    assert(sig_immed(sig, POSITIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 3\t\t", __func__);
    sig = sig_wrap(-1);
    assert(sig_immed(sig, NEGATIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 4\t\t", __func__);
    sig = sig_wrap(INT64_MAX);
    assert(sig_immed(sig, POSITIVE, 1, INT64_MAX));
    
    if(show) printf("\n\t\t%s 5\t\t", __func__);
    sig = sig_wrap(INT64_MIN);
    assert(sig_immed(sig, NEGATIVE, 1, (uint64_t)INT64_MIN));

    assert(clu_mem_empty());
}

void test_sig_wrap_str(bool show)
{
    printf("\n\t%s\t\t", __func__);
    
    if(show) printf("\n\t\t%s  1\t\t", __func__);
    sig_p sig = sig_wrap_str("");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s  2\t\t", __func__);
    sig = sig_wrap_str("0");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s  3\t\t", __func__);
    sig = sig_wrap_str("1");
    assert(sig_immed(sig, POSITIVE, 1, 1));
    
    if(show) printf("\n\t\t%s  4\t\t", __func__);
    sig = sig_wrap_str("10");
    assert(sig_immed(sig, POSITIVE, 1, 10));
    
    if(show) printf("\n\t\t%s  5\t\t", __func__);
    sig = sig_wrap_str("+");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s  6\t\t", __func__);
    sig = sig_wrap_str("+0");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s  7\t\t", __func__);
    sig = sig_wrap_str("+1");
    assert(sig_immed(sig, POSITIVE, 1, 1));
    
    if(show) printf("\n\t\t%s  8\t\t", __func__);
    sig = sig_wrap_str("+10");
    assert(sig_immed(sig, POSITIVE, 1, 10));
    
    if(show) printf("\n\t\t%s  9\t\t", __func__);
    sig = sig_wrap_str("-");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 10\t\t", __func__);
    sig = sig_wrap_str("-0");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 11\t\t", __func__);
    sig = sig_wrap_str("-1");
    assert(sig_immed(sig, NEGATIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 12\t\t", __func__);
    sig = sig_wrap_str("-10");
    assert(sig_immed(sig, NEGATIVE, 1, 10));
    
    if(show) printf("\n\t\t%s 13\t\t", __func__);
    sig = sig_wrap_str("0x");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 14\t\t", __func__);
    sig = sig_wrap_str("0x0");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 15\t\t", __func__);
    sig = sig_wrap_str("0x1");
    assert(sig_immed(sig, POSITIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 16\t\t", __func__);
    sig = sig_wrap_str("0x10");
    assert(sig_immed(sig, POSITIVE, 1, 16));
    
    if(show) printf("\n\t\t%s 17\t\t", __func__);
    sig = sig_wrap_str("+0x");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 18\t\t", __func__);
    sig = sig_wrap_str("+0x0");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 19\t\t", __func__);
    sig = sig_wrap_str("+0x1");
    assert(sig_immed(sig, POSITIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 20\t\t", __func__);
    sig = sig_wrap_str("+0x10");
    assert(sig_immed(sig, POSITIVE, 1, 16));
    
    if(show) printf("\n\t\t%s 21\t\t", __func__);
    sig = sig_wrap_str("-0x");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 22\t\t", __func__);
    sig = sig_wrap_str("-0x0");
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 23\t\t", __func__);
    sig = sig_wrap_str("-0x1");
    assert(sig_immed(sig, NEGATIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 24\t\t", __func__);
    sig = sig_wrap_str("-0x10");
    assert(sig_immed(sig, NEGATIVE, 1, 16));

    assert(clu_mem_empty());
}

void test_sig_copy(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_COPY(NUM, ...)                                 \
        {                                                           \
            if(show) printf("\n\t\t%s  " #NUM "\t\t", __func__);    \
            sig_p sig = sig_create_immed(__VA_ARGS__);              \
            sig_p sig_res = sig_copy(sig);                          \
            assert(sig_immed(sig_res, __VA_ARGS__))                 \
            sig_free(sig);                                          \
        }

    TEST_SIG_COPY(1, POSITIVE, 0);
    TEST_SIG_COPY(2, POSITIVE, 1, 1);
    TEST_SIG_COPY(3, POSITIVE, 2, 1, 2);
    TEST_SIG_COPY(4, NEGATIVE, 1, 1);
    TEST_SIG_COPY(5, NEGATIVE, 2, 1, 2);

    assert(clu_mem_empty());
}




void test_sig_opposite(bool show)
{
    printf("\n\t%s\t\t", __func__);
    
    if(show) printf("\n\t\t%s 1\t\t", __func__);
    sig_p sig = sig_create_immed(POSITIVE, 1, 1);
    sig = sig_opposite(sig);
    assert(sig_immed(sig, NEGATIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 2\t\t", __func__);
    sig = sig_create_immed(ZERO, 0);
    sig = sig_opposite(sig);
    assert(sig_immed(sig, ZERO, 0));
    
    if(show) printf("\n\t\t%s 3\t\t", __func__);
    sig = sig_create_immed(NEGATIVE, 1, 1);
    sig = sig_opposite(sig);
    assert(sig_immed(sig, POSITIVE, 1, 1));

    assert(clu_mem_empty());
}

void test_sig_add(bool show)
{
    printf("\n\t%s\t\t", __func__);
    
    if(show) printf("\n\t\t%s  1\t\t", __func__);
    sig_p sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_p sig_2 = sig_create_immed(POSITIVE, 1, 1);
    sig_p sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 3));
    
    if(show) printf("\n\t\t%s  2\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(ZERO, 0);
    sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 2));
    
    if(show) printf("\n\t\t%s  3\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(NEGATIVE, 1, 1);
    sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 1));
    
    if(show) printf("\n\t\t%s  4\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(NEGATIVE, 1, 2);
    sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, ZERO, 0));
    
    if(show) printf("\n\t\t%s  5\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(NEGATIVE, 1, 3);
    sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, NEGATIVE, 1, 1));
    
    if(show) printf("\n\t\t%s  6\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 1);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 3));
    
    if(show) printf("\n\t\t%s  7\t\t", __func__);
    sig_1 = sig_create_immed(ZERO, 0);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 2));
    
    if(show) printf("\n\t\t%s  8\t\t", __func__);
    sig_1 = sig_create_immed(NEGATIVE, 1, 1);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 1));
    
    if(show) printf("\n\t\t%s  9\t\t", __func__);
    sig_1 = sig_create_immed(NEGATIVE, 1, 2);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, ZERO, 0));
    
    if(show) printf("\n\t\t%s 10\t\t", __func__);
    sig_1 = sig_create_immed(NEGATIVE, 1, 3);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_add(sig_1, sig_2);
    assert(sig_immed(sig_res, NEGATIVE, 1, 1));

    assert(clu_mem_empty());
}

void test_sig_sub(bool show)
{
    printf("\n\t%s\t\t", __func__);
    
    if(show) printf("\n\t\t%s 1\t\t", __func__);
    sig_p sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_p sig_2 = sig_create_immed(POSITIVE, 1, 1);
    sig_p sig_res = sig_sub(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 2\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_sub(sig_1, sig_2);
    assert(sig_immed(sig_res, ZERO, 0));
    
    if(show) printf("\n\t\t%s 3\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(POSITIVE, 1, 3);
    sig_res = sig_sub(sig_1, sig_2);
    assert(sig_immed(sig_res, NEGATIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 4\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(NEGATIVE, 1, 1);
    sig_res = sig_sub(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 3));

    assert(clu_mem_empty());
}

void test_sig_mul(bool show)
{
    printf("\n\t%s\t\t", __func__);
    
    if(show) printf("\n\t\t%s 1\t\t", __func__);
    sig_p sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_p sig_2 = sig_create_immed(POSITIVE, 1, 3);
    sig_p sig_res = sig_mul(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 6));
    
    if(show) printf("\n\t\t%s 2\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(ZERO, 0);
    sig_res = sig_mul(sig_1, sig_2);
    assert(sig_immed(sig_res, ZERO, 0));
    
    if(show) printf("\n\t\t%s 3\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(NEGATIVE, 1, 3);
    sig_res = sig_mul(sig_1, sig_2);
    assert(sig_immed(sig_res, NEGATIVE, 1, 6));
    
    if(show) printf("\n\t\t%s 4\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 3);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_mul(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 6));
    
    if(show) printf("\n\t\t%s 5\t\t", __func__);
    sig_1 = sig_create_immed(ZERO, 0);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_mul(sig_1, sig_2);
    assert(sig_immed(sig_res, ZERO, 0));
    
    if(show) printf("\n\t\t%s 6\t\t", __func__);
    sig_1 = sig_create_immed(NEGATIVE, 1, 3);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_mul(sig_1, sig_2);
    assert(sig_immed(sig_res, NEGATIVE, 1, 6));

    assert(clu_mem_empty());
}

void test_sig_div(bool show)
{
    printf("\n\t%s\t\t", __func__);
    
    if(show) printf("\n\t\t%s 1\t\t", __func__);
    sig_p sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_p sig_2 = sig_create_immed(POSITIVE, 1, 3);
    sig_p sig_res = sig_div(sig_1, sig_2);
    assert(sig_immed(sig_res, ZERO, 0));
    
    if(show) printf("\n\t\t%s 2\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 2);
    sig_2 = sig_create_immed(NEGATIVE, 1, 3);
    sig_res = sig_div(sig_1, sig_2);
    assert(sig_immed(sig_res, ZERO, 0));
    
    if(show) printf("\n\t\t%s 3\t\t", __func__);
    sig_1 = sig_create_immed(POSITIVE, 1, 3);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_div(sig_1, sig_2);
    assert(sig_immed(sig_res, POSITIVE, 1, 1));
    
    if(show) printf("\n\t\t%s 4\t\t", __func__);
    sig_1 = sig_create_immed(ZERO, 0);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_div(sig_1, sig_2);
    assert(sig_immed(sig_res, ZERO, 0));
    
    if(show) printf("\n\t\t%s 5\t\t", __func__);
    sig_1 = sig_create_immed(NEGATIVE, 1, 3);
    sig_2 = sig_create_immed(POSITIVE, 1, 2);
    sig_res = sig_div(sig_1, sig_2);
    assert(sig_immed(sig_res, NEGATIVE, 1, 1));

    assert(clu_mem_empty());
}



void test_sig()
{
    printf("\n%s\t\t", __func__);

    bool show = true;

    test_sig_create(show);
    test_sig_create_immed(show);

    test_sig_wrap(show);
    test_sig_wrap_str(show);
    test_sig_copy(show);

    test_sig_opposite(show);
    test_sig_add(show);
    test_sig_sub(show);
    test_sig_mul(show);
    test_sig_div(show);

    assert(clu_mem_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_sig();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
