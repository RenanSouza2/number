#ifndef __PTHREAD_STRUCT__
#define __PTHREAD_STRUCT__

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>

#include "../../mods/macros/struct.h"
#include "../../mods/macros/threads.h"

#include "../../lib/float/header.h"



void dbg(char format[], ...)
{
    char name[100];
    sprintf(name, "thread_log/log_%lu.txt", pthread_self());
    FILE *fp = fopen(name, "a");
    assert(fp);

    va_list args;
    va_start(args, format);
    vfprintf(fp, format, args);
    fclose(fp);
}

typedef handler_p (*pthread_f)(handler_p);

pthread_t pthread_launch(pthread_f fn, handler_p args)
{
    pthread_t thread_id;
    TREAT(pthread_create(&thread_id, NULL, fn, args));
    return thread_id;
}

void pthread_wait(pthread_t thread_id)
{
    TREAT(pthread_join(thread_id, NULL));
}



STRUCT(line)
{
    sem_t sem_f, sem_b;
    float_num_p res;
    uint64_t size;
    uint64_t start, end;
};

line_t line_init(uint64_t size)
{
    sem_t sem_f, sem_b;
    sem_init(&sem_f, 0, 0);
    sem_init(&sem_b, 0, size);

    float_num_p res = malloc(size * sizeof(float_num_t));
    assert(res);

    return (line_t)
    {
        .sem_f = sem_f,
        .sem_b = sem_b,
        .res = res,
        .size = size,
        .start = 0,
        .end = 0
    };
}

void sem_wait_log(sem_t *sem, bool *is_halted)
{
    if(is_halted)
    {
        if(sem_trywait(sem) != 0)
        {
            *is_halted = true;
            TREAT(sem_wait(sem));
            *is_halted = false;
        }
    } else {
        TREAT(sem_wait(sem));
    }
}

float_num_t line_get_response_locked(line_p l)
{
    float_num_t res = l->res[l->start];
    l->start++;
    if(l->start == l->size)
        l->start = 0;
    TREAT(sem_post(&l->sem_b));

    return res;
}

void line_post_response(line_p l, float_num_t res, bool *is_halted)
{
    sem_wait_log(&l->sem_b, is_halted);
    l->res[l->end] = res;
    l->end++;
    if(l->end == l->size)
        l->end = 0;
    TREAT(sem_post(&l->sem_f));
}

float_num_t line_get_response(line_p l, bool * is_halted)
{
    sem_wait_log(&l->sem_f, is_halted);
    return line_get_response_locked(l);
}

float_num_t line_tryget_response(line_p l)
{
    if(sem_trywait(&l->sem_f) != 0)
        return float_num_wrap(0, 1);

    return line_get_response_locked(l);
}

void line_free(line_p l)
{
    bool ignore;
    for(uint64_t i = 0; l->start != l->end; i++)
    {
        // printf("\nAAA");
        float_num_t flt = line_get_response(l, &ignore);
        // printf("\nBBB");
        float_num_free(flt);
    }
    
    TREAT(sem_destroy(&l->sem_f));
    TREAT(sem_destroy(&l->sem_b));

    free(l->res);
}



STRUCT(zip)
{
    uint64_t total;
    uint64_t index;
    line_p lines;
};

zip_t zip_init(uint64_t total, uint64_t max)
{
    line_p lines = malloc(total * sizeof(line_t));
    for(uint64_t i=0; i<total; i++)
        lines[i] = line_init(max);

    return (zip_t)
    {
        .total = total,
        .index = 0,
        .lines = lines
    };
}

void zip_free(zip_p z)
{
    for(uint64_t i=0; i<z->total; i++)
        line_free(&z->lines[i]);

    free(z->lines);
}

float_num_t zip_get_response(zip_p z, bool *is_halted)
{
    uint64_t index = z->index;
    z->index++;
    if(z->index == z->total)
        z->index = 0;
    return line_get_response(&z->lines[index], is_halted);
}

#endif