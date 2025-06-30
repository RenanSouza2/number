#ifndef __PTHREAD_STRUCT__
#define __PTHREAD_STRUCT__

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>
#include <sched.h>

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
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    TREAT(pthread_create(&thread_id, NULL, fn, args));
    return thread_id;
}

void pthread_lock(pthread_t thread_id, uint64_t cpu)
{
    cpu_set_t cpuset;
    __CPU_ZERO_S(sizeof (cpu_set_t), &cpuset);
    __CPU_SET_S(cpu, sizeof (cpu_set_t), &cpuset);
    TREAT(pthread_setaffinity_np(thread_id, sizeof(cpu_set_t), &cpuset));
}

void pthread_wait(pthread_t thread_id)
{
    TREAT(pthread_join(thread_id, NULL));
}

uint64_t sem_getvalue_treat(sem_t *sem)
{
    int value;
    TREAT(sem_getvalue(sem, &value));
    return value;
}

void sem_wait_log(sem_t *sem, bool volatile * is_idle)
{
    if(is_idle)
    {
        if(sem_trywait(sem))
        {
            *is_idle = true;
            TREAT(sem_wait(sem));
            *is_idle = false;
        }
    } else {
        TREAT(sem_wait(sem));
    }
}



typedef void (*free_f)(handler_p, uint64_t);

STRUCT(queue)
{
    sem_t * sem_f;
    sem_t * sem_b;

    uint64_t res_max;
    uint64_t res_size;
    handler_p *res;

    uint64_t start;
    uint64_t end;

    free_f res_free;
};

queue_t queue_init(uint64_t res_max, uint64_t res_size, free_f res_free)
{
    assert(res_max > 1);

    sem_t *sem_f, *sem_b;
    sem_f = malloc(sizeof(sem_t));
    sem_b = malloc(sizeof(sem_t));
    sem_init(sem_f, 0, 0);
    sem_init(sem_b, 0, res_max);

    handler_p *res = malloc(res_max * sizeof(handler_p));
    assert(res);

    for(uint64_t i=0; i<res_max; i++)
    {
        res[i] = malloc(res_size);
        assert(res[i]);
    }

    return (queue_t)
    {
        .sem_f = sem_f,
        .sem_b = sem_b,
        .res_max = res_max,
        .res_size = res_size,
        .res = res,
        .start = 0,
        .end = 0,
        .res_free = res_free
    };
}

uint64_t queue_get_value(queue_p q)
{
    return sem_getvalue_treat(q->sem_f);
}

void queue_post(queue_p q, handler_p res, bool volatile * is_idle)
{
    sem_wait_log(q->sem_b, is_idle);
    uint64_t index = q->end;
    q->end++;
    if(q->end == q->res_max)
        q->end = 0;
    memcpy(q->res[index], res, q->res_size);
    TREAT(sem_post(q->sem_f));
}

void queue_get(queue_p q, handler_p out_res, bool volatile * is_idle)
{
    sem_wait_log(q->sem_f, is_idle);
    handler_p res = q->res[q->start];
    memcpy(out_res, res, q->res_size);
    q->start++;
    if(q->start == q->res_max)
        q->start = 0;
    TREAT(sem_post(q->sem_b));
}

/* returns true if H ownership returns to caller */
bool queue_unstuck(queue_p q, handler_p h)
{
    if(sem_getvalue_treat(q->sem_f) == 0)
    {
        queue_post(q, h, NULL);
        return false;
    }
    
    if(sem_getvalue_treat(q->sem_b) == 0)
    {
        q->res_free(h, q->res_size);
        queue_get(q, h, NULL);
        return true;
    }

    return true;
}

void queue_free(queue_p q)
{
    handler_p h = malloc(q->res_size);
    assert(h);
    
    while(queue_get_occupancy(q))
    {
        queue_get(q, h, NULL);
        q->res_free(h, q->res_size);
    }
    free(h);

    for(uint64_t i=0; i<q->res_max; i++)
        free(q->res[i]);
    
    free(q->res);
    
    TREAT(sem_destroy(q->sem_f));
    TREAT(sem_destroy(q->sem_b));
}



STRUCT(junc)
{
    uint64_t total;
    uint64_t index;
    queue_p queues;
};

junc_t junc_init(uint64_t total, uint64_t res_max, uint64_t res_size, free_f res_free)
{
    queue_p queues = malloc(total * sizeof(queue_t));
    for(uint64_t i=0; i<total; i++)
        queues[i] = queue_init(res_max, res_size, res_free);

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

void junc_get(junc_p junc, handler_p out_res, bool volatile * is_idle)
{
    uint64_t index = junc->index;
    junc->index++;
    if(junc->index == junc->total)
        junc->index = 0;
    queue_get(&junc->queues[index], out_res, is_idle);
}

void junc_post(junc_p junc, handler_p res, bool volatile * is_idle)
{
    uint64_t index = junc->index;
    junc->index++;
    if(junc->index == junc->total)
        junc->index = 0;
    queue_post(&junc->queues[index], res, is_idle);
}

#endif