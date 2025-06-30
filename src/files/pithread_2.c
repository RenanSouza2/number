#include "time.c"
#include "pthread_struct.c"

#include "../../mods/clu/header.h"

#include "../../lib/fix/header.h"
#include "../../lib/num/header.h"
#include "../../lib/num/struct.h"
#include "../../lib/sig/header.h"



void display_table_line_break(uint64_t n)
{
    printf("\n ----------------");
    for(uint64_t i=0; i<n; i++)
        printf("--------");
}



STRUCT(pi_2_thread_a_args)
{
    uint64_t size;
    queue_p queue_a_b;
    uint64_t id;
    uint64_t layers;
    uint64_t batch_size;
    fix_num_t a0;
    bool volatile keep_going;
    bool volatile is_idle;
    uint64_t volatile state;
};

handler_p pi_2_thread_a(handler_p _args)
{
    dbg("a");
    pi_2_thread_a_args_p args = _args;
    fix_num_t fix_a = args->a0;
    uint64_t jump = args->layers * args->batch_size;
    fix_num_t fix_a_batch[args->batch_size];
    uint64_t del = 0;
    for(uint64_t i = args->id + args->layers; args->keep_going; i += jump)
    {
        del++;
        for(uint64_t j=0; j<args->batch_size; j++)
        {
            uint64_t index = i + args->layers * j;

            args->state = 0;
            sig_num_t sig_a = sig_num_wrap((int64_t)2 * index - 3);
            sig_num_t sig_b = sig_num_wrap((int64_t)index);
            for(uint64_t k=1; k<args->layers; k++)
            {
                sig_a = sig_num_mul(sig_a, sig_num_wrap((int64_t)2 * (index - k) - 3));
                sig_b = sig_num_mul(sig_b, sig_num_wrap((int64_t)(index - k)));
            }

            args->state = 1;
            fix_a = fix_num_mul_sig(fix_a, sig_a);

            args->state = 2;
            fix_a = fix_num_shr(fix_a, 3 * args->layers);
            
            args->state = 3;
            fix_a = fix_num_div_sig(fix_a, sig_b);
            args->state = 0;
            fix_a_batch[j] = fix_num_copy(fix_a);

            // printf("\na: ");fix_num_display_dec(fix_a);
        }
        
        args->state = 4;
        queue_post(args->queue_a_b, &fix_a_batch, &args->is_idle);
    }
    fix_num_free(fix_a);

    return NULL;
}

STRUCT(pi_2_thread_b_args)
{
    uint64_t size;
    queue_p queue_a_b;
    queue_p queue_b_pi;
    uint64_t id;
    uint64_t layers;
    uint64_t batch_size;
    bool volatile keep_going;
    bool volatile is_idle;
    uint64_t volatile state;
};

handler_p pi_2_thread_b(handler_p _args)
{
    dbg("b");
    pi_2_thread_b_args_p args = _args;
    uint64_t jump = args->layers * args->batch_size;
    fix_num_t fix_a_batch[args->batch_size], fix_b_batch[args->batch_size];
    for(uint64_t i = args->id + args->layers; args->keep_going; i += jump)
    {
        args->state = 0;
        queue_get(args->queue_a_b, &fix_a_batch, &args->is_idle);
        args->state = 1;
        
        if(!args->keep_going)
        {
            for(uint64_t j=0; j<args->batch_size; j++)
                fix_num_free(fix_a_batch[j]);

            break;
        }

        for(uint64_t j=0; j<args->batch_size; j++)
        {
            uint64_t index = i + args->layers * j;

            args->state = 2;
            fix_num_t fix_b = fix_num_mul_sig(fix_a_batch[j], sig_num_wrap((int64_t)1 - 2 * index));
            args->state = 3;
            fix_b = fix_num_div_sig(fix_b, sig_num_wrap((int64_t)4 * index + 2));
            args->state = 4;
            fix_b_batch[j] = fix_b;
            
            // printf("\nb: ");fix_num_display_dec(fix_b);
        }

        args->state = 5;
        queue_post(args->queue_b_pi, &fix_b_batch, &args->is_idle);
    }

    return NULL;
}

STRUCT(pi_2_thread_pi_args)
{
    uint64_t size;
    junc_p junc_b_pi;
    uint64_t layers;
    uint64_t batch_size;
    fix_num_t pi0;
    fix_num_t res;
    bool volatile is_idle;
    uint64_t volatile state;
    uint64_t volatile total;
};

