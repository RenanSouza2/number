#include <stdio.h>

#include "../lib/num/header.h"

int main(int argc, char** argv)
{
    num_p num_1 = num_wrap(2);
    for(uint64_t i=0; ; i++)
    {
        num_p num_2 = num_copy(num_1);
        num_1 = num_mul(num_1, num_2);
        num_2 = num_wrap(1);
        num_1 = num_add(num_1, num_2, false);
        num_display_immed("num", num_1);
    }

    printf("\n");
    return 0;
}