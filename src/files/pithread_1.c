#include <unistd.h>

#include "time.c"
#include "pthread_struct.c"

#include "../../lib/float/header.h"
#include "../../lib/num/header.h"
#include "../../lib/sig/header.h"



STRUCT(pi_1_thread_a_args)
{
    uint64_t size;
    queue_p queue_a_b;
    uint64_t id;
    uint64_t layers;
    bool keep_going;
    bool is_idle;
};

handler_p pi_1_thread_a(handler_p _args)
{
    pi_1_thread_a_args_p args = _args;
    for(uint64_t i = args->id + args->layers; args->keep_going; i += args->layers)
    {
        sig_num_t sig_1 = sig_num_wrap((int64_t)2*i - 3);
        sig_num_t sig_2 = sig_num_wrap(8 * i);
        for(uint64_t j = 1; j < args->layers; j++)
        {
            sig_1 = sig_num_mul(sig_1, sig_num_wrap((int64_t)2 * i - 3 - 2 * j));
            sig_2 = sig_num_mul(sig_2, sig_num_wrap(8 * (i - j)));
        }
        float_num_t flt_a = float_num_div(
            float_num_wrap_sig(sig_1, args->size),
            float_num_wrap_sig(sig_2, args->size)
        );

        queue_post(args->queue_a_b, &flt_a, &args->is_idle);
    }

    return NULL;
}

STRUCT(pi_1_thread_b_args)
{
    uint64_t size;
    queue_p queue_a_b;
    queue_p queue_b_d;
    uint64_t id;
    uint64_t layers;
    float_num_t b0;
    bool keep_going;
    bool is_idle;
};

handler_p pi_1_thread_b(handler_p _args)
{
    pi_1_thread_b_args_p args = _args;
    float_num_t flt_b = args->b0;
    for(uint64_t i = args->id + args->layers; args->keep_going; i += args->layers)
    {
        float_num_t flt_a;
        queue_get(args->queue_a_b, &flt_a, &args->is_idle);
        if(!args->keep_going)
        {
            float_num_free(flt_a);
            break;
        }

        flt_b = float_num_mul(flt_b, flt_a);

        float_num_t flt_copy = float_num_copy(flt_b);
        queue_post(args->queue_b_d, &flt_copy, &args->is_idle);
    }
    float_num_free(flt_b);

    return NULL;
}

STRUCT(pi_1_thread_c_args)
{
    uint64_t size;
    queue_p queue_c_d;
    float_num_t flt_1;
    float_num_t flt_m_1_2;
    uint64_t id;
    uint64_t layers;
    bool keep_going;
    bool is_idle;
};

handler_p pi_1_thread_c(handler_p _args)
{
    pi_1_thread_c_args_p args = _args;
    for(uint64_t i = args->id + args->layers; args->keep_going; i += args->layers)
    {
        float_num_t flt = float_num_div(
            float_num_copy(args->flt_1),
            float_num_wrap(2 * i + 1, args->size)
        );
        flt = float_num_add(flt, float_num_copy(args->flt_m_1_2));

        queue_post(args->queue_c_d, &flt, &args->is_idle);
    }

    return NULL;
}

STRUCT(pi_1_thread_d_args)
{
    uint64_t size;
    queue_p queue_b_d;
    queue_p queue_c_d;
    queue_p queue_d_pi;
    uint64_t id;
    uint64_t layers;
    bool keep_going;
    bool is_idle;
};

handler_p pi_1_thread_d(handler_p _args)
{
    pi_1_thread_d_args_p args = _args;
    for(uint64_t i = args->id + args->layers; args->keep_going; i += args->layers)
    {
        float_num_t flt_b, flt_c;
        queue_get(args->queue_b_d, &flt_b, &args->is_idle);
        queue_get(args->queue_c_d, &flt_c, &args->is_idle);
        if(!args->keep_going)
        {
            float_num_free(flt_b);
            float_num_free(flt_c);
            break;
        }

        float_num_t flt_d = float_num_mul(flt_b, flt_c);

        queue_post(args->queue_d_pi, &flt_d, &args->is_idle);
    }

    return NULL;
}

