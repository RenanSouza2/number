#include <stdio.h>
#include <time.h>

#include "../lib/num/header.h"

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
    // #if !defined(_WIN32) && !defined(__APPLE__)
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1e6 + time.tv_nsec / 1e3;
    // #else
    // struct timeval time;
    // gettimeofday(&time, NULL);
    // return time.tv_sec * 1000000 + time.tv_usec;
    // #endif
}
 

int main(int argc, char** argv)
{
    setbuf(stdout, NULL);

    num_free(num_wrap(1));

    for(uint64_t i=5; i<21; i++)
    {
        uint64_t begin = altutime();
        num_p num_1 = num_generate(i+1, 1);
        // num_display_immed("num_1", num_1);
        uint64_t end = altutime();
        printf("\n%2lu:\t%16lu", i, end - begin);

        begin = altutime();
        num_p num_2 = num_generate(i, 2);
        // num_display_immed("num_2", num_2);
        end = altutime();
        printf("\t%16lu", end - begin);

        begin = altutime();
        num_1 = num_div(num_1, num_2);
        // num_display_immed("num_1", num_1);
        end = altutime();
        printf("\t%20lu", end - begin);

        num_free(num_1);
    }

    printf("\n");
    return 0;
}
