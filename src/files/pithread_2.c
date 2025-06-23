#include "time.c"
#include "pthread_struct.c"

#include "../../lib/num/header.h"



STRUCT(pi_2_thread_a_args)
{
    uint64_t size;
    line_p line_a_b;
    uint64_t i0;
    bool keep_going;
};

handler_p pi_2_thread_a(handler_p args)
{
    pi_2_thread_a_args_p _args = args;
    for(uint64_t i=_args->i0; _args->keep_going; i+=2)
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

        line_post_response(_args->line_a_b, flt_a);
    }

    return NULL;
}

STRUCT(pi_2_thread_b_args)
{
    uint64_t size;
    line_p line_a_b;
    line_p line_b_d;
    uint64_t i0;
    float_num_t b0;
    bool keep_going;
};

handler_p pi_2_thread_b(handler_p args)
{
    pi_2_thread_b_args_p _args = args;
    float_num_t flt_b = _args->b0;
    for(uint64_t i=_args->i0; _args->keep_going; i+=2)
    {
        float_num_t flt_a = line_get_response(_args->line_a_b);

        flt_b = float_num_mul(flt_b, flt_a);
        // if(i%2==0)
        // {
        //     printf("\n2 | i: %lu\t| ", i);
        //     float_num_display_dec(flt_b);
        // }

        line_post_response(_args->line_b_d, float_num_copy(flt_b));
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
    uint64_t i0;
    bool keep_going;
};

handler_p pi_2_thread_c(handler_p args)
{
    pi_2_thread_c_args_p _args = args;
    for(uint64_t i=_args->i0; _args->keep_going; i+=2)
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

STRUCT(pi_2_thread_d_args)
{
    uint64_t size;
    line_p line_b_d;
    line_p line_c_d;
    line_p line_d_pi;
    uint64_t i0;
    bool keep_going;
};

handler_p pi_2_thread_d(handler_p args)
{
    pi_2_thread_d_args_p _args = args;
    for(uint64_t i=_args->i0; _args->keep_going; i+=2)
    {
        float_num_t flt_b = line_get_response(_args->line_b_d);
        float_num_t flt_c = line_get_response(_args->line_c_d);

        float_num_t flt_d = float_num_mul(flt_b, flt_c);

        line_post_response(_args->line_d_pi, flt_d);
    }

    return NULL;
}

STRUCT(pi_2_thread_pi_args)
{
    uint64_t size;
    zip_p zip_d_pi;
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
        float_num_t flt_d = zip_get_response(_args->zip_d_pi);
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



void pi_threads_2(uint64_t size)
{
    pi_2_thread_a_args_t args_a[2];
    pi_2_thread_b_args_t args_b[2];
    pi_2_thread_c_args_t args_c[2];
    pi_2_thread_d_args_t args_d[2];

    line_t line_a_b[2];
    line_t line_b_d[2];
    line_t line_c_d[2];
    
    zip_t zip_d_pi = zip_init(2, 50);

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
            .i0 = i + 2,
            .keep_going = true
        };
        pid_a[i] = pthread_launch(pi_2_thread_a, &args_a[i]);
        
        line_b_d[i] = line_init(50);
        args_b[i] = (pi_2_thread_b_args_t)
        {
            .size = size,
            .line_a_b = &line_a_b[i],
            .line_b_d = &line_b_d[i],
            .i0 = i + 2,
            .b0 = b0[i],
            .keep_going = true
        };
        pid_b[i] = pthread_launch(pi_2_thread_b, &args_b[i]);
        
        line_c_d[i] = line_init(50);
        args_c[i] = (pi_2_thread_c_args_t)
        {
            .size = size,
            .line_c_d = &line_c_d[i],
            .flt_1 = flt_1,
            flt_m_1_2 = flt_m_1_2,
            .i0 = i + 2,
            .keep_going = true
        };
        pid_c[i] = pthread_launch(pi_2_thread_c, &args_c[i]);

        args_d[i] = (pi_2_thread_d_args_t)
        {
            .size = size,
            .line_b_d = &line_b_d[i],
            .line_c_d = &line_c_d[i],
            .line_d_pi = &zip_d_pi.lines[i],
            .i0 = i + 2,
            .keep_going = true
        };
        pid_d[i] = pthread_launch(pi_2_thread_d, &args_d[i]);
    }

    pi_2_thread_pi_args_t args_pi = (pi_2_thread_pi_args_t)
    {
        .size = size,
        .zip_d_pi = &zip_d_pi
    };
    pthread_t pid_pi = pthread_launch(pi_2_thread_pi, &args_pi);

    float_num_p flt_res;
    pthread_join(pid_pi, (handler_p*)&flt_res);
    float_num_t flt_pi = *flt_res;
    free(flt_res);

    for(uint64_t i=0; i<2; i++)
    {
        args_a[i].keep_going = false;
        args_b[i].keep_going = false;
        args_c[i].keep_going = false;
        args_d[i].keep_going = false;
    }

    for(uint64_t i=0; i<2; i++)
    {
        float_num_free(line_tryget_response(&line_a_b[i]));
        float_num_free(line_tryget_response(&line_b_d[i]));
        float_num_free(line_tryget_response(&line_c_d[i]));
        float_num_free(line_tryget_response(&zip_d_pi.lines[i]));
    }

    for(uint64_t i=0; i<2; i++)
    {
        pthread_join(pid_a[i], NULL);
        pthread_join(pid_b[i], NULL);
        pthread_join(pid_c[i], NULL);
        pthread_join(pid_d[i], NULL);
    }

    for(uint64_t i=0; i<2; i++)
    {
        line_free(&line_a_b[i]);
        line_free(&line_b_d[i]);
        line_free(&line_c_d[i]);
    }
    zip_free(&zip_d_pi);

    float_num_free(flt_1);
    float_num_free(flt_m_1_2);

    printf("\n\n");
    float_num_display_dec(flt_pi);
    float_num_free(flt_pi);
} 
