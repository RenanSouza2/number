#include "time.c"
#include "pthread_struct.c"

#include "../../lib/num/header.h"
#include "../../lib/sig/header.h"



STRUCT(pi_2_thread_a_args)
{
    uint64_t size;
    line_p line_a_b;
    uint64_t id;
    bool keep_going;
    bool is_halted;
};

handler_p pi_2_thread_a(handler_p args)
{
    pi_2_thread_a_args_p _args = args;
    for(uint64_t i = _args->id + 2; _args->keep_going; i+=2)
    {
        sig_num_t sig_1 = sig_num_mul(
            sig_num_wrap((int64_t)2*i - 3),
            sig_num_wrap((int64_t)2*i - 5)
        );
        sig_num_t sig_2 = sig_num_mul(
            sig_num_wrap(64*i),
            sig_num_wrap(i - 1)
        );
        float_num_t flt_a = float_num_div(
            float_num_wrap_sig_num(sig_1, _args->size),
            float_num_wrap_sig_num(sig_2, _args->size)
        );

        line_post_response(_args->line_a_b, flt_a, &_args->is_halted);
    }

    return NULL;
}

STRUCT(pi_2_thread_b_args)
{
    uint64_t size;
    line_p line_a_b;
    line_p line_b_d;
    uint64_t id;
    float_num_t b0;
    bool keep_going;
    bool is_halted;
};

handler_p pi_2_thread_b(handler_p args)
{
    pi_2_thread_b_args_p _args = args;
    float_num_t flt_b = _args->b0;
    for(uint64_t i = _args->id + 2; _args->keep_going; i+=2)
    {
        float_num_t flt_a = line_get_response(_args->line_a_b, &_args->is_halted);

        flt_b = float_num_mul(flt_b, flt_a);

        line_post_response(_args->line_b_d, float_num_copy(flt_b), &_args->is_halted);
    }
    float_num_free(flt_b);

    return NULL;
}

STRUCT(pi_2_thread_c_args)
{
    uint64_t size;
    line_p line_c_d;
    float_num_t flt_1;
    float_num_t flt_m_1_2;
    uint64_t id;
    bool keep_going;
    bool is_halted;
};

handler_p pi_2_thread_c(handler_p args)
{
    pi_2_thread_c_args_p _args = args;
    for(uint64_t i = _args->id + 2; _args->keep_going; i+=2)
    {
        float_num_t flt = float_num_div(
            float_num_copy(_args->flt_1),
            float_num_wrap(2 * i + 1, _args->size)
        );
        flt = float_num_add(flt, float_num_copy(_args->flt_m_1_2));
        
        line_post_response(_args->line_c_d, flt, &_args->is_halted);
    }

    return NULL;
}

STRUCT(pi_2_thread_d_args)
{
    uint64_t size;
    line_p line_b_d;
    line_p line_c_d;
    line_p line_d_pi;
    uint64_t id;
    bool keep_going;
    bool is_halted;
};

handler_p pi_2_thread_d(handler_p args)
{
    pi_2_thread_d_args_p _args = args;
    for(uint64_t i = _args->id + 2; _args->keep_going; i+=2)
    {
        float_num_t flt_b = line_get_response(_args->line_b_d, &_args->is_halted);
        float_num_t flt_c = line_get_response(_args->line_c_d, &_args->is_halted);

        float_num_t flt_d = float_num_mul(flt_b, flt_c);
        
        line_post_response(_args->line_d_pi, flt_d, &_args->is_halted);
    }

    return NULL;
}

STRUCT(pi_2_thread_pi_args)
{
    uint64_t size;
    junc_p junc_d_pi;
    float_num_t res;
    bool is_halted;
};

