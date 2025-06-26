#include "time.c"
#include "pthread_struct.c"

#include "../../lib/num/header.h"
#include "../../lib/sig/header.h"



STRUCT(pi_2_thread_a_args)
{
    uint64_t size;
    line_p line_a_b;
    uint64_t id;
    uint64_t layers;
    float_num_t a0;
    bool keep_going;
    bool is_idle;
};

handler_p pi_2_thread_a(handler_p _args)
{
    pi_2_thread_a_args_p args = _args;
    float_num_t flt_a = args->a0;
    for(uint64_t i = args->id + args->layers; args->keep_going; i += args->layers)
    {
        sig_num_t sig_a = sig_num_wrap((int64_t)2 * i - 3);
        sig_num_t sig_b = sig_num_wrap((int64_t)8 * i);
        for(uint64_t j=1; j<args->layers; j++)
        {
            sig_a = sig_num_mul(sig_a, sig_num_wrap((int64_t)2 * (i - j) - 3));
            sig_b = sig_num_mul(sig_b, sig_num_wrap((int64_t)8 * (i - j)));
        }

        flt_a = float_num_mul_sig(flt_a, sig_a);
        flt_a = float_num_div_sig(flt_a, sig_b);

        line_post_response(args->line_a_b, float_num_copy(flt_a), &args->is_idle);
    }
    float_num_free(flt_a);

    return NULL;
}

STRUCT(pi_2_thread_b_args)
{
    uint64_t size;
    line_p line_a_b;
    line_p line_b_pi;
    uint64_t id;
    uint64_t layers;
    bool keep_going;
    bool is_idle;
};

handler_p pi_2_thread_b(handler_p _args)
{
    pi_2_thread_b_args_p args = _args;
    for(uint64_t i = args->id + args->layers; args->keep_going; i += args->layers)
    {
        float_num_t flt_a = line_get_response(args->line_a_b, &args->is_idle);
        if(!args->keep_going)
        {
            float_num_free(flt_a);
            break;
        }
        
        float_num_t flt_b;
        flt_b = float_num_mul_sig(flt_a, sig_num_wrap((int64_t)1 - 2 * i));
        flt_b = float_num_div_sig(flt_b, sig_num_wrap((int64_t)4 * i + 2));

        line_post_response(args->line_b_pi, flt_b, &args->is_idle);
    }

    return NULL;
}

STRUCT(pi_2_thread_pi_args)
{
    uint64_t size;
    junc_p junc_b_pi;
    uint64_t layers;
    float_num_t pi0;
    float_num_t res;
    bool is_idle;
};

handler_p pi_2_thread_pi(handler_p _args)
{
    pi_2_thread_pi_args_p args = _args;
    float_num_t flt_pi = args->pi0;
    for(uint64_t i=args->layers; ; i++)
    {
        float_num_t flt_b = junc_get_response(args->junc_b_pi, &args->is_idle);

        if(i%100000 == 0)
        {
            uint64_t done = -(flt_b.size + flt_b.exponent);
            fprintf(stderr, "\npgr: %lu / %lu", done / 1000, flt_b.size / 1000);
        }

        if(!float_num_safe_add(flt_pi, flt_b))
        {
            float_num_free(flt_b);
            break;
        }

        flt_pi = float_num_add(flt_pi, flt_b);
    }

    args->res = flt_pi;
    return NULL;
}



STRUCT(pi_2_monitor_idle_res)
{
    uint64_t *count_a;
    uint64_t *count_b;
    uint64_t *count_c;
    uint64_t *count_d;
    uint64_t count_pi;
    uint64_t total;
};

pi_2_monitor_idle_res_t pi_2_monitor_idle_res_create(uint64_t layers)
{
    uint64_t *count_a = calloc(layers, sizeof(uint64_t));
    uint64_t *count_b = calloc(layers, sizeof(uint64_t));
    assert(count_a);
    assert(count_b);

    return (pi_2_monitor_idle_res_t)
    {
        .count_a = count_a,
        .count_b = count_b,
    };
}

void pi_2_monitor_idle_res_free(pi_2_monitor_idle_res_t res)
{
    free(res.count_a);
    free(res.count_b);
}

