#ifndef _RM_UTILS_H_
#define _RM_UTILS_H_

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define _64BIT_

#ifndef  _64BIT_
    typedef unsigned int rm_int;
#else
    typedef unsigned long long rm_int;
#endif

/*
    A helper function to get a random value in the range [2, n]
    //  [Seq-Opt]: the random is actually not real random number, need to find a way
    to generate real random number.
    //  In parallel version, we need to find a way to generate efficiently
*/
unsigned int getRandom(unsigned int n){
    unsigned int r = rand() % (n+1);
    unsigned int ret = r > 2 ? r : 2;
    return ret;   //  Should consider 0
}

/*  *** Deprecated ***
    return x ← (a ^ d) mod n
    Currently is a very simple and low-efficiency function
    //  (Done) [Seq-Opt]: group the exponent into group of two
    //  ? Maybe can't get optimized in parallel version?
*/
unsigned int slow_expoMod(unsigned int a, unsigned int d, unsigned int n){
    unsigned int x = 1;

    a = a % n;
    for(unsigned int i=0; i<d; i++){
        x = x * a;
        x = x % n;
    }

    return x;
}


/*
    return x ← (a ^ d) mod n
    Faster version, take O(logn) time
*/
unsigned int expoMod(unsigned int a, unsigned int d, unsigned int n){
    unsigned int ret = 1;

    while (d > 0) {
        if (d & 1) ret = ret * a % n;
        a = a * a % n;
        d >>= 1;
    }

    return ret;
}

#endif