handler_p pi_2_thread_pi(handler_p _args)
{
    dbg("pi");
    pi_2_thread_pi_args_p args = _args;
    fix_num_t fix_pi = args->pi0;
    fix_num_t fix_b_batch[args->batch_size];
    for(uint64_t i=1; ; i++)
    {
        args->total++;
    
        fix_num_t fix_b = fix_num_wrap(0, args->size);
        for(uint64_t j=0; j<args->layers; j++)
        {
            args->state = 1;
            junc_get(args->junc_b_pi, &fix_b_batch, &args->is_idle);
            args->state = 2;
            for(uint64_t k=0; k<args->batch_size; k++)
                fix_b = fix_num_add(fix_b, fix_b_batch[k]);
        }

        args->state = 3;
        uint64_t tam = 100;
        if(i%(tam) == 0)
        {
            uint64_t done = args->size - fix_b.sig.num->count;
            fprintf(stderr, "\npgr: %lu / %lu", done / tam, args->size / tam);
        }
        
        if(fix_num_is_zero(fix_b))
        {
            fix_num_free(fix_b);
            break;
        }
        
        args->state = 4;
        fix_pi = fix_num_add(fix_pi, fix_b);
    }

    args->res = fix_pi;
    return NULL;
}



handler_p matrix_alloc(uint64_t n_i, uint64_t n_j, uint64_t size)
{
    handler_p *m = malloc(n_i * sizeof(handler_p));
    assert(m);

    for(uint64_t i=0; i<n_i; i++)
    {
        m[i] = calloc(n_j, size);
        assert(m[i]);
    }

    return m;
}

void matrix_free(handler_p *m, uint64_t n_i)
{
    for(uint64_t i=0; i<n_i; i++)
        free(m[i]);

    free(m);
}

STRUCT(pi_2_monitor_idle_res)
{
    uint64_t n_threads;
    uint64_t layers;
    uint64_t *count;
    uint64_t count_pi;
    uint64_t total;
};

pi_2_monitor_idle_res_t pi_2_monitor_idle_res_create(uint64_t n_threads, uint64_t layers)
{
    uint64_t *count = calloc(layers, sizeof(uint64_t));
    assert(count);

    return (pi_2_monitor_idle_res_t)
    {
        .n_threads = n_threads,
        .layers = layers,
        .count = count
    };
}

void pi_2_monitor_idle_res_free(pi_2_monitor_idle_res_t res)
{
    free(res.count);
}

STRUCT(pi_2_monitor_idle_args)
{
    bool volatile ***is_idle;
    pi_2_monitor_idle_res_t res;
    bool keep_going;
};

pi_2_monitor_idle_args_t pi_2_monitor_idle_args_create(uint64_t n_threads, uint64_t layers)
{
    bool volatile ***is_idle = matrix_alloc(n_threads, layers, sizeof(bool*));
    return (pi_2_monitor_idle_args_t)
    {
        .is_idle = is_idle,
        .res = pi_2_monitor_idle_res_create(n_threads, layers),
        .keep_going = true
    };
}

 void pi_2_monitor_idle_args_free(pi_2_monitor_idle_args_t args)
 {
    matrix_free((handler_p*)args.is_idle, args.res.n_threads);
    pi_2_monitor_idle_res_free(args.res);
 }

handler_p pi_2_monitor_idle(handler_p _args)
{
    pi_2_monitor_idle_args_p args = _args;
    pi_2_monitor_idle_res_p res = &args->res;
    for(res->total=0; args->keep_going; res->total++)
    {
        for(uint64_t i=0; i<res->n_threads; i++)
        for(uint64_t j=0; j<res->layers; j++)
            res->count[i] += *(args->is_idle[i][j]);

        usleep(10000);
    }
    return NULL;
}

void pi_2_monitor_idle_treat_res(pi_2_monitor_idle_args_t args)
{
    pi_2_monitor_idle_res_t res = args.res;
    printf("\n");
    printf("\n\t\t| use rate\t|");
    display_table_line_break(2);
    printf("\n| thread_a\t| \t%3.f %%\t|", 100 - 100.0 * res.count[0] / (res.total * res.layers));
    printf("\n| thread_b\t| \t%3.f %%\t|", 100 - 100.0 * res.count[1] / (res.total * res.layers));
    printf("\n| thread_pi\t| \t%3.f %%\t|", 100 - 100.0 * res.count[2] / (res.total * res.layers));

    pi_2_monitor_idle_args_free(args);
}



STRUCT(pi_2_monitor_state_res)
{
    uint64_t n_threads;
    uint64_t layers;
    uint64_t max_total_state;
    uint64_t **count;
    uint64_t total;
};

