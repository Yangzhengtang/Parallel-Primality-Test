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

static int breakcount[128] = {0};
static int bk_prime[0xFFFF] = {0};
int bignum_rabin_miller_helper(mpz_t test_num, int k){
    //  Sanity check
    //  mpz_t two;
    //  mpz_init_set_ui(two, 2);
    if(mpz_cmp_ui(test_num, 2) < 0)
        return 0;
    if(mpz_cmp_ui(test_num, 2) == 0)   
        return 1;
    if(mpz_cmp_ui(test_num, 3) == 0)   
        return 1;   

    mpz_t temp;
    mpz_init(temp);
    unsigned int easy_factor[8] = {2,3,5,7,11,13,17,19};
    for(int i=0; i<8; i++)
    {
        mpz_mod_ui(temp, test_num, easy_factor[i]);
        if(mpz_cmp_ui(temp, 0) == 0){
            //  quick return
            mpz_clear(temp);
            return 0;
        }
    }
    int b_c = 0;
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
            {
                bk_prime[0] += 1;
                continue;
                }
        //  The s should be less than 2^32, so we can just use an int to iterate
        for(unsigned r=1; mpz_cmp_ui(s, r)>0; r++){
            mpz_powm(x, x, two, test_num);   //  This maybe is not the right way...
            //  printf("\tAt round %llu, got %llu\n", r, x);
            if(mpz_cmp_ui(x,1)==0){
                //printf("got witness at %d\n", x)
                b_c = i;
                goto ret_0;
            }  
            if(mpz_cmp(x, rand_limit)==0){
                early_terminate = 1;
                bk_prime[r] += 1;
                break;
            }
        }

        if(!early_terminate){
            //printf("can't early return\n");
            b_c = i;
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
    mpz_clear(s);
    ret = 0;
    breakcount[b_c] += 1;
    return ret;
}



int bignum_rabin_miller_sf(mpz_t test_num, int k){
    //  Sanity check
    //  mpz_t two;
    //  mpz_init_set_ui(two, 2);
    if(mpz_cmp_ui(test_num, 2) < 0)
        return 0;
    if(mpz_cmp_ui(test_num, 2) == 0)   
        return 1;
    if(mpz_cmp_ui(test_num, 3) == 0)   
        return 1;   

    /* mpz_t temp;
    mpz_init(temp);
    unsigned int easy_factor[8] = {2,3,5,7,11,13,17,19};
    for(int i=0; i<8; i++)
    {
        mpz_mod_ui(temp, test_num, easy_factor[i]);
        if(mpz_cmp_ui(temp, 0) == 0){
            //  quick return
            mpz_clear(temp);
            return 0;
        }
    } */

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
    mpz_clear(s);
    ret = 0;
    return ret;
}



#define MAX_ROUND  0xFFFF
static char bigNumIsPrime[MAX_ROUND] = {0};

#define THREAD_NUM 4
#define SEQ_PART 64
/* void init_local(int k){
    for(int i=0; i<=k; i++){
        mpz_init(a_array[i]);
        mpz_init(x_array[i]);
    }
}

void clear_local(int k){
    for(int i=0; i<=k; i++){
        mpz_clear(a_array[i]);
        mpz_init(x_array[i]);
    }
} */

int bignum_rabin_miller_para(mpz_t test_num, int k){
    if(mpz_cmp_ui(test_num, 2) < 0)
        return 0;
    if(mpz_cmp_ui(test_num, 2) == 0)   
        return 1;
    if(mpz_cmp_ui(test_num, 3) == 0)   
        return 1;
    /* mpz_t temp;
    mpz_init(temp);
    unsigned int easy_factor[8] = {2,3,5,7,11,13,17,19};
    for(int i=0; i<8; i++)
    {
        mpz_mod_ui(temp, test_num, easy_factor[i]);
        if(mpz_cmp_ui(temp, 0) == 0){
            //  quick return
            mpz_clear(temp);
            return 0;
        }
    } */

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
   

    struct timeval tv;
    gettimeofday(&tv, NULL);
    gmp_randstate_t rstate;
    gmp_randinit_default(rstate);
    gmp_randseed_ui(rstate, tv.tv_usec);

    memset(bigNumIsPrime, 0, sizeof(char)*(k+1));
    volatile char shouldStop = 0;

//  Sequential Component
    mpz_t la;
    mpz_t lx;
    mpz_init(la);
    mpz_init(lx);
    for(int i=0; i<1; i++){    //  [Par-Opt]   This for loop can be performed in parallel
        mpz_urandomm(la, rstate, rand_limit);    //  pick a randomly in the range [2, n − 1]
        while(mpz_cmp_ui(la, 2)<0){
            mpz_urandomm(la, rstate, rand_limit);
        }
        //  gmp_printf("Random: %Zd \n", a);
        mpz_powm(lx, la, d, test_num);
        int early_terminate = 0;
        if(mpz_cmp_ui(lx,1)==0 || mpz_cmp(lx, rand_limit)==0)   //  if x = 1 or x = n − 1 then do next LOOP
            continue;
        //  The s should be less than 2^32, so we can just use an int to iterate
        for(unsigned r=1; mpz_cmp_ui(s, r)>0; r++){
            mpz_powm(lx, lx, two, test_num);   //  This maybe is not the right way...
            //  printf("\tAt round %llu, got %llu\n", r, x);
            if(mpz_cmp_ui(lx,1)==0){
                //printf("got witness at %d\n", x)
                goto ret_0;
            }  
            if(mpz_cmp(lx, rand_limit)==0){
                early_terminate = 1;
                break;
            }
        }

        if(!early_terminate){
            //printf("can't early return\n");
            goto ret_0;   //  [Seq-Opt]: can use a goto to reduce this
        }   
    }

//  Parallel component
    #pragma omp parallel for num_threads(THREAD_NUM)
    for(int i=0; i<k; i++){ 
        if(shouldStop){
            continue;
        }    
        
        mpz_t a;
        mpz_t x;
        mpz_init(a);
        mpz_init(x);

        mpz_urandomm(a, rstate, rand_limit);    //  pick a randomly in the range [2, n − 1]
        
        while(mpz_cmp_ui(a, 2)<0){
            mpz_urandomm(a, rstate, rand_limit);
        }
        
        mpz_powm(x, a, d, test_num);
        int early_terminate = 0;
        if(mpz_cmp_ui(x,1)==0 || mpz_cmp(x, rand_limit)==0){
            mpz_clear(a);
            mpz_clear(x);
            bigNumIsPrime[i] = 1;
            continue;
        }   //  if x = 1 or x = n − 1 then do next LOOP
            
        //  The s should be less than 2^32, so we can just use an int to iterate
        for(unsigned r=1; mpz_cmp_ui(s, r)>0; r++){
            if(shouldStop){
                break;
            }
            mpz_powm(x, x, two, test_num);   //  This maybe is not the right way...
            if(mpz_cmp_ui(x,1)==0){
                break;
            }  
            if(mpz_cmp(x, rand_limit)==0){
                early_terminate = 1;
                break;
            }
        }
        mpz_clear(a);
        mpz_clear(x);
        if(!early_terminate){
            bigNumIsPrime[i] = 0;
            shouldStop = 1;
        }  
        else{
            bigNumIsPrime[i] = 1;
        } 
    }

    ret = 1;

    for (int i = 0; i < k; i++)
        ret = ret && bigNumIsPrime[i];

    mpz_clear(d);
    mpz_clear(s);
    mpz_clear(rand_limit);
    mpz_clear(two);
    return ret;

ret_0:
    mpz_clear(d);
    mpz_clear(rand_limit);
    mpz_clear(two);
    mpz_clear(la);
    mpz_clear(lx);
    mpz_clear(s);
    ret = 0;
    return ret;
}

/* 
int bignum_naive(mpz_t test_num){
    mpz_t div;
    mpz_init_set_ui(div, 2);
    
    //  mpz_init_set(div, test_num);
    //  mpz_sub(div, div, 1);

    volatile char shouldReturn = 0;

#pragma omp parallel for num_threads(THREAD_NUM)
    for(int x=0;mpz_cmp(div, test_num) < 0; mpz_add(div, div,1)){
        if (shouldReturn)
            continue;
        mpz_t temp;
        mpz_init(temp);
        mpz_div(temp, test_num, div);
        if(mpz_sgn(temp)==0)
            shouldReturn = 1;
        mpz_clear(temp);
    }

    mpz_clear(div);
    return (shouldReturn == 0);
} */
