#ifndef _RM_UTILS_H_
#define _RM_UTILS_H_

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


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

/*
    return x ← (a ^ d) mod n
    Currently is a very simple and low-efficiency function
    //  [Seq-Opt]: group the exponent into group of two, then recursively do mapping,
    This will make the complexity log(n)
    //  Maybe can't get optimized in parallel version?
*/
unsigned int expoMod(unsigned int a, unsigned int d, unsigned int n){
    unsigned int x = 1;

    a = a % n;
    for(unsigned int i=0; i<d; i++){
        x = x * a;
        x = x % n;
    }

    return x;
}


#endif