pi_2_monitor_state_res_t pi_2_monitor_state_res_create(
    uint64_t n_threads,
    uint64_t layers,
    uint64_t max_total_state
)
{
    uint64_t **count = matrix_alloc(n_threads,  max_total_state, sizeof(uint64_t));
    return (pi_2_monitor_state_res_t)
    {
        .n_threads = n_threads,
        .layers = layers,
        .max_total_state = max_total_state,
        .count = count
    };
}

void pi_2_monitor_state_res_free(pi_2_monitor_state_res_t res)
{
    matrix_free((handler_p*)res.count, res.n_threads);
}

STRUCT(pi_2_monitor_state_args)
{
    uint64_t volatile ***state;
    pi_2_monitor_state_res_t res;
    bool keep_going;
};

pi_2_monitor_state_args_t pi_2_monitor_state_args_create(
    uint64_t n_threads,
    uint64_t layers,
    uint64_t max_total_state
)
{
    uint64_t volatile ***state = matrix_alloc(n_threads, layers, sizeof(uint64_t*));
    return (pi_2_monitor_state_args_t)
    {
        .state = state,
        .res = pi_2_monitor_state_res_create(n_threads, layers, max_total_state),
        .keep_going = true
    };
}

void pi_2_monitor_state_args_free(pi_2_monitor_state_args_t args)
{
    matrix_free((handler_p*)args.state, args.res.n_threads);
    pi_2_monitor_state_res_free(args.res);
}

handler_p pi_2_monitor_state(handler_p _args)
{
    pi_2_monitor_state_args_p args = _args;
    pi_2_monitor_state_res_p res = &args->res;
    for(res->total = 0; args->keep_going; res->total++)
    {
        for(uint64_t i=0; i<res->n_threads; i++)
        for(uint64_t j=0; j<res->layers; j++)
        {
            uint64_t state = *(args->state[i][j]);
            assert(state < res->max_total_state);
            res->count[i][state]++;
        }

        sleep(1);
    }
    return NULL;
}

void pi_2_monitor_state_treat_res(pi_2_monitor_state_args_t args)
{
    pi_2_monitor_state_res_t res = args.res;
    printf("\n");
    printf("\n \tstate\t|");
    for(uint64_t i=0; i<res.max_total_state; i++)
        printf(" %lu\t|", i);
    display_table_line_break(res.max_total_state);
    printf("\n| thread_a\t|");
    for(uint64_t i=0; i<res.max_total_state; i++)
        printf(" %5.1f\t|", 100.0 * res.count[0][i] / (res.total * res.layers));
    printf("\n| thread_b\t|");
    for(uint64_t i=0; i<res.max_total_state; i++)
        printf(" %5.1f\t|", 100.0 * res.count[1][i] / (res.total * res.layers));
    printf("\n| thread_pi\t|");
    for(uint64_t i=0; i<res.max_total_state; i++)
        printf(" %5.1f\t|", 100.0 * res.count[2][i] / (res.total * res.layers));

    pi_2_monitor_state_args_free(args);
}




STRUCT(pi_2_monitor_queue_res)
{
    uint64_t n_queues;
    uint64_t layers;
    uint64_t queue_size;
    uint64_t *count;
    uint64_t total;
};

pi_2_monitor_queue_res_t pi_2_monitor_queue_res_create(
    uint64_t n_queues,
    uint64_t layers,
    uint64_t queue_size
)
{
    uint64_t *count = calloc(n_queues, sizeof(uint64_t*));
    assert(count);

    return (pi_2_monitor_queue_res_t)
    {
        .n_queues = n_queues,
        .layers = layers,
        .queue_size = queue_size,
        .count = count
    };
}

void pi_2_monitor_queue_res_free(pi_2_monitor_queue_res_t res)
{
    free(res.count);
}

STRUCT(pi_2_monitor_queue_args)
{
    queue_p **queues;
    pi_2_monitor_queue_res_t res;
    bool keep_going;
};

pi_2_monitor_queue_args_t pi_2_monitor_queue_args_create(
    uint64_t n_queues,
    uint64_t layers,
    uint64_t queue_size
)
{
    queue_p **queues = matrix_alloc(n_queues, layers, sizeof(queue_p));
    return (pi_2_monitor_queue_args_t)
    {
        .queues = queues,
        .res = pi_2_monitor_queue_res_create(n_queues, layers, queue_size),
        .keep_going = true
    };
}

 void pi_2_monitor_queue_args_free(pi_2_monitor_queue_args_t args)
 {
    matrix_free((handler_p*)args.queues, args.res.n_queues);
    pi_2_monitor_queue_res_free(args.res);
 }

