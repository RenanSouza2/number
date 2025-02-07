#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_mod_add(bool show)
{
    printf("\n\t%s", __func__);
    
    num_p p = num_wrap(7);

    if(show) printf("\n\t\t%s 1", __func__);
    mod_t mod_1 = mod_wrap(0, p);
    mod_t mod_2 = mod_wrap(0, p);
    mod_t mod_res = mod_add(mod_1, mod_2);
    assert(mod_immed(mod_res, 0))

    assert(clu_mem_empty());
}



void test_mod()
{
    printf("\n%s", __func__);

    assert(clu_mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_mod();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
