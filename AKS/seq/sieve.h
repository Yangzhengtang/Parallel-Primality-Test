#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

typedef struct {
  mpz_t table;
  unsigned int size;		/* Bit set 1: composite */
} Sieve;

void initialize_sieve (Sieve* p_sieve);
void destroy_sieve (Sieve* p_sieve);
int sieve_primality_test (unsigned int n, Sieve* p_sieve);
