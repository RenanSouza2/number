#ifndef __PTHREAD_STRUCT__
#define __PTHREAD_STRUCT__

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>

#include "../../mods/macros/struct.h"
#include "../../mods/macros/threads.h"
#include "../../mods/macros/U64.h"



void dbg(char format[], ...)
{
    char name[100];
    sprintf(name, "thread_log/log_%lu.txt", pthread_self());
    FILE *fp = fopen(name, "a");
    assert(fp);

    va_list args;
    va_start(args, format);
    fprintf(fp, "\n");
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

uint64_t sem_getvalue_return(sem_t *sem)
{
    int value;
    TREAT(sem_getvalue(sem, &value));
    return value;
}



typedef void (*free_f)(handler_p);

STRUCT(queue)
{
    sem_t sem_f, sem_b;
    uint64_t max_res;
    uint64_t size_res;
    handler_p *res;
    uint64_t start, end;
    free_f free_res;
};

queue_t queue_init(uint64_t max_res, uint64_t size_res, free_f free_res)
{
    sem_t sem_f, sem_b;
    sem_init(&sem_f, 0, 0);
    sem_init(&sem_b, 0, max_res);

    handler_p *res = malloc(max_res * sizeof(handler_p));
    assert(res);

    for(uint64_t i=0; i<max_res; i++)
    {
        res[i] = malloc(size_res);
        assert(res[i]);
    }

    return (queue_t)
    {
        .sem_f = sem_f,
        .sem_b = sem_b,
        .max_res = max_res,
        .size_res = size_res,
        .res = res,
        .start = 0,
        .end = 0,
        .free_res = free_res
    };
}

void sem_wait_log(sem_t *sem, bool *is_halted)
{
    if(is_halted)
    {
        if(sem_trywait(sem))
        {
            *is_halted = true;
            TREAT(sem_wait(sem));
            *is_halted = false;
        }
    } else {
        TREAT(sem_wait(sem));
    }
}

void queue_post_locked(queue_p q, handler_p res)
{
    uint64_t index = q->end;
    q->end++;
    if(q->end == q->max_res)
        q->end = 0;
    memcpy(q->res[index], res, q->size_res);
    TREAT(sem_post(&q->sem_f));
}

void queue_post(queue_p q, handler_p res, bool * is_halted)
{
    sem_wait_log(&q->sem_b, is_halted);
    queue_post_locked(q, res);
}

void queue_trypost(queue_p q, handler_p res)
{
    if(sem_trywait(&q->sem_b))
    {
        q->free_res(res);
        return;
    }

    queue_post_locked(q, res);
}

void queue_get_locked(queue_p q, handler_p out_res)
{
    handler_p res = q->res[q->start];
    memcpy(out_res, res, q->size_res);
    q->start++;
    if(q->start == q->max_res)
        q->start = 0;
    TREAT(sem_post(&q->sem_b));
}

void queue_get(queue_p q, handler_p out_res, bool * is_halted)
{
    sem_wait_log(&q->sem_f, is_halted);
    queue_get_locked(q, out_res);
}

void queue_tryget(queue_p q, handler_p out_res)
{
    if(sem_trywait(&q->sem_f))
        return;

    queue_get_locked(q, out_res);
}

void queue_free(queue_p q)
{
    handler_p h = malloc(q->size_res);
    assert(h);

    while(sem_getvalue_return(&q->sem_f))
    {
        queue_get(q, h, NULL);
        q->free_res(h);
    }
    
    for(uint64_t i=0; i<q->start; i++)
    free(q->res[i]);
    
    free(q->res);
    
    TREAT(sem_destroy(&q->sem_f));
    TREAT(sem_destroy(&q->sem_b));
}



STRUCT(junc)
{
    uint64_t total;
    uint64_t index;
    queue_p queues;
};

junc_t junc_init(
    uint64_t total,
    uint64_t max_res,
    uint64_t size_res,
    free_f free_res
)
{
    queue_p queues = malloc(total * sizeof(queue_t));
    for(uint64_t i=0; i<total; i++)
        queues[i] = queue_init(max_res, size_res, free_res);

    return (junc_t)
    {
        .total = total,
        .index = 0,
        .queues = queues
    };
}

void junc_free(junc_p junc)
{
    for(uint64_t i=0; i<junc->total; i++)
        queue_free(&junc->queues[i]);

    free(junc->queues);
}

void junc_get(junc_p junc, handler_p out_res, bool * is_halted)
{
    uint64_t index = junc->index;
    junc->index++;
    if(junc->index == junc->total)
        junc->index = 0;
    queue_get(&junc->queues[index], out_res, is_halted);
}

void junc_post(junc_p junc, handler_p res, bool * is_halted)
{
    uint64_t index = junc->index;
    junc->index++;
    if(junc->index == junc->total)
        junc->index = 0;
    queue_post(&junc->queues[index], res, is_halted);
}

#endif