handler_p pi_2_thread_pi(handler_p args)
{
    pi_2_thread_pi_args_p _args = args;
    float_num_t flt_pi = float_num_div(
        float_num_wrap(25, _args->size),
        float_num_wrap( 8, _args->size)
    );
    for(uint64_t i=2; ; i++)
    {
        float_num_t flt_d = junc_get_response(_args->junc_d_pi, &_args->is_halted);

        if(!float_num_safe_add(flt_pi, flt_d))
        {
            float_num_free(flt_d);
            break;
        }

        flt_pi = float_num_add(flt_pi, flt_d);
        
        // if(i%1000 == 0)
        // {
        //     printf("\n");
        //     float_num_display_dec(flt_pi);
        // }
    }

    _args->res = flt_pi;
    return NULL;
}



STRUCT(pi_2_monitor_thread_res)
{
    uint64_t count_a[2];
    uint64_t count_b[2];
    uint64_t count_c[2];
    uint64_t count_d[2];
    uint64_t count_pi;
    uint64_t total;
};

STRUCT(pi_2_monitor_thread_args)
{
    bool *a_is_halted[2];
    bool *b_is_halted[2];
    bool *c_is_halted[2];
    bool *d_is_halted[2];
    bool *pi_is_halted;
    pi_2_monitor_thread_res_t res;
    bool keep_going;
};

handler_p pi_2_monitor_thread(handler_p args)
{
    pi_2_monitor_thread_args_p _args = args;
    pi_2_monitor_thread_res_t res = (pi_2_monitor_thread_res_t) {};
    uint64_t i=0;
    for(; _args->keep_going; i++)
    {
        for(uint64_t j=0; j<2; j++)
        {
            res.count_a[j] += *(_args->a_is_halted[j]);
            res.count_b[j] += *(_args->b_is_halted[j]);
            res.count_c[j] += *(_args->c_is_halted[j]);
            res.count_d[j] += *(_args->d_is_halted[j]);
        }
        res.count_pi += *(_args->pi_is_halted);
        
        sleep(1);
    }

    res.total = i;
    _args->res = res;
    return NULL;
}

void pi_2_monitor_thread_treat_res(pi_2_monitor_thread_res_t res)
{
    printf("\n");
    printf("\n\t\t| 0\t| 1\t|");
    printf("\n --------------------------------");
    printf("\n| thread_a\t| %3.f %%", 100 - 100.0 * res.count_a[0] / res.total);
    printf("\t| %3.f %%\t|", 100 - 100.0 * res.count_a[1] / res.total);
    printf("\n| thread_b\t| %3.f %%", 100 - 100.0 * res.count_b[0] / res.total);
    printf("\t| %3.f %%\t|", 100 - 100.0 * res.count_b[1] / res.total);
    printf("\n| thread_c\t| %3.f %%", 100 - 100.0 * res.count_c[0] / res.total);
    printf("\t| %3.f %%\t|", 100 - 100.0 * res.count_c[1] / res.total);
    printf("\n| thread_d\t| %3.f %%", 100 - 100.0 * res.count_d[0] / res.total);
    printf("\t| %3.f %%\t|", 100 - 100.0 * res.count_d[1] / res.total);
    printf("\n");
    printf("\n| thread_pi\t| %3.f %%\t|", 100 - 100.0 * res.count_pi / res.total);
}



