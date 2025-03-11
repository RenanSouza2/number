#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/header.h"



void test_fixed_fn(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);

    assert(clu_mem_empty());
}



void test_fixed()
{
    printf("\n%s\t\t", __func__);

    bool show = false;

    test_fixed_fn(show);

    assert(clu_mem_empty());
}


int main()
{
    setbuf(stdout, NULL);
    test_fixed();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
