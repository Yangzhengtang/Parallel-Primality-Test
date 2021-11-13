#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utils.h"

#define PTHREAD 1

#define MAX_ROUND  (rm_int)(0xFFFF)
static char isPrime[MAX_ROUND] = {0};

int rabin_miller_shitty_parallel(rm_int test_num, rm_int k)
{
    //  First do some input validation
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

    int *localisPrime = (int *)(malloc(sizeof(int) * (k + 1)));
    memset(localisPrime, 0, sizeof(int) * (k + 1));

#pragma omp parallel for num_threads(PTHREAD)
    for (rm_int i = 0; i < k; i++)
    {                                             //  [Par-Opt]   This for loop can be performed in parallel
        rm_int a = getRandom(test_num - 1); //  pick a randomly in the range [2, n − 1]
        rm_int x = expoMod(a, d, test_num);
        int early_terminate = 0;
        if ((x == 1) || (x == test_num - 1)) //  if x = 1 or x = n − 1 then do next LOOP
        {
            early_terminate = 1;
        }
        for (rm_int r = 1; r < s; r++)
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
            localisPrime[i] = 0; //  [Seq-Opt]: can use a goto to reduce this
        else
            localisPrime[i] = 1;
    }

    int ret = 1;
    for (rm_int i = 0; i < k; i++)
        ret = ret && localisPrime[i];

    free(localisPrime);
    return ret;
}

//  Break down the process into small steps and make each step parallel
int rabin_miller_v1_parallel(rm_int test_num, rm_int k)
{
    //  First do some input validation
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

    rm_int *randoms = (rm_int *)(malloc(sizeof(rm_int) * (k + 1)));
    rm_int *mytemp = (rm_int *)(malloc(sizeof(rm_int) * (k + 1)));
    int *terminate = (int *)(malloc(sizeof(int) * (k + 1)));
    memset(terminate, 0, sizeof(int) * (k + 1));

//  Now divide this process into steps and paralize each step individually:

//  Generate Random Number, and do the first round calculation
#pragma omp parallel for num_threads(PTHREAD)
    for (rm_int i = 0; i < k; i++)
    {
        randoms[i] = getRandom(test_num - 1);
        mytemp[i] = expoMod(randoms[i], d, test_num);
        if ((mytemp[i] == 1) || (mytemp[i] == test_num - 1))
            terminate[i] = 1;
    }

    //  After first round, we can determine
    int all_done = 1;
    for (rm_int i = 0; i < k; i++)
    {
        all_done = all_done && terminate[i];
    }
    if (all_done){
        free(randoms);
        free(mytemp);
        free(terminate);
        return 1;
    }
        

    for (rm_int r = 1; r < s; r++)
    {
#pragma omp parallel for num_threads(PTHREAD)
        for (rm_int i = 0; i < k; i++)
        {
            if (terminate[i] != 1)
            {
                mytemp[i] = expoMod(mytemp[i], 2, test_num);
                if (mytemp[i] == test_num - 1)
                    terminate[i] = 1;
            }
        }

        //  Sequential examine
        for (rm_int i = 0; i < k; i++)
        {
            if (terminate[i] == 0 && mytemp[i] == 1) //  Any time we got this answer, it should be composite
            {
                free(randoms);
                free(mytemp);
                free(terminate);
                return 0;
            }
        }

        all_done = 1;
        for (rm_int i = 0; i < k; i++)
        {
            all_done = all_done && (terminate[i]);
        }
        if (all_done)
        {
            free(randoms);
            free(mytemp);
            free(terminate);
            return 1;
        }       
    }


    free(randoms);
    free(mytemp);
    free(terminate);
    return 0;
}


static rm_int temp[MAX_ROUND] = {0};
static char finish_checking[MAX_ROUND] = {0};

