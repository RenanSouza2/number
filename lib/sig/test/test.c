#include "../debug.h"
#include "../../num/debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



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



void test_sig()
{
    printf("\n%s\t\t", __func__);

    bool show = true;

    test_sig_create(show);
    test_sig_create_immed(show);

    assert(clu_mem_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_sig();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
