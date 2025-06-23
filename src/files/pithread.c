#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../mods/macros/struct.h"
#include "../../mods/clu/header.h"

#include "../../lib/float/header.h"

#include "time.c"



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

void line_post_response(line_p l, float_num_t flt_res)
{
    sem_wait(&l->sem_b);
    pthread_mutex_lock(&l->mut);
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

float_num_t line_get_response(line_p l)
{
    sem_wait(&l->sem_f);
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
    line_p line_a_b;
    float_num_t flt_m_3_8;
    float_num_t flt_1_4;
};

handler_p thread_a(handler_p args)
{
    thread_a_args_p _args = args;
    for(uint64_t i=1; ; i++)
    {
        float_num_t flt = float_num_div(
            float_num_copy(_args->flt_m_3_8),
            float_num_wrap(i, _args->size)
        );
        flt = float_num_add(flt, float_num_copy(_args->flt_1_4));

        line_post_response(_args->line_a_b, flt);
    }

    return NULL;
}

STRUCT(thread_b_args)
{
    uint64_t size;
    line_p line_a_b;
    line_p line_b_d;
};

handler_p thread_b(handler_p args)
{
    thread_b_args_p _args = args;
    float_num_t flt_b = float_num_wrap(6, _args->size);
    for(uint64_t i=1; ; i++)
    {
        float_num_t flt_a = line_get_response(_args->line_a_b);
        flt_b = float_num_mul(flt_b, flt_a);

        line_post_response(_args->line_b_d, float_num_copy(flt_b));
    }

    return NULL;
}

STRUCT(thread_c_args)
{
    uint64_t size;
    line_p line_c_d;
    float_num_t flt_1;
    float_num_t flt_m_1_2;
};

handler_p thread_c(handler_p args)
{
    thread_c_args_p _args = args;
    for(uint64_t i=1; ; i++)
    {
        float_num_t flt = float_num_div(
            float_num_copy(_args->flt_1),
            float_num_wrap(2 * i + 1, _args->size)
        );
        flt = float_num_add(flt, float_num_copy(_args->flt_m_1_2));

        line_post_response(_args->line_c_d, flt);
    }

    return NULL;
}

STRUCT(thread_d_args)
{
    uint64_t size;
    line_p line_b_d;
    line_p line_c_d;
    line_p line_d_pi;
};

handler_p thread_d(handler_p args)
{
    thread_d_args_p _args = args;
    for(uint64_t i=1; ; i++)
    {
        float_num_t flt_b = line_get_response(_args->line_b_d);
        float_num_t flt_c = line_get_response(_args->line_c_d);
        flt_b = float_num_mul(flt_b, flt_c);
        line_post_response(_args->line_d_pi, flt_b);
    }

    return NULL;
}

STRUCT(thread_pi_args)
{
    uint64_t size;
    line_p line_d_pi;
};

handler_p thread_pi(handler_p args)
{
    thread_pi_args_p _args = args;
    float_num_t flt_pi = float_num_wrap(3, _args->size);
    for(uint64_t i=1; ; i++)
    {
        float_num_t flt_d = line_get_response(_args->line_d_pi);
        flt_pi = float_num_add(flt_pi, flt_d);
        
        if(i%1000 == 0)
        {
            printf("\n");
            float_num_display_dec(flt_pi);
        }
    }

    return NULL;
}

void pi_threads()
{
    uint64_t size = 1000;

    line_t line_a_b = line_init(50);
    float_num_t flt_m_3_8 = float_num_div(
        float_num_wrap(-3, size),
        float_num_wrap(8, size)
    );
    float_num_t flt_1_4 = float_num_div(
        float_num_wrap(1, size),
        float_num_wrap(4, size)
    );
    thread_a_args_t args_a = (thread_a_args_t)
    {
        .size = size,
        .line_a_b = &line_a_b,
        .flt_m_3_8 = flt_m_3_8,
        .flt_1_4 = flt_1_4
    };
    pthread_t pid_a = pthread_launch(thread_a, &args_a);

    line_t line_b_d = line_init(50);
    thread_b_args_t args_b = (thread_b_args_t)
    {
        .size = size,
        .line_a_b = &line_a_b,
        .line_b_d = &line_b_d
    };
    pthread_launch(thread_b, &args_b);
    
    line_t line_c_d = line_init(50);
    float_num_t flt_1 = float_num_wrap(1, size);
    float_num_t flt_m_1_2 = float_num_div (
        float_num_wrap(-1, size),
        float_num_wrap(2, size)
    );
    thread_c_args_t args_c = (thread_c_args_t)
    {
        .size = size,
        .line_c_d = &line_c_d,
        .flt_1 = flt_1,
        .flt_m_1_2 = flt_m_1_2
    };
    pthread_launch(thread_c, &args_c);

    line_t line_d_pi = line_init(50);
    thread_d_args_t args_d = (thread_d_args_t)
    {
        .size = size,
        .line_b_d = &line_b_d,
        .line_c_d = &line_c_d,
        .line_d_pi = &line_d_pi
    };
    pthread_launch(thread_d, &args_d);

    thread_pi_args_t args_pi = (thread_pi_args_t)
    {
        .size = size,
        .line_d_pi = &line_d_pi
    };
    pthread_launch(thread_pi, &args_pi);

    pthread_join(pid_a, NULL);
} 
