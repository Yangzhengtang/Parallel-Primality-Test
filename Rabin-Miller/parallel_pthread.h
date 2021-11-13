#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "utils.h"

#define PTHREAD 8

static pthread_t running_threads[PTHREAD];

typedef struct single_thread_RM_args
{
    rm_int test_num;
    rm_int k;
    rm_int d;
    rm_int s;
    int *ret;
} single_thread_RM_args;

void* single_RM_thread(single_thread_RM_args *args)
{
    rm_int test_num = args->test_num;
    rm_int k = args->k;
    rm_int d = args->d;
    rm_int s = args->s;
    int *ret = args->ret;

    for (rm_int i = 0; i < k; i++)
    {                                             //  [Par-Opt]   This for loop can be performed in parallel
        rm_int a = getRandom(test_num - 1); //  pick a randomly in the range [2, n − 1]
        rm_int x = expoMod(a, d, test_num);
        int early_terminate = 0;
        if ((x == 1) || (x == (test_num - 1))) //  if x = 1 or x = n − 1 then do next LOOP
            continue;

        for (rm_int r = 1; r < s; r++)
        {
            x = expoMod(x, 2, test_num);
            if (x == 1)
            {
                *ret = 0;
                return;
            }
            if (x == test_num - 1)
            {
                early_terminate = 1;
                break;
            }
        }

        if (!early_terminate)
        {
            *ret = 0;
            return; //  [Seq-Opt]: can use a goto to reduce this
        }
    }

    *ret = 1;
    return;
}

int rabin_miller_pthread_parallel(rm_int test_num, rm_int k)
{
    //  In this case, the k better be the multiple of PTHREAD
    if (test_num < 2)
        return 0;
    if (test_num == 2)
        return 1;

    //  write n − 1 as (2^s)*d with d odd by factoring powers of 2 from n − 1
    rm_int d = test_num - 1;
    rm_int s = 0;

    //  Keep dividing the temp_num until it is an odd number
    while (!(d & 1))
    {
        d = d >> 1;
        s += 1;
    }

    // printf("*** Testing %d, d: %d, s: %d\n", test_num, d, s);
    int result[PTHREAD] = {0};
    single_thread_RM_args args[PTHREAD];

    for (int i = 0; i < PTHREAD; i++)
    {
        args[i].test_num = test_num;
        args[i].k = k / PTHREAD;
        args[i].d = d;
        args[i].s = s;
        args[i].ret = result + i;
        pthread_create(&(running_threads[i]), NULL, single_RM_thread, &args[i]);
    }

    for (int i = 0; i < PTHREAD; i++)
    {
        pthread_join(running_threads[i], NULL);
    }

    int ret = 1;
    for (int i = 0; i < PTHREAD; i++)
    {
        ret = ret && result[i];
    }

    return ret;
}




typedef struct single_thread_RM_args_opt
{
    rm_int test_num;
    rm_int k;
    rm_int d;
    rm_int s;
    char* stop;
    int *ret;
} single_thread_RM_args_opt;

void* single_RM_thread_opt(single_thread_RM_args_opt *args)
{
    rm_int test_num = args->test_num;
    rm_int k = args->k;
    rm_int d = args->d;
    rm_int s = args->s;
    char* stop =args->stop;
    int *ret = args->ret;

    for (rm_int i = 0; i < k; i++)
    {                                            
        if(*stop == 1)  
            return;

        rm_int a = getRandom(test_num - 1); //  pick a randomly in the range [2, n − 1]
        rm_int x = expoMod(a, d, test_num);
        int early_terminate = 0;
        if ((x == 1) || (x == (test_num - 1))) //  if x = 1 or x = n − 1 then do next LOOP
            continue;

        for (rm_int r = 1; r < s; r++)
        {
            if(*stop == 1)
                return;
            x = expoMod(x, 2, test_num);
            if (x == 1)
            {
                *stop = 1;
                *ret = 0;
                return;
            }
            if (x == test_num - 1)
            {
                early_terminate = 1;
                break;
            }
        }

        if (!early_terminate)
        {
            *stop = 1;
            *ret = 0;
            return; //  [Seq-Opt]: can use a goto to reduce this
        }
    }

    *ret = 1;
    return;
}

static single_thread_RM_args_opt args[PTHREAD] = {0};
static int result[PTHREAD] = {0};
static char myStop = 0;

int rabin_miller_pthread_parallel_opt(rm_int test_num, rm_int k)
{
    //  In this case, the k better be the multiple of PTHREAD
    if (test_num < 2)
        return 0;
    if (test_num == 2)
        return 1;

    //  write n − 1 as (2^s)*d with d odd by factoring powers of 2 from n − 1
    rm_int d = test_num - 1;
    rm_int s = 0;

    //  Keep dividing the temp_num until it is an odd number
    while (!(d & 1))
    {
        d = d >> 1;
        s += 1;
    }

    myStop = 0;
    for (int i = 0; i < PTHREAD; i++)
    {
        args[i].test_num = test_num;
        args[i].k = k / PTHREAD;
        args[i].d = d;
        args[i].s = s;
        args[i].ret = result + i;
        args[i].stop = &myStop;
        pthread_create(&(running_threads[i]), NULL, single_RM_thread_opt, &args[i]);
    }

    for (int i = 0; i < PTHREAD; i++)
    {
        pthread_join(running_threads[i], NULL);
    }

    int ret = 1;
    for (int i = 0; i < PTHREAD; i++)
    {
        ret = ret && result[i];
    }

    return ret;
}