STRUCT(pi_1_thread_pi_args)
{
    uint64_t size;
    junc_p junc_d_pi;
    uint64_t layers;
    float_num_t pi0;
    float_num_t res;
    bool is_idle;
};

handler_p pi_1_thread_pi(handler_p _args)
{
    pi_1_thread_pi_args_p args = _args;
    float_num_t flt_pi = args->pi0;
    for(uint64_t i=args->layers; ; i++)
    {
        float_num_t flt_d;
        junc_get(args->junc_d_pi, &flt_d, &args->is_idle);

        if(i%1000 == 0)
            fprintf(stderr, "\nexp: %ld", -(flt_d.size + flt_d.exponent));

        if(!float_num_safe_add(flt_pi, flt_d))
        {
            float_num_free(flt_d);
            break;
        }

        flt_pi = float_num_add(flt_pi, flt_d);
    }

    args->res = flt_pi;
    return NULL;
}



STRUCT(pi_1_monitor_thread_res)
{
    uint64_t *count_a;
    uint64_t *count_b;
    uint64_t *count_c;
    uint64_t *count_d;
    uint64_t count_pi;
    uint64_t total;
};

pi_1_monitor_thread_res_t pi_1_monitor_thread_res_create(uint64_t layers)
{
    uint64_t *count_a = calloc(layers, sizeof(uint64_t));
    uint64_t *count_b = calloc(layers, sizeof(uint64_t));
    uint64_t *count_c = calloc(layers, sizeof(uint64_t));
    uint64_t *count_d = calloc(layers, sizeof(uint64_t));
    assert(count_a);
    assert(count_b);
    assert(count_c);
    assert(count_d);

    return (pi_1_monitor_thread_res_t)
    {
        .count_a = count_a,
        .count_b = count_b,
        .count_c = count_c,
        .count_d = count_d,
    };
}

void pi_1_monitor_thread_res_free(pi_1_monitor_thread_res_t res)
{
    free(res.count_a);
    free(res.count_b);
    free(res.count_c);
    free(res.count_d);
}

STRUCT(pi_1_monitor_thread_args)
{
    uint64_t layers;
    bool **a_is_idle;
    bool **b_is_idle;
    bool **c_is_idle;
    bool **d_is_idle;
    bool *pi_is_idle;
    pi_1_monitor_thread_res_t res;
    bool keep_going;
};

pi_1_monitor_thread_args_t pi_1_monitor_thread_args_create(uint64_t layers)
{
    bool **a_is_idle = malloc(layers * sizeof(bool*));
    bool **b_is_idle = malloc(layers * sizeof(bool*));
    bool **c_is_idle = malloc(layers * sizeof(bool*));
    bool **d_is_idle = malloc(layers * sizeof(bool*));
    assert(a_is_idle);
    assert(b_is_idle);
    assert(c_is_idle);
    assert(d_is_idle);

    return (pi_1_monitor_thread_args_t)
    {
        .layers = layers,
        .a_is_idle = a_is_idle,
        .b_is_idle = b_is_idle,
        .c_is_idle = c_is_idle,
        .d_is_idle = d_is_idle,
        .res = pi_1_monitor_thread_res_create(layers),
        .keep_going = true
    };
}

 void pi_1_monitor_thread_args_free(pi_1_monitor_thread_args_t args)
 {
    free(args.a_is_idle);
    free(args.b_is_idle);
    free(args.c_is_idle);
    free(args.d_is_idle);
 }

handler_p pi_1_monitor_thread(handler_p _args)
{
    pi_1_monitor_thread_args_p args = _args;
    pi_1_monitor_thread_res_t res = args->res;
    uint64_t total=0;
    for(; args->keep_going; total++)
    {
        for(uint64_t i=0; i<args->layers; i++)
        {
            res.count_a[i] += *(args->a_is_idle[i]);
            res.count_b[i] += *(args->b_is_idle[i]);
            res.count_c[i] += *(args->c_is_idle[i]);
            res.count_d[i] += *(args->d_is_idle[i]);
        }
        res.count_pi += *(args->pi_is_idle);

        sleep(1);
    }

    res.total = total;
    args->res = res;
    return NULL;
}