STRUCT(pi_2_monitor_idle_args)
{
    uint64_t layers;
    bool **a_is_idle;
    bool **b_is_idle;
    bool *pi_is_idle;
    pi_2_monitor_idle_res_t res;
    bool keep_going;
};

pi_2_monitor_idle_args_t pi_2_monitor_idle_args_create(uint64_t layers)
{
    bool **a_is_idle = malloc(layers * sizeof(bool*));
    bool **b_is_idle = malloc(layers * sizeof(bool*));
    assert(a_is_idle);
    assert(b_is_idle);

    return (pi_2_monitor_idle_args_t)
    {
        .layers = layers,
        .a_is_idle = a_is_idle,
        .b_is_idle = b_is_idle,
        .res = pi_2_monitor_idle_res_create(layers),
        .keep_going = true
    };
}

 void pi_2_monitor_idle_args_free(pi_2_monitor_idle_args_t args)
 {
    free(args.a_is_idle);
    free(args.b_is_idle);

    pi_2_monitor_idle_res_free(args.res);
 }

handler_p pi_2_monitor_idle(handler_p _args)
{
    pi_2_monitor_idle_args_p args = _args;
    pi_2_monitor_idle_res_t res = args->res;
    uint64_t total=0;
    for(; args->keep_going; total++)
    {
        for(uint64_t i=0; i<args->layers; i++)
        {
            res.count_a[i] += *(args->a_is_idle[i]);
            res.count_b[i] += *(args->b_is_idle[i]);
        }
        res.count_pi += *(args->pi_is_idle);

        usleep(10000);
    }

    res.total = total;
    args->res = res;
    return NULL;
}

void pi_2_monitor_idle_treat_res(pi_2_monitor_idle_args_t args, uint64_t layers)
{
    pi_2_monitor_idle_res_t res = args.res;
    printf("\n");
    printf("\n\t\t|");
    for(uint64_t i=0; i<layers; i++)
        printf(" %lu\t|", i);
    printf("\n ----------------");
    for(uint64_t i=0; i<layers; i++)
        printf("--------");
    printf("\n| thread_a\t|");
    for(uint64_t i=0; i<layers; i++)
        printf(" %3.f %%\t|", 100 - 100.0 * res.count_a[i] / res.total);
    printf("\n| thread_b\t|");
    for(uint64_t i=0; i<layers; i++)
        printf(" %3.f %%\t|", 100 - 100.0 * res.count_b[i] / res.total);
    printf("\n");
    printf("\n| thread_pi\t| %3.f %%\t|", 100 - 100.0 * res.count_pi / res.total);

    pi_2_monitor_idle_args_free(args);
}



