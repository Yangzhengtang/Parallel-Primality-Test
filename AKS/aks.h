#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "poly.h"
#include "sieve.h"

#define COMPOSITE 0
#define PRIME 1

#ifdef USE_PARALLEL
#include <omp.h>
#else
#include <time.h>
#endif

int aks (mpz_t n);