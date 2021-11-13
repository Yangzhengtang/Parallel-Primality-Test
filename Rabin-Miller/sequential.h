#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.h"

/*
    Rabin-Miller Algo
    Pseudocodes from Wiki:
    -----------------------------------------------------------------------
        Input: n > 2, an odd integer to be tested for primality;
            k, a parameter that determines the accuracy of the test
        Output: composite if n is composite, otherwise probably prime
        write n − 1 as 2s·d with d odd by factoring powers of 2 from n − 1
        LOOP: repeat k times:
        pick a randomly in the range [2, n − 1]
        x ← ad mod n
        if x = 1 or x = n − 1 then do next LOOP
        for r = 1 .. s − 1
            x ← x2 mod n
            if x = 1 then return composite
            if x = n − 1 then do next LOOP
        return composite
        return probably prime
    -----------------------------------------------------------------------
*/

/*
    Return: 0 if n is composite, 1 if probably prime
*/
int rabin_miller_sf(rm_int test_num, rm_int k){
    //  First do some input validation
    if(test_num < 2)    return 0;
    if(test_num == 2)   return 1;

    //  write n − 1 as (2^s)*d with d odd by factoring powers of 2 from n − 1
    rm_int d = test_num - 1;
    rm_int s = 0;

    //  Keep dividing the temp_num until it is an odd number
    while(!(d & 1)){
        d = d >> 1;
        s += 1;
    }

    for(rm_int i=0; i<k; i++){    //  [Par-Opt]   This for loop can be performed in parallel
        rm_int a = getRandom(test_num - 1);   //  pick a randomly in the range [2, n − 1]
        //printf("The random number is %d\n", a);
        rm_int x = expoMod(a, d, test_num);
        //printf("\tGot x: %d\n", x);
        int early_terminate = 0;
        if((x == 1) || (x == test_num-1))   //  if x = 1 or x = n − 1 then do next LOOP
            continue;
        for(rm_int r=1; r<s; r++){
            x = expoMod(x, 2, test_num);
            //printf("\tAt round %d, got %d\n", r, x);
            if(x == 1){
                //printf("got witness at %d\n", x);
                return 0;
            }  
            if(x == test_num-1){
                early_terminate = 1;
                break;
            }
        }

        if(!early_terminate){
            //printf("can't early return\n");
            return 0;   //  [Seq-Opt]: can use a goto to reduce this
        }   
    }

    return 1;
}


/*
    Return: 0 if n is composite, 1 if probably prime
    A shitty version, should be very very slow
*/
int rabin_miller_shitty(rm_int test_num, rm_int k){
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

    int *isPrime = (int*) (malloc(sizeof(int) * k));

    for (rm_int i = 0; i < k; i++)
    { //  [Par-Opt]   This for loop can be performed in parallel
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
            isPrime[k] = 0; //  [Seq-Opt]: can use a goto to reduce this
        else
            isPrime[k] = 1;
    }

    int ret = 1;
    for (rm_int i = 0; i < k; i++)
        ret = ret && isPrime[k];

    free(isPrime);
    return ret;
}

