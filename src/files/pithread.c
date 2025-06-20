#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../mods/macros/struct.h"

#include "../../lib/float/header.h"



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



STRUCT(counter)
{
    pthread_mutex_t mut;
    uint64_t counter;
};

counter_t counter_init(uint64_t value)
{
    pthread_mutex_t mut;
    pthread_mutex_init(&mut, NULL);
    return (counter_t)
    {
        .mut = mut,
        .counter = value
    };
}

uint64_t counter_consume(counter_p c)
{
    pthread_mutex_lock(&c->mut);
    uint64_t res = c->counter;
    c->counter = res + 1;
    pthread_mutex_unlock(&c->mut);
    return res;
}



STRUCT(line)
{
    sem_t sem_f, sem_b;
    counter_t req;
    pthread_mutex_t mut;
    uint64_t index_expected;
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
        .req = counter_init(1),
        .index_expected = 1,
        .first = NULL,
        .last = NULL
    };
}

uint64_t line_get_request(line_p l)
{
    sem_wait(&l->sem_b);
    return counter_consume(&l->req);
}

void line_post_response(line_p l, uint64_t index, float_num_t flt_res)
{
    while(l->index_expected != index)
        usleep(10000);

    pthread_mutex_lock(&l->mut);
    l->index_expected++;
    list_float_p list = list_create(flt_res);
    if(l->last)
    {
        l->last = l->last->next = list;
    }
    else
    {
        l->last = l->first = list;
    }
    pthread_mutex_unlock(&l->mut);
    sem_post(&l->sem_f);
}

float_num_t line_get_response(line_p l, pthread_f fn, handler_p args)
{
    if(sem_trywait(&l->sem_f) == -1)
    {
        pthread_launch(fn, args);
        sem_wait(&l->sem_f);
    }

    pthread_mutex_lock(&l->mut);
    list_float_p list = l->first;
    l->first = list->next;
    if(l->first == NULL)
        l->last = NULL;
    pthread_mutex_unlock(&l->mut);
    sem_post(&l->sem_b);

    float_num_t res = list->flt_res;
    free(list);
    return res;
}



STRUCT(thread_a_args)
{
    uint64_t size;
    line_p line;
    float_num_t flt_m_3_8;
    float_num_t flt_1_4;
};

handler_p thread_a(handler_p args)
{
    thread_a_args_p _args = args;
    for(;;)
    {
        uint64_t req = line_get_request(_args->line);

        float_num_t flt = float_num_div(
            float_num_copy(_args->flt_m_3_8),
            float_num_wrap(req, _args->size)
        );
        flt = float_num_add(flt, float_num_copy(_args->flt_1_4));

        line_post_response(_args->line, req, flt);
    }

    return NULL;
}

handler_p thread_b(handler_p)
{
    uint64_t size = 2;
    line_t line = line_init(50);
    float_num_t flt_m_3_8 = float_num_div(
        float_num_wrap(-3, size),
        float_num_wrap(8, size)
    );
    float_num_t flt_1_4 = float_num_div(
        float_num_wrap(1, size),
        float_num_wrap(4, size)
    );

    thread_a_args_t args = (thread_a_args_t)
    {
        .line = &line,
        .size = size,
        .flt_m_3_8 = flt_m_3_8,
        .flt_1_4 = flt_1_4
    };

    float_num_t flt_b = float_num_wrap(6, size);
    for(uint64_t i=1; i<10; i++)
    {
        float_num_t flt_a = line_get_response(&line, thread_a, &args);

        flt_b = float_num_mul(flt_b, flt_a);

        printf("\n");
        float_num_display(flt_b);
    }

    return NULL;
}

void verify_b()
{
    uint64_t size = 2;
    float_num_t flt_m_3_8 = float_num_div(
        float_num_wrap(-3, size),
        float_num_wrap(8, size)
    );
    float_num_t flt_1_4 = float_num_div(
        float_num_wrap(1, size),
        float_num_wrap(4, size)
    );
    float_num_t flt_b = float_num_wrap(6, size);
    for(uint64_t i=1; i<10; i++)
    {
        float_num_t flt_tmp = float_num_div(
            float_num_copy(flt_m_3_8),
            float_num_wrap(i, size)
        );
        flt_tmp = float_num_add(flt_tmp, float_num_copy(flt_1_4));
        flt_b = float_num_mul(flt_b, flt_tmp);

        printf("\n");
        float_num_display(flt_b);
    }
}