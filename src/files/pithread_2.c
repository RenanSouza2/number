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



// STRUCT(pi_2_monitor_thread_res)
// {
//     uint64_t count_a[4];
//     uint64_t count_b[4];
//     uint64_t count_c[4];
//     uint64_t count_d[4];
//     uint64_t count_pi;
//     uint64_t total;
// };

// STRUCT(pi_2_monitor_thread_args)
// {
//     bool *a_is_halted[4];
//     bool *b_is_halted[4];
//     bool *c_is_halted[4];
//     bool *d_is_halted[4];
//     bool *pi_is_halted;
//     pi_2_monitor_thread_res_t res;
//     bool keep_going;
// };

// handler_p pi_2_monitor_thread(handler_p args)
// {
//     pi_2_monitor_thread_args_p _args = args;
//     pi_2_monitor_thread_res_t res = (pi_2_monitor_thread_res_t) {};
//     uint64_t i=0;
//     for(; _args->keep_going; i++)
//     {
//         for(uint64_t j=0; j<4; j++)
//         {
//             res.count_a[j] += *(_args->a_is_halted[j]);
//             res.count_b[j] += *(_args->b_is_halted[j]);
//             res.count_c[j] += *(_args->c_is_halted[j]);
//             res.count_d[j] += *(_args->d_is_halted[j]);
//         }
//         res.count_pi += *(_args->pi_is_halted);
        
//         sleep(1);
//     }

//     res.total = i;
//     _args->res = res;
//     return NULL;
// }

// void pi_2_monitor_thread_treat_res(pi_2_monitor_thread_res_t res)
// {
//     printf("\n");
//     printf("\n\t\t| 0\t| 1\t| 2\t| 3\t|");
//     printf("\n ------------------------------------------------");
//     printf("\n| thread_a\t|");
//     for(uint64_t i=0; i<4; i++)
//         printf(" %3.f %%\t|", 100 - 100.0 * res.count_a[i] / res.total);
//     printf("\n| thread_b\t|");
//     for(uint64_t i=0; i<4; i++)
//         printf(" %3.f %%\t|", 100 - 100.0 * res.count_b[i] / res.total);
//     printf("\n| thread_c\t|");
//     for(uint64_t i=0; i<4; i++)
//         printf(" %3.f %%\t|", 100 - 100.0 * res.count_c[i] / res.total);
//     printf("\n| thread_d\t|");
//     for(uint64_t i=0; i<4; i++)
//         printf(" %3.f %%\t|", 100 - 100.0 * res.count_d[i] / res.total);
//     printf("\n");
//     printf("\n| thread_pi\t| %3.f %%\t|", 100 - 100.0 * res.count_pi / res.total);
// }



void pi_threads_2(uint64_t size, uint64_t layers, bool /*monitoring*/)
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

//     pi_2_monitor_thread_args_t args_monitor_thread;
//     pthread_t pid_monitor_thread;
//     if(monitoring)
//     {
//         args_monitor_thread = (pi_2_monitor_thread_args_t)
//         {
//             .pi_is_halted = &args_pi.is_halted,
//             .keep_going = true
//         };
//         for(uint64_t i=0; i<4; i++)
//         {
//             args_monitor_thread.a_is_halted[i] = &args_a[i].is_halted;
//             args_monitor_thread.b_is_halted[i] = &args_b[i].is_halted;
//             args_monitor_thread.c_is_halted[i] = &args_c[i].is_halted;
//             args_monitor_thread.d_is_halted[i] = &args_d[i].is_halted;
//         }
//         pid_monitor_thread = pthread_launch(pi_2_monitor_thread, &args_monitor_thread);
//     }

    pthread_wait(pid_pi);
    float_num_t flt_pi = args_pi.res;

    for(uint64_t i=0; i<layers; i++)
    {
        args_a[i].keep_going = false;
        args_b[i].keep_going = false;
    }

//     if(monitoring)
//     {
//         args_monitor_thread.keep_going = false;
//     }

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

//     if(monitoring)
//     {
//         pthread_wait(pid_monitor_thread);
//     }
    
    for(uint64_t i=0; i<layers; i++)
        line_free(&line_a_b[i]);

    junc_free(&junc_b_pi);

//     if(monitoring)
//     {
//         pi_2_monitor_thread_treat_res(args_monitor_thread.res);
//     }
//     else
//     {
        printf("\n\n");
        float_num_display_dec(flt_pi);
//     }
    float_num_free(flt_pi);
} 
