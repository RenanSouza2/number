#include "time.c"
#include "pthread_struct.c"

#include "../../lib/num/header.h"
#include "../../lib/sig/header.h"



STRUCT(pi_2_thread_a_args)
{
    uint64_t size;
    line_p line_a_b;
    uint64_t id;
    float_num_t a0;
    bool keep_going;
    bool is_halted;
};

handler_p pi_2_thread_a(handler_p _args)
{
    pi_2_thread_a_args_p args = _args;
    float_num_t flt_a = args->a0;
    for(uint64_t i = args->id + 1; args->keep_going; i += 1)
    {
        flt_a = float_num_mul(flt_a, float_num_wrap((int64_t)2 * i - 3, args->size));
        flt_a = float_num_div(flt_a, float_num_wrap((int64_t)8 * i, args->size));

        line_post_response(args->line_a_b, float_num_copy(flt_a), &args->is_halted);
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
    bool keep_going;
    bool is_halted;
};

handler_p pi_2_thread_b(handler_p _args)
{
    pi_2_thread_b_args_p args = _args;
    for(uint64_t i = args->id + 1; args->keep_going; i += 1)
    {
        float_num_t flt_a = line_get_response(args->line_a_b, &args->is_halted);
    
        float_num_t flt_b;
        flt_b = float_num_mul(flt_a, float_num_wrap((int64_t)1 - 2 * i, args->size));
        flt_b = float_num_div(flt_b, float_num_wrap((int64_t)4 * i + 2, args->size));

        line_post_response(args->line_b_pi, flt_b, &args->is_halted);
    }

    return NULL;
}

STRUCT(pi_2_thread_pi_args)
{
    uint64_t size;
    junc_p junc_b_pi;
    float_num_t pi0;
    float_num_t res;
    bool is_halted;
};

handler_p pi_2_thread_pi(handler_p _args)
{
    pi_2_thread_pi_args_p args = _args;
    float_num_t flt_pi = args->pi0;
    for(uint64_t i=1; ; i++)
    {
        float_num_t flt_b = junc_get_response(args->junc_b_pi, &args->is_halted);

        if(i%1000 == 0)
            fprintf(stderr, "\nexp: %ld", -(flt_b.size + flt_b.exponent));

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



STRUCT(pi_2_monitor_thread_res)
{
    uint64_t *count_a;
    uint64_t *count_b;
    uint64_t count_pi;
    uint64_t total;
};

pi_2_monitor_thread_res_t pi_2_monitor_thread_res_create(uint64_t layers)
{
    uint64_t *count_a = calloc(layers, sizeof(uint64_t));
    uint64_t *count_b = calloc(layers, sizeof(uint64_t));
    assert(count_a);
    assert(count_b);

    return (pi_2_monitor_thread_res_t)
    {
        .count_a = count_a,
        .count_b = count_b,
    };
}

void pi_2_monitor_thread_res_free(pi_2_monitor_thread_res_t res)
{
    free(res.count_a);
    free(res.count_b);
}

STRUCT(pi_2_monitor_thread_args)
{
    uint64_t layers;
    bool **a_is_halted;
    bool **b_is_halted;
    bool *pi_is_halted;
    pi_2_monitor_thread_res_t res;
    bool keep_going;
};

pi_2_monitor_thread_args_t pi_2_monitor_thread_args_create(uint64_t layers)
{
    bool **a_is_halted = malloc(layers * sizeof(bool*));
    bool **b_is_halted = malloc(layers * sizeof(bool*));
    assert(a_is_halted);
    assert(b_is_halted);

    return (pi_2_monitor_thread_args_t)
    {
        .layers = layers,
        .a_is_halted = a_is_halted,
        .b_is_halted = b_is_halted,
        .res = pi_2_monitor_thread_res_create(layers),
        .keep_going = true
    };
}

 void pi_2_monitor_thread_args_free(pi_2_monitor_thread_args_t args)
 {
    free(args.a_is_halted);
    free(args.b_is_halted);
 }

handler_p pi_2_monitor_thread(handler_p _args)
{
    pi_2_monitor_thread_args_p args = _args;
    pi_2_monitor_thread_res_t res = args->res;
    uint64_t total=0;
    for(; args->keep_going; total++)
    {
        for(uint64_t i=0; i<args->layers; i++)
        {
            res.count_a[i] += *(args->a_is_halted[i]);
            res.count_b[i] += *(args->b_is_halted[i]);
        }
        res.count_pi += *(args->pi_is_halted);
        
        sleep(1);
    }

    res.total = total;
    args->res = res;
    return NULL;
}

void pi_2_monitor_thread_treat_res(pi_2_monitor_thread_res_t res, uint64_t layers)
{
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
}



void pi_threads_2(uint64_t size, uint64_t layers, bool monitoring)
{
    pi_2_thread_a_args_t args_a[layers];
    pi_2_thread_b_args_t args_b[layers];

    line_t line_a_b[layers];

    junc_t junc_b_pi = junc_init(layers, 50);

    pthread_t pid_a[layers];
    pthread_t pid_b[layers];

    float_num_t a0[layers];
    a0[0] = float_num_wrap(6, size);
    
    float_num_t pi0 = float_num_wrap(3, size);

    for(uint64_t i=0; i<layers; i++)
    {
        line_a_b[i] = line_init(50);
        args_a[i] = (pi_2_thread_a_args_t)
        {
            .size = size,
            .line_a_b = &line_a_b[i],
            .id = i,
            .a0 = a0[i],
            .keep_going = true,
            .is_halted = false
        };
        pid_a[i] = pthread_launch(pi_2_thread_a, &args_a[i]);

        args_b[i] = (pi_2_thread_b_args_t)
        {
            .size = size,
            .line_a_b = &line_a_b[i],
            .line_b_pi = &junc_b_pi.lines[i],
            .id = i,
            .keep_going = true,
            .is_halted = false
        };
        pid_b[i] = pthread_launch(pi_2_thread_b, &args_b[i]);
    }

    pi_2_thread_pi_args_t args_pi = (pi_2_thread_pi_args_t)
    {
        .size = size,
        .junc_b_pi = &junc_b_pi,
        .pi0 = pi0,
        .is_halted = false
    };
    pthread_t pid_pi = pthread_launch(pi_2_thread_pi, &args_pi);

    pi_2_monitor_thread_args_t args_monitor_thread;
    pthread_t pid_monitor_thread = 0;
    if(monitoring)
    {
        args_monitor_thread = pi_2_monitor_thread_args_create(layers);
        for(uint64_t i=0; i<layers; i++)
        {
            args_monitor_thread.a_is_halted[i] = &args_a[i].is_halted;
            args_monitor_thread.b_is_halted[i] = &args_b[i].is_halted;
        }
        args_monitor_thread.pi_is_halted = &args_pi.is_halted;
        pid_monitor_thread = pthread_launch(pi_2_monitor_thread, &args_monitor_thread);
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
        args_monitor_thread.keep_going = false;
    }

    for(uint64_t i=0; i<layers; i++)
    {
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
        pthread_wait(pid_monitor_thread);
    }
    
    for(uint64_t i=0; i<layers; i++)
        line_free(&line_a_b[i]);

    junc_free(&junc_b_pi);

    if(monitoring)
    {
        pi_2_monitor_thread_treat_res(args_monitor_thread.res, layers);
    }
    else
    {
        printf("\n\n");
        float_num_display_dec(flt_pi);
    }
    float_num_free(flt_pi);
} 
