#include "time.c"
#include "pthread_struct.c"



STRUCT(pi_1_thread_a_args)
{
    uint64_t size;
    line_p line_a_b;
    float_num_t flt_m_3_8;
    float_num_t flt_1_4;
    bool keep_going;
};

handler_p pi_1_thread_a(handler_p args)
{
    pi_1_thread_a_args_p _args = args;
    for(uint64_t i=1; _args->keep_going; i++)
    {
        float_num_t flt_a = float_num_div(
            float_num_copy(_args->flt_m_3_8),
            float_num_wrap(i, _args->size)
        );
        flt_a = float_num_add(flt_a, float_num_copy(_args->flt_1_4));

        line_post_response(_args->line_a_b, flt_a, NULL);
    }

    return NULL;
}

STRUCT(pi_1_thread_b_args)
{
    uint64_t size;
    line_p line_a_b;
    line_p line_b_d;
    bool keep_going;
};

handler_p pi_1_thread_b(handler_p args)
{
    pi_1_thread_b_args_p _args = args;
    float_num_t flt_b = float_num_wrap(6, _args->size);
    for(uint64_t i=1; _args->keep_going; i++)
    {
        float_num_t flt_a = line_get_response(_args->line_a_b, NULL);

        flt_b = float_num_mul(flt_b, flt_a);

        line_post_response(_args->line_b_d, float_num_copy(flt_b), NULL);
    }
    float_num_free(flt_b);

    return NULL;
}

STRUCT(pi_1_thread_c_args)
{
    uint64_t size;
    line_p line_c_d;
    float_num_t flt_1;
    float_num_t flt_m_1_2;
    bool keep_going;
};

handler_p pi_1_thread_c(handler_p args)
{
    pi_1_thread_c_args_p _args = args;
    for(uint64_t i=1; _args->keep_going; i++)
    {
        float_num_t flt = float_num_div(
            float_num_copy(_args->flt_1),
            float_num_wrap(2 * i + 1, _args->size)
        );
        flt = float_num_add(flt, float_num_copy(_args->flt_m_1_2));

        line_post_response(_args->line_c_d, flt, NULL);
    }

    return NULL;
}

STRUCT(pi_1_thread_d_args)
{
    uint64_t size;
    line_p line_b_d;
    line_p line_c_d;
    line_p line_d_pi;
    bool keep_going;
};

handler_p pi_1_thread_d(handler_p args)
{
    pi_1_thread_d_args_p _args = args;
    for(uint64_t i=1; _args->keep_going; i++)
    {
        float_num_t flt_b = line_get_response(_args->line_b_d, NULL);
        float_num_t flt_c = line_get_response(_args->line_c_d, NULL);

        float_num_t flt_d = float_num_mul(flt_b, flt_c);

        line_post_response(_args->line_d_pi, flt_d, NULL);
    }

    return NULL;
}

STRUCT(pi_1_thread_pi_args)
{
    uint64_t size;
    line_p line_d_pi;
};

handler_p pi_1_thread_pi(handler_p args)
{
    pi_1_thread_pi_args_p _args = args;
    float_num_t flt_pi = float_num_wrap(3, _args->size);
    for(uint64_t i=1; ; i++)
    {
        float_num_t flt_d = line_get_response(_args->line_d_pi, NULL);
        if(!float_num_safe_add(flt_pi, flt_d))
        {
            float_num_free(flt_d);
            break;
        }

        flt_pi = float_num_add(flt_pi, flt_d);
        
        if(i%1000 == 0)
        {
            printf("\n");
            float_num_display_dec(flt_pi);
        }
    }

    float_num_p flt_res = malloc(sizeof(float_num_t));
    assert(flt_res);
    *flt_res = flt_pi;
    return flt_res;
}



void pi_threads_1(uint64_t size)
{
    line_t line_a_b = line_init(50);
    float_num_t flt_m_3_8 = float_num_div(
        float_num_wrap(-3, size),
        float_num_wrap(8, size)
    );
    float_num_t flt_1_4 = float_num_div(
        float_num_wrap(1, size),
        float_num_wrap(4, size)
    );
    pi_1_thread_a_args_t args_a = (pi_1_thread_a_args_t)
    {
        .size = size,
        .line_a_b = &line_a_b,
        .flt_m_3_8 = flt_m_3_8,
        .flt_1_4 = flt_1_4,
        .keep_going = true
    };
    pthread_t pid_a = pthread_launch(pi_1_thread_a, &args_a);

    line_t line_b_d = line_init(50);
    pi_1_thread_b_args_t args_b = (pi_1_thread_b_args_t)
    {
        .size = size,
        .line_a_b = &line_a_b,
        .line_b_d = &line_b_d,
        .keep_going = true
    };
    pthread_t pid_b = pthread_launch(pi_1_thread_b, &args_b);
    
    line_t line_c_d = line_init(50);
    float_num_t flt_1 = float_num_wrap(1, size);
    float_num_t flt_m_1_2 = float_num_div (
        float_num_wrap(-1, size),
        float_num_wrap(2, size)
    );
    pi_1_thread_c_args_t args_c = (pi_1_thread_c_args_t)
    {
        .size = size,
        .line_c_d = &line_c_d,
        .flt_1 = flt_1,
        .flt_m_1_2 = flt_m_1_2,
        .keep_going = true
    };
    pthread_t pid_c = pthread_launch(pi_1_thread_c, &args_c);

    line_t line_d_pi = line_init(50);
    pi_1_thread_d_args_t args_d = (pi_1_thread_d_args_t)
    {
        .size = size,
        .line_b_d = &line_b_d,
        .line_c_d = &line_c_d,
        .line_d_pi = &line_d_pi,
        .keep_going = true
    };
    pthread_t pid_d = pthread_launch(pi_1_thread_d, &args_d);

    pi_1_thread_pi_args_t args_pi = (pi_1_thread_pi_args_t)
    {
        .size = size,
        .line_d_pi = &line_d_pi
    };
    pthread_t pid_pi = pthread_launch(pi_1_thread_pi, &args_pi);

    float_num_p flt_res;
    TREAT(pthread_join(pid_pi, (handler_p*)&flt_res));
    float_num_t flt_pi = *flt_res;
    free(flt_res);

    args_a.keep_going = false;
    args_b.keep_going = false;
    args_c.keep_going = false;
    args_d.keep_going = false;

    float_num_free(line_tryget_response(&line_a_b));
    float_num_free(line_tryget_response(&line_b_d));
    float_num_free(line_tryget_response(&line_c_d));
    float_num_free(line_tryget_response(&line_d_pi));

    TREAT(pthread_join(pid_a, NULL));
    TREAT(pthread_join(pid_b, NULL));
    TREAT(pthread_join(pid_c, NULL));
    TREAT(pthread_join(pid_d, NULL));

    line_free(&line_a_b);
    line_free(&line_b_d);
    line_free(&line_c_d);
    line_free(&line_d_pi);

    float_num_free(flt_m_3_8);
    float_num_free(flt_1_4);
    float_num_free(flt_1);
    float_num_free(flt_m_1_2);

    printf("\n\n");
    float_num_display_dec(flt_pi);
    float_num_free(flt_pi);
} 
