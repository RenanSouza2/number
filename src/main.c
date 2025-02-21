#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../lib/num/header.h"
#include "../utils/assert.h"
#include "../utils/U64.h"



num_p num_generate(uint64_t max, uint64_t salt)
{
    num_p num = num_wrap(2);
    for(uint64_t i=0; i<max; i++)
    {
        num = num_add(num, num_wrap(salt));
        num = num_mul(num, num_copy(num));
    }
    return num;
}

num_p num_generate_2(uint64_t index)
{
    num_p num = num_wrap(0xe6503424c62eef89);
    for(uint64_t i=1; i<index; i++)
    {
        num = num_add(num, num_wrap(2));
        num = num_mul(num, num_wrap(0xe6503424c62eef89));
    }
    return num;
}

uint64_t altutime() 
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1e6 + time.tv_nsec / 1e3;
}

uint64_t rand_64()
{
    return ((uint64_t)rand()) << 32 | rand();
}



void time_1()
{
    
    for(uint64_t i=13; i<21; i++)
    {
        printf("\n" U64P() "", i);

        num_p num_1 = num_generate(i+1, 1);
        // num_display_tag("num_1", num_1);
        num_p num_2 = num_generate(i, 2);
        // num_display_tag("num_2", num_2);

        uint64_t begin = altutime();
        num_1 = num_div(num_1, num_2);
        // num_display_tag("num_q", num_1);
        uint64_t end = altutime();
        printf("\t%10.3f", (end - begin) / 1e6);

        num_free(num_1);
    }
}

void time_2(int argc, char** argv)
{
    assert(argc > 1);
    uint64_t id = atoi(argv[1]);
    printf("\nid: " U64P() "", id);

    num_p num_1 = num_generate(20, 2);
    num_display_tag("num_1", num_1);

    num_p num_2 = num_wrap(0xe6503424c62eef89);
    for(uint64_t i=1; num_cmp(num_1, num_2) > 0; i++)
    {
        num_2 = num_add(num_2, num_wrap(2));
        num_2 = num_mul(num_2, num_wrap(0xe6503424c62eef89));

        if(i % 8 != id)
            continue;

        printf("\n" U64P(5) "", i);

        num_p num_aux_1 = num_copy(num_1);
        num_p num_aux_2 = num_copy(num_2);

        uint64_t begin = altutime();
        num_aux_1 = num_div(num_aux_1, num_aux_2);
        uint64_t end = altutime();
        printf("\t%10.3lf", (end - begin) / 1e6);
    }
}



void fibonacci()
{

    num_p num_a = num_wrap(1);
    num_p num_b = num_wrap(1);

    uint64_t space = 1e4;
    for(uint64_t i=0; ; i++)
    {
        num_p num_c = num_add(num_a, num_copy(num_b));

        if(i%space == 0)
        {
            printf("\n" U64P() " num: ", i);
            num_display(num_c);
        }

        num_a = num_b;
        num_b = num_c;
    }
}

void a()
{
    num_p num = num_wrap(1);

    uint64_t space = 1e2;
    for(uint64_t i=0; ; i++)
    {
        num = num_mul(num, num_wrap(0xb94ace09e8c3e6ed));
        
        if(i%space == 0)
        {
            printf("\n" U64P() " num: ", i);
            num_display(num);
        }
    }
}



int main(int argc, char** argv)
{
    setbuf(stdout, NULL);
    srand(time(NULL));

    // time_1();
    time_2(argc, argv);

    // num_p num_1 = num_generate(20, 2);
    // num_display_tag("num_1", num_1);

    // uint64_t i, space = 1e3;
    // num_p num_2 = num_wrap(0xe6503424c62eef89);
    // for(i=1; num_cmp(num_1, num_2) > 0; i++)
    // {
    //     // num_2 = num_generate_2(i);

    //     num_2 = num_add(num_2, num_wrap(2));
    //     num_2 = num_mul(num_2, num_wrap(0xe6503424c62eef89));

    //     if(i%space == 0)
    //         printf("\ni: %llu", i);
    // }
    // printf("\nfinal res: %llu", i);

    printf("\n");
    return 0;
}