//  Break down the process into small steps and make each step parallel
int rabin_miller_v1_parallel_opt(rm_int test_num, rm_int k)
{
    //  First do some input validation
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

    //  rm_int *randoms = (rm_int *)(malloc(sizeof(rm_int) * (k + 1)));     We don't actually need randoms here
    //  int *terminate = (int *)(malloc(sizeof(int) * (k + 1)));        
    //  rm_int *temp = (rm_int *)(malloc(sizeof(rm_int) * (k + 1)));

    memset(isPrime, 0, sizeof(char) * (k + 1));
    memset(temp, 0, sizeof(rm_int) * (k+1));
    memset(finish_checking, 0, sizeof(char) * (k+1));

    //  Now divide this process into steps and paralize each step individually:

    //  Generate Random Number, and do the first round calculation
    #pragma omp parallel for num_threads(PTHREAD)
    for (rm_int i = 0; i < k; i++)
    {
        temp[i] = getRandom(test_num - 1);
        temp[i] = expoMod(temp[i], d, test_num);
        if ((temp[i] == 1) || (temp[i] == test_num - 1))
            finish_checking[i] = 1;
    }

    //  It's very likely that we can determine now, if it is a prime
    int all_done = 1;
    for (rm_int i = 0; i < k; i++)
    {
        all_done = all_done && finish_checking[i];
    }
    if (all_done)
        return 1;

    //  If we can't determine now, start a new loop
    for (rm_int r = 1; r < s; r++)
    {
        
#pragma omp parallel for num_threads(PTHREAD)
        for (rm_int i = 0; i < k; i++)
        {
            if (finish_checking[i] != 1)
            {
                temp[i] = expoMod(temp[i], 2, test_num);
                if (temp[i] == test_num - 1)
                    finish_checking[i] = 1;
            }
        }

        //  Sequential examine
        for (rm_int i = 0; i < k; i++)
        {
            if (finish_checking[i] == 0 && temp[i] == 1) //  Any time we got this answer, it should be composite
            {
                //printf("When test %d, terminate here, i: %d, r: %d\n", test_num, i, r);
                return 0;
            }
        }

        all_done = 1;
        for (rm_int i = 0; i < k; i++)
        {
            all_done = all_done && (finish_checking[i]);
        }
        if (all_done)
            return 1;
    }

    return 0;
}


//  This time, ask openMP to break all running threads
int rabin_miller_v2_parallel(rm_int test_num, rm_int k)
{
    //  First do some input validation
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

    int *localisPrime = (int *)(malloc(sizeof(int) * (k + 1)));
    for(int i=0; i<k+1; i++)  localisPrime[i] = 1;
    volatile char shouldStop = 0;

#pragma omp parallel for num_threads(PTHREAD)
    for (rm_int i = 0; i < k; i++)
    {                     
        if(shouldStop)
            continue;
        rm_int a = getRandom(test_num - 1); //  pick a randomly in the range [2, n − 1]
        rm_int x = expoMod(a, d, test_num);
        int early_terminate = 0;
        if ((x == 1) || (x == test_num - 1)) //  if x = 1 or x = n − 1 then do next LOOP
        {
            continue;
        }
        for (rm_int r = 1; r < s; r++)
        {
            if(shouldStop)  //  Some other thread prove that it's a composite
            {
                break;
            }
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

        if (!early_terminate){
            localisPrime[i] = 0;
            shouldStop = 1;
        }
             //  [Seq-Opt]: can use a goto to reduce this
        else
            localisPrime[i] = 1;
    }

    int ret = 1;
    for (rm_int i = 0; i < k; i++)
        ret = ret && localisPrime[i];

    free(localisPrime);
    return ret;
}

//  Stop mallocing and freeing
int rabin_miller_v3_parallel(rm_int test_num, rm_int k)
{
    //  First do some input validation
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

    //  int *isPrime = (int *)(malloc(sizeof(int) * (k + 1)));
    //  for(int i=0; i<k+1; i++)  isPrime[i] = 1;
    memset(isPrime, 0, sizeof(char)*(k+1));

    volatile char shouldStop = 0;

#pragma omp parallel for num_threads(PTHREAD)
    for (rm_int i = 0; i < k; i++)
    {                     
        if(shouldStop)
            continue;
        rm_int a = getRandom(test_num - 1); //  pick a randomly in the range [2, n − 1]
        rm_int x = expoMod(a, d, test_num);
        int early_terminate = 0;
        if ((x == 1) || (x == test_num - 1)) //  if x = 1 or x = n − 1 then do next LOOP
        {
            isPrime[i] = 1;
            continue;
        }
        for (rm_int r = 1; r < s; r++)
        {
            if(shouldStop)  //  Some other thread prove that it's a composite
            {
                break;
            }
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

        if (!early_terminate){
            isPrime[i] = 0;
            shouldStop = 1;
        }
             //  [Seq-Opt]: can use a goto to reduce this
        else
            isPrime[i] = 1;
    }

    int ret = 1;
    for (rm_int i = 0; i < k; i++)
        ret = ret && isPrime[i];

    return ret;
}