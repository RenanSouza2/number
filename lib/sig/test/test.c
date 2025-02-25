#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_sig()
{
    printf("\n%s\t\t", __func__);

    assert(clu_mem_empty());
}


int main()
{
    setbuf(stdout, NULL);
    test_sig();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
