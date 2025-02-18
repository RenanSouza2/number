#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../lib/num/header.h"
#include "../utils/assert.h"

num_p num_generate(uint64_t max, uint64_t salt)
{
    num_p num = num_wrap(2);
    for(uint64_t i=0; i<max; i++)
    {
        // printf("\ni: %lu", i);
        num = num_add(num, num_wrap(salt));
        num = num_mul(num, num_copy(num));
    }
    return num;
}

uint64_t altutime() 
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1e6 + time.tv_nsec / 1e3;
}

void time_1()
{
    
    for(uint64_t i=6; i<21; i++)
    {
        printf("\n%2lu:", i);

        uint64_t begin = altutime();
        num_p num_1 = num_generate(i+1, 1);
        num_display_tag("num_1", num_1);
        uint64_t end = altutime();
        printf("\t%10lu", end - begin);

        begin = altutime();
        num_p num_2 = num_generate(i, 2);
        num_display_tag("num_2", num_2);
        end = altutime();
        printf("\t%10lu", end - begin);

        begin = altutime();
        num_1 = num_div(num_1, num_2);
        num_display_tag("num_q", num_1);
        end = altutime();
        printf("\t%10lu", end - begin);

        num_free(num_1);
    }
}

void time_2(int argc, char** argv)
{
    assert(argc > 1);
    uint64_t id = atoi(argv[1]);
    printf("\nid: %lu", id);

    num_p num_1 = num_generate(20, 2);
    num_display_tag("num_1", num_1);

    num_p num_2 = num_wrap(0xe6503424c62eef89);
    for(uint64_t i=1; num_cmp(num_1, num_2) > 0; i++)
    {
        num_2 = num_add(num_2, num_wrap(2));
        num_2 = num_mul(num_2, num_wrap(0xe6503424c62eef89));

        if(i % 6 != id)
            continue;

        printf("\n%5lu", i);

        num_p num_aux_1 = num_copy(num_1);
        num_p num_aux_2 = num_copy(num_2);

        uint64_t begin = altutime();
        num_aux_1 = num_div(num_aux_1, num_aux_2);
        uint64_t end = altutime();
        printf("\t%10.3lf", (end - begin) / 1e6);

        num_free(num_aux_1);
    }

}

int main(int argc, char** argv)
{
    setbuf(stdout, NULL);

    // time_1();
    time_2(argc, argv);

    printf("\n");
    return 0;
}
