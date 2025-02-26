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
    assert(sig_immed(sig, POSITIVE, 1, 1))

    assert(clu_mem_empty());
}



void test_sig()
{
    printf("\n%s\t\t", __func__);

    bool show = true;

    test_sig_create(show);

    assert(clu_mem_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_sig();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