handler_p pi_2_monitor_queue(handler_p _args)
{
    pi_2_monitor_queue_args_p args = _args;
    pi_2_monitor_queue_res_p res = &args->res;
    for(res->total = 0; args->keep_going; res->total++)
    {
        for(uint64_t i=0; i<res->n_queues; i++)
        for(uint64_t j=0; j<res->layers; j++)
        {
            uint64_t val = queue_get_value(args->queues[i][j]);
            res->count[i] += val;
        }

        usleep(10000);
    }

    return NULL;
}

void pi_2_monitor_queue_treat_res(pi_2_monitor_queue_args_t args)
{
    pi_2_monitor_queue_res_t res = args.res;
    printf("\n");
    printf("\n\t\t| occupancy\t|");
    display_table_line_break(2);
    printf("\n| queue_a_b\t| \t%5.1f\t|", 100.0 * res.count[0] / (res.total * res.layers * res.queue_size));
    printf("\n| queue_b_pi\t| \t%5.1f\t|", 100.0 * res.count[1] / (res.total * res.layers * res.queue_size));

    pi_2_monitor_queue_args_free(args);
}



void pi_2_res_free(handler_p h, uint64_t res_size)
{
    uint64_t batch_size = res_size / sizeof(fix_num_t);
    for(uint64_t i=0; i<batch_size; i++)
        fix_num_free(((fix_num_p)h)[i]);
}