void pi_threads_2(uint64_t size)
{
    bool monitoring = false;

    pi_2_thread_a_args_t args_a[2];
    pi_2_thread_b_args_t args_b[2];
    pi_2_thread_c_args_t args_c[2];
    pi_2_thread_d_args_t args_d[2];

    line_t line_a_b[2];
    line_t line_b_d[2];
    line_t line_c_d[2];
    
    junc_t junc_d_pi = junc_init(2, 50);

    pthread_t pid_a[2];
    pthread_t pid_b[2];
    pthread_t pid_c[2];
    pthread_t pid_d[2];

    float_num_t b0[2] = {
        float_num_wrap(6, size),
        float_num_div(
            float_num_wrap(-3, size),
            float_num_wrap( 4, size)
        )
    };
    float_num_t flt_1 = float_num_wrap(1, size);
    float_num_t flt_m_1_2 = float_num_div(
        float_num_wrap(-1, size),
        float_num_wrap( 2, size)
    );

    for(uint64_t i=0; i<2; i++)
    {
        line_a_b[i] = line_init(50);
        args_a[i] = (pi_2_thread_a_args_t)
        {
            .size = size,
            .line_a_b = &line_a_b[i],
            .id = i,
            .keep_going = true,
            .is_halted = false
        };
        pid_a[i] = pthread_launch(pi_2_thread_a, &args_a[i]);
        
        line_b_d[i] = line_init(50);
        args_b[i] = (pi_2_thread_b_args_t)
        {
            .size = size,
            .line_a_b = &line_a_b[i],
            .line_b_d = &line_b_d[i],
            .id = i,
            .b0 = b0[i],
            .keep_going = true,
            .is_halted = false
        };
        pid_b[i] = pthread_launch(pi_2_thread_b, &args_b[i]);
        
        line_c_d[i] = line_init(50);
        args_c[i] = (pi_2_thread_c_args_t)
        {
            .size = size,
            .line_c_d = &line_c_d[i],
            .flt_1 = flt_1,
            .flt_m_1_2 = flt_m_1_2,
            .id = i,
            .keep_going = true,
            .is_halted = false
        };
        pid_c[i] = pthread_launch(pi_2_thread_c, &args_c[i]);

        args_d[i] = (pi_2_thread_d_args_t)
        {
            .size = size,
            .line_b_d = &line_b_d[i],
            .line_c_d = &line_c_d[i],
            .line_d_pi = &junc_d_pi.lines[i],
            .id = i,
            .keep_going = true,
            .is_halted = false
        };
        pid_d[i] = pthread_launch(pi_2_thread_d, &args_d[i]);
    }

    pi_2_thread_pi_args_t args_pi = (pi_2_thread_pi_args_t)
    {
        .size = size,
        .junc_d_pi = &junc_d_pi,
        .is_halted = false
    };
    pthread_t pid_pi = pthread_launch(pi_2_thread_pi, &args_pi);

    pi_2_monitor_thread_args_t args_monitor_thread;
    pthread_t pid_monitor_thread;
    if(monitoring)
    {
        args_monitor_thread = (pi_2_monitor_thread_args_t)
        {
            .a_is_halted = {&args_a[0].is_halted, &args_a[1].is_halted},
            .b_is_halted = {&args_b[0].is_halted, &args_b[1].is_halted},
            .c_is_halted = {&args_c[0].is_halted, &args_c[1].is_halted},
            .d_is_halted = {&args_d[0].is_halted, &args_d[1].is_halted},
            .pi_is_halted = &args_pi.is_halted,
            .keep_going = true
        };
        pid_monitor_thread = pthread_launch(pi_2_monitor_thread, &args_monitor_thread);
    }

    pthread_wait(pid_pi);
    float_num_t flt_pi = args_pi.res;

    for(uint64_t i=0; i<2; i++)
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

    for(uint64_t i=0; i<2; i++)
    {
        float_num_free(line_tryget_response(&line_a_b[i]));
        float_num_free(line_tryget_response(&line_b_d[i]));
        float_num_free(line_tryget_response(&line_c_d[i]));
        float_num_free(line_tryget_response(&junc_d_pi.lines[i]));
    }

    for(uint64_t i=0; i<2; i++)
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
    
    for(uint64_t i=0; i<2; i++)
    {
        line_free(&line_a_b[i]);
        line_free(&line_b_d[i]);
        line_free(&line_c_d[i]);
    }
    junc_free(&junc_d_pi);

    float_num_free(flt_1);
    float_num_free(flt_m_1_2);


    if(monitoring)
    {
        pi_2_monitor_thread_treat_res(args_monitor_thread.res);
    }
    else
    {
        printf("\n\n");
        float_num_display_dec(flt_pi);
    }
    float_num_free(flt_pi);
} 
