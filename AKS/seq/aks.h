#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include "poly.h"
#include "sieve.h"

#define COMPOSITE 0
#define PRIME 1

// #define USE_PARALLEL

#ifdef USE_PARALLEL
#define THREAD_NUM 2
#endif

int aks (mpz_t n);