fix_num_t pi_threads_2_calc(uint64_t size, uint64_t thread_0, bool monitoring)
{
    uint64_t layers = 3;
    uint64_t queue_size = 5;
    uint64_t batch_size = 20;
    uint64_t max_total_state = 6;

    pi_2_thread_a_args_t args_a[layers];
    pi_2_thread_b_args_t args_b[layers];

    queue_t queue_a_b[layers];

    junc_t junc_b_pi = junc_init(layers, queue_size, batch_size * sizeof(fix_num_t), pi_2_res_free);

    pthread_t pid_a[layers];
    pthread_t pid_b[layers];

    fix_num_t a0[layers];
    a0[0] = fix_num_wrap(6, size);

    fix_num_t fix_a = fix_num_wrap(6, size);
    fix_num_t pi0 = fix_num_wrap(3, size);
    for(uint64_t i=1; i<layers; i++)
    {
        fix_a = fix_num_mul_sig(fix_a, sig_num_wrap((int64_t)2 * i - 3));
        fix_a = fix_num_div_sig(fix_a, sig_num_wrap((int64_t)8 * i));
        a0[i] = fix_num_copy(fix_a);
        
        fix_num_t fix_d = fix_num_copy(fix_a);
        fix_d = fix_num_mul_sig(fix_d, sig_num_wrap((int64_t)1 - 2 * i));
        fix_d = fix_num_div_sig(fix_d, sig_num_wrap((int64_t)4 * i + 2));

        pi0 = fix_num_add(pi0, fix_d);
    }
    fix_num_free(fix_a);

    for(uint64_t i=0; i<layers; i++)
    {
        queue_a_b[i] = queue_init(queue_size, batch_size * sizeof(fix_num_t), pi_2_res_free);
        args_a[i] = (pi_2_thread_a_args_t)
        {
            .size = size,
            .queue_a_b = &queue_a_b[i],
            .id = i,
            .layers = layers,
            .batch_size = batch_size,
            .a0 = a0[i],
            .keep_going = true,
            .is_idle = false
        };
        pid_a[i] = pthread_launch(pi_2_thread_a, &args_a[i]);
        pthread_lock(pid_a[i], thread_0 + 2 * i);

        args_b[i] = (pi_2_thread_b_args_t)
        {
            .size = size,
            .queue_a_b = &queue_a_b[i],
            .queue_b_pi = &junc_b_pi.queues[i],
            .id = i,
            .layers = layers,
            .batch_size = batch_size,
            .keep_going = true,
            .is_idle = false
        };
        pid_b[i] = pthread_launch(pi_2_thread_b, &args_b[i]);
        pthread_lock(pid_b[i], thread_0 + 2 * i + 1);
    }

    pi_2_thread_pi_args_t args_pi = (pi_2_thread_pi_args_t)
    {
        .size = size,
        .junc_b_pi = &junc_b_pi,
        .layers = layers,
        .batch_size = batch_size,
        .pi0 = pi0,
        .is_idle = false
    };
    pthread_t pid_pi = pthread_launch(pi_2_thread_pi, &args_pi);
    pthread_lock(pid_pi, thread_0 + 6);

    pi_2_monitor_idle_args_t args_monitor_idle;
    pi_2_monitor_state_args_t args_monitor_state;
    pi_2_monitor_queue_args_t args_monitor_queue;
    pthread_t pid_monitor_idle = 0;
    pthread_t pid_monitor_state = 0;
    pthread_t pid_monitor_queue = 0;
    if(monitoring)
    {
        args_monitor_idle = pi_2_monitor_idle_args_create(3, layers);
        for(uint64_t i=0; i<layers; i++)
        {
            args_monitor_idle.is_idle[0][i] = &args_a[i].is_idle;
            args_monitor_idle.is_idle[1][i] = &args_b[i].is_idle;
            args_monitor_idle.is_idle[2][i] = &args_pi.is_idle;
        }
        pid_monitor_idle = pthread_launch(pi_2_monitor_idle, &args_monitor_idle);

        args_monitor_state = pi_2_monitor_state_args_create(3, layers, max_total_state);
        for(uint64_t i=0; i<layers; i++)
        {
            args_monitor_state.state[0][i] = &args_a[i].state;
            args_monitor_state.state[1][i] = &args_b[i].state;
            args_monitor_state.state[2][i] = &args_pi.state;
        }
        pid_monitor_state = pthread_launch(pi_2_monitor_state, &args_monitor_state);

        args_monitor_queue = pi_2_monitor_queue_args_create(2, layers, queue_size);
        for(uint64_t i=0; i<layers; i++)
        {
            args_monitor_queue.queues[0][i] = &queue_a_b[i];
            args_monitor_queue.queues[1][i] = &junc_b_pi.queues[i];
        }
        pid_monitor_queue = pthread_launch(pi_2_monitor_queue, &args_monitor_queue);
    }

    pthread_wait(pid_pi);
    fix_num_t fix_pi = args_pi.res;

    for(uint64_t i=0; i<layers; i++)
    {
        args_a[i].keep_going = false;
        args_b[i].keep_going = false;
    }

    if(monitoring)
    {
        args_monitor_idle.keep_going = false;
        args_monitor_state.keep_going = false;
        args_monitor_queue.keep_going = false;
    }

    for(uint64_t i=0; i<layers; i++)
    {
        fix_num_t fix[batch_size];
        for(uint64_t j=0; j<batch_size; j++)
            fix[j] = fix_num_wrap(0, 2);
        
        if(!queue_unstuck(&queue_a_b[i], &fix))
        {
            for(uint64_t j=0; j<batch_size; j++)
                fix[j] = fix_num_wrap(0, 2);
        }
        if(queue_unstuck(&junc_b_pi.queues[i], &fix))
        {
            for(uint64_t j=0; j<batch_size; j++)
                fix_num_free(fix[j]);
        }
    }

    for(uint64_t i=0; i<layers; i++)
    {
        pthread_wait(pid_a[i]);
        pthread_wait(pid_b[i]);
    }

    if(monitoring)
    {
        pthread_wait(pid_monitor_idle);
        pthread_wait(pid_monitor_state);
        pthread_wait(pid_monitor_queue);
    }

    for(uint64_t i=0; i<layers; i++)
    {
        queue_free(&queue_a_b[i]);
    }
    junc_free(&junc_b_pi);

    if(monitoring)
    {
        pi_2_monitor_idle_treat_res(args_monitor_idle);
        pi_2_monitor_state_treat_res(args_monitor_state);
        pi_2_monitor_queue_treat_res(args_monitor_queue);

        printf("\n");
        printf("\n\t\t| total\t|");
        display_table_line_break(1);
        printf("\n| total_pi\t| %lu\t|", args_pi.total);
    }

    return fix_pi;
}

void pi_threads_2(uint64_t size, uint64_t thread_0, bool monitoring)
{
    fix_num_t fix_pi = pi_threads_2_calc(size, thread_0, monitoring);
    if(!monitoring)
    {
        printf("\n\n");
        fix_num_display_dec(fix_pi);
    }
    fix_num_free(fix_pi);
}



void pi_2_time_2(uint64_t thread_0)
{
    for(uint64_t size = 100; size < 5000; size += 100)
    {
        printf("\ni: %4lu\t", size);
        uint64_t begin = get_time();
        pi_threads_2_calc(size, thread_0, false);
        uint64_t end = get_time();
        uint64_t time = end - begin;
        printf("\t%10.3f", time / 1e9);
    }
}