void pi_1_monitor_thread_treat_res(pi_1_monitor_thread_res_t res, uint64_t layers)
{
    printf("\n| thread_a\t|");
    for(uint64_t i=0; i<layers; i++)
        printf(" %3.f %%\t|", 100 - 100.0 * res.count_a[i] / res.total);
    printf("\n| thread_b\t|");
    for(uint64_t i=0; i<layers; i++)
        printf(" %3.f %%\t|", 100 - 100.0 * res.count_b[i] / res.total);
    printf("\n| thread_c\t|");
    for(uint64_t i=0; i<layers; i++)
        printf(" %3.f %%\t|", 100 - 100.0 * res.count_c[i] / res.total);
    printf("\n| thread_d\t|");
    for(uint64_t i=0; i<layers; i++)
        printf(" %3.f %%\t|", 100 - 100.0 * res.count_d[i] / res.total);
    printf("\n");
    printf("\n| thread_pi\t| %3.f %%\t|", 100 - 100.0 * res.count_pi / res.total);
}


void pi_1_res_free(handler_p h, uint64_t)
{
    float_num_free(*(float_num_p)h);
}

void pi_threads_1(uint64_t size, uint64_t layers, bool monitoring)
{
    pi_1_thread_a_args_t args_a[layers];
    pi_1_thread_b_args_t args_b[layers];
    pi_1_thread_c_args_t args_c[layers];
    pi_1_thread_d_args_t args_d[layers];

    queue_t queue_a_b[layers];
    queue_t queue_b_d[layers];
    queue_t queue_c_d[layers];

    junc_t junc_d_pi = junc_init(layers, 50, sizeof(float_num_t), pi_1_res_free);

    pthread_t pid_a[layers];
    pthread_t pid_b[layers];
    pthread_t pid_c[layers];
    pthread_t pid_d[layers];

    float_num_t b0[layers];

    float_num_t flt_b = float_num_wrap(6, size);
    float_num_t pi0 = float_num_wrap(3, size);
    b0[0] = float_num_copy(flt_b);

    for(uint64_t i=1; i<layers; i++)
    {
        float_num_t flt_a = float_num_div(
            float_num_wrap((int64_t)2 * i - 3, size),
            float_num_wrap((int64_t)8 * i, size)
        );
        flt_b = float_num_mul(flt_b, flt_a);
        b0[i] = float_num_copy(flt_b);

        float_num_t flt_c = float_num_div(
            float_num_wrap((int64_t)1 - 2 * i, size),
            float_num_wrap((int64_t)4 * i + 2, size)
        );
        flt_c = float_num_mul(flt_c, float_num_copy(flt_b));

        pi0 = float_num_add(pi0, flt_c);
    }

    float_num_t flt_1 = float_num_wrap(1, size);
    float_num_t flt_m_1_2 = float_num_div(
        float_num_wrap(-1, size),
        float_num_wrap( 2, size)
    );

    for(uint64_t i=0; i<layers; i++)
    {
        queue_a_b[i] = queue_init(50, sizeof(float_num_t), pi_1_res_free);
        args_a[i] = (pi_1_thread_a_args_t)
        {
            .size = size,
            .queue_a_b = &queue_a_b[i],
            .id = i,
            .layers = layers,
            .keep_going = true,
            .is_idle = false
        };
        pid_a[i] = pthread_launch(pi_1_thread_a, &args_a[i]);

        queue_b_d[i] = queue_init(50, sizeof(float_num_t), pi_1_res_free);
        args_b[i] = (pi_1_thread_b_args_t)
        {
            .size = size,
            .queue_a_b = &queue_a_b[i],
            .queue_b_d = &queue_b_d[i],
            .id = i,
            .layers = layers,
            .b0 = b0[i],
            .keep_going = true,
            .is_idle = false
        };
        pid_b[i] = pthread_launch(pi_1_thread_b, &args_b[i]);

        queue_c_d[i] = queue_init(50, sizeof(float_num_t), pi_1_res_free);
        args_c[i] = (pi_1_thread_c_args_t)
        {
            .size = size,
            .queue_c_d = &queue_c_d[i],
            .flt_1 = flt_1,
            .flt_m_1_2 = flt_m_1_2,
            .id = i,
            .layers = layers,
            .keep_going = true,
            .is_idle = false
        };
        pid_c[i] = pthread_launch(pi_1_thread_c, &args_c[i]);

        args_d[i] = (pi_1_thread_d_args_t)
        {
            .size = size,
            .queue_b_d = &queue_b_d[i],
            .queue_c_d = &queue_c_d[i],
            .queue_d_pi = &junc_d_pi.queues[i],
            .id = i,
            .layers = layers,
            .keep_going = true,
            .is_idle = false
        };
        pid_d[i] = pthread_launch(pi_1_thread_d, &args_d[i]);
    }

    pi_1_thread_pi_args_t args_pi = (pi_1_thread_pi_args_t)
    {
        .size = size,
        .junc_d_pi = &junc_d_pi,
        .layers = layers,
        .pi0 = pi0,
        .is_idle = false
    };
    pthread_t pid_pi = pthread_launch(pi_1_thread_pi, &args_pi);

    pi_1_monitor_thread_args_t args_monitor_thread;
    pthread_t pid_monitor_thread = 0;
    if(monitoring)
    {
        args_monitor_thread = pi_1_monitor_thread_args_create(layers);
        for(uint64_t i=0; i<layers; i++)
        {
            args_monitor_thread.a_is_idle[i] = &args_a[i].is_idle;
            args_monitor_thread.b_is_idle[i] = &args_b[i].is_idle;
            args_monitor_thread.c_is_idle[i] = &args_c[i].is_idle;
            args_monitor_thread.d_is_idle[i] = &args_d[i].is_idle;
        }
        args_monitor_thread.pi_is_idle = &args_pi.is_idle;
        pid_monitor_thread = pthread_launch(pi_1_monitor_thread, &args_monitor_thread);
    }

    pthread_wait(pid_pi);
    float_num_t flt_pi = args_pi.res;

    for(uint64_t i=0; i<layers; i++)
    {
        args_a[i].keep_going = false;
        args_b[i].keep_going = false;
        args_c[i].keep_going = false;
        args_d[i].keep_going = false;
    }

    if(monitoring)
    {
        args_monitor_thread.keep_going = false;
    }

    for(uint64_t i=0; i<layers; i++)
    {
        float_num_t flt = float_num_wrap(0, 2);
        if(!queue_unstuck(&queue_a_b[i], &flt)) flt = float_num_wrap(0, 2);
        if(!queue_unstuck(&queue_b_d[i], &flt)) flt = float_num_wrap(0, 2);
        if(!queue_unstuck(&queue_c_d[i], &flt)) flt = float_num_wrap(0, 2);
        if(queue_unstuck(&junc_d_pi.queues[i], &flt)) float_num_free(flt);
    }

    for(uint64_t i=0; i<layers; i++)
    {
        pthread_wait(pid_a[i]);
        pthread_wait(pid_b[i]);
        pthread_wait(pid_c[i]);
        pthread_wait(pid_d[i]);
    }

    if(monitoring)
    {
        pthread_wait(pid_monitor_thread);
    }

    for(uint64_t i=0; i<layers; i++)
    {
        queue_free(&queue_a_b[i]);
        queue_free(&queue_b_d[i]);
        queue_free(&queue_c_d[i]);
    }
    junc_free(&junc_d_pi);

    float_num_free(flt_1);
    float_num_free(flt_m_1_2);

    if(monitoring)
    {
        pi_1_monitor_thread_treat_res(args_monitor_thread.res, layers);
    }
    else
    {
        printf("\n\n");
        float_num_display_dec(flt_pi);
    }
    float_num_free(flt_pi);
}
