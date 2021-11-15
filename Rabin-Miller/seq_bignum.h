#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.h"
#include <gmp.h>



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
int bignum_rabin_miller_sf(mpz_t test_num, int k){
    //  Sanity check
    //  mpz_t two;
    //  mpz_init_set_ui(two, 2);
    if(mpz_cmp_ui(test_num, 2) < 0)
        return 0;
    if(mpz_cmp_ui(test_num, 2) == 0)   
        return 1;
    
    //  write n − 1 as (2^s)*d with d odd by factoring powers of 2 from n − 1
    int ret = 0;
    mpz_t d;
    mpz_t s;

    mpz_init(d);
    mpz_init_set_ui(s, 0);
    mpz_sub_ui(d, test_num, 1);

    //  Keep dividing the temp_num until it is an odd number
    while(mpz_even_p(d)){
        mpz_fdiv_q_2exp(d, d, 1);
        mpz_add_ui(s, s, 1);
    }

    mpz_t rand_limit;   //  test_num - 1, for the following random generation
    mpz_init(rand_limit);
    mpz_sub_ui(rand_limit, test_num, 1);
    
    mpz_t two;
    mpz_init_set_ui(two, 2);

    //  These variable can't be shared in parallel version
    mpz_t a;
    mpz_t x;
    mpz_init(a);
    mpz_init(x);

    struct timeval tv;
    gettimeofday(&tv, NULL);
    gmp_randstate_t rstate;
    gmp_randinit_default(rstate);
    gmp_randseed_ui(rstate, tv.tv_usec);

    //  gmp_printf("d: %Zd, s: %Zd \n", d, s);

    for(int i=0; i<k; i++){    //  [Par-Opt]   This for loop can be performed in parallel
        mpz_urandomm(a, rstate, rand_limit);    //  pick a randomly in the range [2, n − 1]
        while(mpz_cmp_ui(a, 2)<0){
            mpz_urandomm(a, rstate, rand_limit);
        }
        //  gmp_printf("Random: %Zd \n", a);
        mpz_powm(x, a, d, test_num);
        int early_terminate = 0;
        if(mpz_cmp_ui(x,1)==0 || mpz_cmp(x, rand_limit)==0)   //  if x = 1 or x = n − 1 then do next LOOP
            continue;
        //  The s should be less than 2^32, so we can just use an int to iterate
        for(unsigned r=1; mpz_cmp_ui(s, r)>0; r++){
            mpz_powm(x, x, two, test_num);   //  This maybe is not the right way...
            //  printf("\tAt round %llu, got %llu\n", r, x);
            if(mpz_cmp_ui(x,1)==0){
                //printf("got witness at %d\n", x)
                goto ret_0;
            }  
            if(mpz_cmp(x, rand_limit)==0){
                early_terminate = 1;
                break;
            }
        }

        if(!early_terminate){
            //printf("can't early return\n");
            goto ret_0;   //  [Seq-Opt]: can use a goto to reduce this
        }   
    }

    ret = 1;

    return ret;
ret_0:
    mpz_clear(d);
    mpz_clear(rand_limit);
    mpz_clear(two);
    mpz_clear(a);
    mpz_clear(x);
    ret = 0;
    return ret;
}