#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "rm_utils.h"

#define ARRAY_MAX 512
#define STR_MAX 128
#define PTHREAD 8

static pthread_t running_threads[PTHREAD];

struct single_thread_RM_args
{
    unsigned int test_num;
    unsigned int k;
    unsigned int d;
    unsigned int s;
    int *ret;
};

int rabin_miller_shitty_parallel(unsigned int test_num, unsigned int k)
{
    //  First do some input validation
    if (test_num < 2)
        return 0;
    if (test_num == 2)
        return 1;

    //  write n − 1 as (2^s)*d with d odd by factoring powers of 2 from n − 1
    unsigned int d = test_num - 1;
    unsigned int s = 0;

    //  Keep dividing the temp_num until it is an odd number
    while (!(d & 1))
    {
        d = d >> 1;
        s += 1;
    }

    int *isComposite = (int *)(malloc(sizeof(int) * (k + 1)));

#pragma omp parallel for num_threads(PTHREAD)
    for (unsigned int i = 0; i < k; i++)
    {                                             //  [Par-Opt]   This for loop can be performed in parallel
        unsigned int a = getRandom(test_num - 1); //  pick a randomly in the range [2, n − 1]
        unsigned int x = expoMod(a, d, test_num);
        int early_terminate = 0;
        if ((x == 1) || (x == test_num - 1)) //  if x = 1 or x = n − 1 then do next LOOP
        {
            early_terminate = 1;
        }
        for (unsigned int r = 1; r < s; r++)
        {
            if (early_terminate)
                break;
            x = expoMod(x, 2, test_num);
            if (x == 1)
            {
                break;
            }
            if (x == test_num - 1)
            {
                early_terminate = 1;
                break;
            }
        }
        //  printf("RM from thread = %d, i is: %d\n", omp_get_thread_num(), i);

        if (!early_terminate)
            isComposite[k] = 0; //  [Seq-Opt]: can use a goto to reduce this
        else
            isComposite[k] = 1;
    }

    int ret = 1;
    for (unsigned int i = 0; i < k; i++)
        ret = ret && isComposite[k];

    free(isComposite);
    return ret;
}

void single_RM_thread(struct single_thread_RM_args *args)
{
    unsigned int test_num = args->test_num;
    unsigned int k = args->k;
    unsigned int d = args->d;
    unsigned int s = args->s;
    int *ret = args->ret;

    for (unsigned int i = 0; i < k; i++)
    {                                             //  [Par-Opt]   This for loop can be performed in parallel
        unsigned int a = getRandom(test_num - 1); //  pick a randomly in the range [2, n − 1]
        unsigned int x = expoMod(a, d, test_num);
        int early_terminate = 0;
        if ((x == 1) || (x == (test_num - 1))) //  if x = 1 or x = n − 1 then do next LOOP
            continue;

        for (unsigned int r = 1; r < s; r++)
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

int rm_shitty_parallel_pthread(unsigned int test_num, unsigned int k)
{
    //  In this case, the k better be the multiple of PTHREAD
    if (test_num < 2)
        return 0;
    if (test_num == 2)
        return 1;

    //  write n − 1 as (2^s)*d with d odd by factoring powers of 2 from n − 1
    unsigned int d = test_num - 1;
    unsigned int s = 0;

    //  Keep dividing the temp_num until it is an odd number
    while (!(d & 1))
    {
        d = d >> 1;
        s += 1;
    }

    // printf("*** Testing %d, d: %d, s: %d\n", test_num, d, s);
    int result[PTHREAD] = {0};
    struct single_thread_RM_args args[PTHREAD];

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

int rabin_miller_v1_parallel(unsigned int test_num, unsigned int k)
{
    //  First do some input validation
    if (test_num < 2)
        return 0;
    if (test_num == 2)
        return 1;

    //  write n − 1 as (2^s)*d with d odd by factoring powers of 2 from n − 1
    unsigned int d = test_num - 1;
    unsigned int s = 0;

    //  Keep dividing the temp_num until it is an odd number
    while (!(d & 1))
    {
        d = d >> 1;
        s += 1;
    }

    unsigned int *randoms = (unsigned int *)(malloc(sizeof(unsigned int) * (k + 1)));
    unsigned int *temp = (unsigned int *)(malloc(sizeof(unsigned int) * (k + 1)));
    int *terminate = (int *)(malloc(sizeof(int) * (k + 1)));
    memset(terminate, 0, sizeof(int) * (k + 1));

//  Now divide this process into steps and paralize each step individually:

//  Generate Random Number, and do the first round calculation
#pragma omp parallel for num_threads(PTHREAD)
    for (unsigned int i = 0; i < k; i++)
    {
        randoms[i] = getRandom(test_num - 1);
        temp[i] = expoMod(randoms[i], d, test_num);
        if ((temp[i] == 1) || (temp[i] == test_num - 1))
            terminate[i] = 1;
    }

    //  After first round, we can determine
    int all_done = 1;
    for (unsigned int i = 0; i < k; i++)
    {
        all_done = all_done && terminate[i];
    }
    if (all_done)
        return 1;

    for (unsigned int r = 1; r < s; r++)
    {
#pragma omp parallel for num_threads(PTHREAD)
        for (unsigned int i = 0; i < k; i++)
        {
            if (terminate[i] != 1)
            {
                temp[i] = expoMod(temp[i], 2, test_num);
                if (temp[i] == test_num - 1)
                    terminate[i] = 1;
            }
        }

        //  Sequential examine
        for (unsigned int i = 0; i < k; i++)
        {
            if (terminate[i] == 0 && temp[i] == 1) //  Any time we got this answer, it should be composite
            {
                //printf("When test %d, terminate here, i: %d, r: %d\n", test_num, i, r);
                return 0;
            }
        }

        all_done = 1;
        for (unsigned int i = 0; i < k; i++)
        {
            all_done = all_done && (terminate[i]);
        }
        if (all_done)
            return 1;
    }

    return 0;
}