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
    pthread_create(&thread_id, NULL, fn, args);
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
    pthread_mutex_t mut;
    list_float_p first;
    list_float_p last;
};

line_t line_init(uint64_t max)
{
    sem_t sem_f, sem_b;
    sem_init(&sem_f, 0, 0);
    sem_init(&sem_b, 0, max);
    return (line_t)
    {
        .sem_f = sem_f,
        .sem_b = sem_b,
        .first = NULL,
        .last = NULL
    };
}

void line_free(line_p l)
{
    sem_destroy(&l->sem_f);
    sem_destroy(&l->sem_b);
    pthread_mutex_destroy(&l->mut);

    for(list_float_p head = l->first; head; )
    {
        list_float_p next = head->next;
        float_num_free(head->flt_res);
        free(head);
        head = next;
    }
}

void line_post_response(line_p l, float_num_t flt_res)
{
    TREAT(sem_wait(&l->sem_b));
    TREAT(pthread_mutex_lock(&l->mut));
    list_float_p list = list_create(flt_res);
    if(l->last)
    {
        l->last = l->last->next = list;
    }
    else
    {
        l->last = l->first = list;
    }
    TREAT(pthread_mutex_unlock(&l->mut));
    TREAT(sem_post(&l->sem_f));
}

float_num_t line_get_response(line_p l)
{
    TREAT(sem_wait(&l->sem_f));
    TREAT(pthread_mutex_lock(&l->mut));
    list_float_p list = l->first;
    l->first = list->next;
    if(l->first == NULL)
        l->last = NULL;
    TREAT(pthread_mutex_unlock(&l->mut));
    TREAT(sem_post(&l->sem_b));

    float_num_t res = list->flt_res;
    free(list);
    return res;
}

float_num_t line_tryget_response(line_p l)
{
    if(sem_trywait(&l->sem_f) != 0)
    {
        return float_num_wrap(0, 1);
    }

    TREAT(pthread_mutex_lock(&l->mut));
    list_float_p list = l->first;
    l->first = list->next;
    if(l->first == NULL)
        l->last = NULL;
    TREAT(pthread_mutex_unlock(&l->mut));
    TREAT(sem_post(&l->sem_b));

    float_num_t res = list->flt_res;
    free(list);
    return res;
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