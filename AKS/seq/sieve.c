#include "sieve.h"

void initialize_sieve (Sieve* p_sieve)
{
  p_sieve->size = 2;
  mpz_init(p_sieve->table);
}

void destroy_sieve (Sieve* p_sieve)
{
  mpz_clear(p_sieve->table);
  p_sieve->size = 0;
}

int sieve_primality_test (unsigned int n, Sieve* p_sieve)
{
  if (n <= p_sieve->size) {
    return !mpz_tstbit(p_sieve->table, n);
  }
  unsigned int prev_size = p_sieve->size;
  p_sieve->size *= 2;
  unsigned int i;
  for (i = 2; i <= prev_size; i++) {
    if (mpz_tstbit(p_sieve->table, i) == 0) {
      unsigned int j;
      for (j = i * 2; j <= p_sieve->size; j += i) {
	mpz_setbit(p_sieve->table, j);
      }
    }
  }
  return sieve_primality_test(n, p_sieve);
}