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



STRUCT(list_float)
{
    float_num_t flt_res;
    list_float_p next;
};

list_float_p list_create(float_num_t flt_res)
{
    list_float_p list = malloc(sizeof(list_float_t));
    assert(list);

    *list = (list_float_t)
    {
        .flt_res = flt_res,
        .next = NULL
    };
    return list;
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

uint64_t line_get_count(line_p l)
{
    return l->start > l->end ?
        l->end + l->size - l->start :
        l->end - l->start;
}

uint64_t line_get_index(line_p l, uint64_t i)
{
    assert(i < line_get_count(l));
    uint64_t index = l->start + i;
    return index >= l->size ?
        index - l->size : index;
}

void line_post_response(line_p l, float_num_t res)
{
    TREAT(sem_wait(&l->sem_b));
    l->res[l->end] = res;
    l->end++;
    if(l->end == l->size)
        l->end = 0;
    TREAT(sem_post(&l->sem_f));
}

float_num_t line_get_response(line_p l)
{
    TREAT(sem_wait(&l->sem_f));
    float_num_t res = l->res[l->start];
    l->start++;
    if(l->start == l->size)
        l->start = 0;
    TREAT(sem_post(&l->sem_b));

    return res;
}

float_num_t line_tryget_response(line_p l)
{
    if(sem_trywait(&l->sem_f) != 0)
        return float_num_wrap(0, 1);

    float_num_t res = l->res[l->start];
    l->start++;
    if(l->start == l->size)
        l->start = 0;
    TREAT(sem_post(&l->sem_b));

    return res;
}

void line_free(line_p l)
{
    while(l->start != l->end)
    {
        float_num_t flt = line_get_response(l);
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

float_num_t zip_get_response(zip_p z)
{
    uint64_t index = z->index;
    z->index++;
    if(z->index == z->total)
        z->index = 0;
    return line_get_response(&z->lines[index]);
}

#endif