float_num_t pi_threads_2_calc(uint64_t size, uint64_t layers, bool monitoring)
{
    uint64_t line_size = 1000;

    pi_2_thread_a_args_t args_a[layers];
    pi_2_thread_b_args_t args_b[layers];

    line_t line_a_b[layers];

    junc_t junc_b_pi = junc_init(layers, line_size);

    pthread_t pid_a[layers];
    pthread_t pid_b[layers];

    float_num_t a0[layers];
    a0[0] = float_num_wrap(6, size);

    float_num_t flt_a = float_num_wrap(6, size);
    float_num_t pi0 = float_num_wrap(3, size);
    for(uint64_t i=1; i<layers; i++)
    {
        flt_a = float_num_mul(flt_a, float_num_wrap((int64_t)2 * i - 3, size));
        flt_a = float_num_div(flt_a, float_num_wrap((int64_t)8 * i, size));
        a0[i] = float_num_copy(flt_a);

        float_num_t flt_d = float_num_copy(flt_a);
        flt_d = float_num_mul(flt_d, float_num_wrap((int64_t)1 - 2 * i, size));
        flt_d = float_num_div(flt_d, float_num_wrap((int64_t)4 * i + 2, size));

        pi0 = float_num_add(pi0, flt_d);
    }
    float_num_free(flt_a);

    for(uint64_t i=0; i<layers; i++)
    {
        line_a_b[i] = line_init(line_size);
        args_a[i] = (pi_2_thread_a_args_t)
        {
            .size = size,
            .line_a_b = &line_a_b[i],
            .id = i,
            .layers = layers,
            .a0 = a0[i],
            .keep_going = true,
            .is_idle = false
        };
        pid_a[i] = pthread_launch(pi_2_thread_a, &args_a[i]);
        
        args_b[i] = (pi_2_thread_b_args_t)
        {
            .size = size,
            .line_a_b = &line_a_b[i],
            .line_b_pi = &junc_b_pi.lines[i],
            .id = i,
            .layers = layers,
            .keep_going = true,
            .is_idle = false
        };
        pid_b[i] = pthread_launch(pi_2_thread_b, &args_b[i]);
    }

    pi_2_thread_pi_args_t args_pi = (pi_2_thread_pi_args_t)
    {
        .size = size,
        .junc_b_pi = &junc_b_pi,
        .layers = layers,
        .pi0 = pi0,
        .is_idle = false
    };
    pthread_t pid_pi = pthread_launch(pi_2_thread_pi, &args_pi);

    pi_2_monitor_idle_args_t args_monitor_idle;
    pthread_t pid_monitor_idle = 0;
    if(monitoring)
    {
        args_monitor_idle = pi_2_monitor_idle_args_create(layers);
        for(uint64_t i=0; i<layers; i++)
        {
            args_monitor_idle.a_is_idle[i] = &args_a[i].is_idle;
            args_monitor_idle.b_is_idle[i] = &args_b[i].is_idle;
        }
        args_monitor_idle.pi_is_idle = &args_pi.is_idle;
        pid_monitor_idle = pthread_launch(pi_2_monitor_idle, &args_monitor_idle);
    }

    pthread_wait(pid_pi);
    float_num_t flt_pi = args_pi.res;

    for(uint64_t i=0; i<layers; i++)
    {
        args_a[i].keep_going = false;
        args_b[i].keep_going = false;
    }

    if(monitoring)
    {
        args_monitor_idle.keep_going = false;
    }

    for(uint64_t i=0; i<layers; i++)
    {
        line_trypost_response(&line_a_b[i], float_num_wrap(0, 2));
        line_trypost_response(&junc_b_pi.lines[i], float_num_wrap(0, 2));

        float_num_free(line_tryget_response(&line_a_b[i]));
        float_num_free(line_tryget_response(&junc_b_pi.lines[i]));
    }

    for(uint64_t i=0; i<layers; i++)
    {
        pthread_wait(pid_a[i]);
        pthread_wait(pid_b[i]);
    }
    
    if(monitoring)
    {
        pthread_wait(pid_monitor_idle);
    }

    for(uint64_t i=0; i<layers; i++)
    {
        line_free(&line_a_b[i]);
    }
    junc_free(&junc_b_pi);
    
    if(monitoring)
    {
        pi_2_monitor_idle_treat_res(args_monitor_idle, layers);
    }

    return flt_pi;
}

void pi_threads_2(uint64_t size, uint64_t layers, bool monitoring)
{
    float_num_t flt_pi = pi_threads_2_calc(size, layers, monitoring);
    if(!monitoring)
    {
        printf("\n\n");
        float_num_display_dec(flt_pi);
    }
    float_num_free(flt_pi);
}

void pi_2_time_1(uint64_t size)
{
    for(uint64_t layers = 1; layers<20; layers++)
    {
        printf("\ni: %lu\t", layers);
        uint64_t begin = get_time();
        pi_threads_2_calc(size, layers, false);
        uint64_t end = get_time();
        uint64_t time = end - begin;
        printf("\t%.1f\t%.1f", time / 1e9, time * layers / 1e9);
    }
}

void pi_2_time_2()
{
    for(uint64_t size = 100; size < 2000; size += 100)
    {
        printf("\ni: %5lu\t", size);
        uint64_t begin = get_time();
        pi_threads_2_calc(size, 8, false);
        uint64_t end = get_time();
        uint64_t time = end - begin;
        printf("\t%10.3f", time / 1e9);
    }
}
