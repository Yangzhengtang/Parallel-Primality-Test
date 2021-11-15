#include "aks.h"

// Bernstein’s improvement
int aks (mpz_t n) {
  /* Step 1: perfect power */
  if (mpz_perfect_power_p(n)) {
    return COMPOSITE;
  }

  /* Step 2: witness search*/
  mpz_t r;
  mpz_init_set_ui(r, 2);
  unsigned int r_ui = 2;
  unsigned int logn_ui = mpz_sizeinbase(n, 2);
  mpz_t logn;
  mpz_init_set_ui(logn, logn_ui);
  mpz_t imax;			/* Upper bound of i = 4 * logn ^ 2*/
  mpz_init(imax);
  mpz_mul(imax, logn, logn);
  mpz_mul_ui(imax, imax, 4);
  Sieve sieve;			/* Sieve of Eratosthenes */
  initialize_sieve(&sieve);

  // find smallest r such that: ord_r(n) > log^2(n)
  while (mpz_cmp(r, n) < 0) {
    if (mpz_divisible_p(n, r)) {
      mpz_clear(r);
      mpz_clear(logn);
      mpz_clear(imax);
      destroy_sieve(&sieve);
      return COMPOSITE;
    }

    if (sieve_primality_test(r_ui, &sieve) == PRIME) {
      int is_break = 0;
      mpz_t pwm;
      mpz_init(pwm);
      mpz_t i;
      mpz_init_set_ui(i, 1);
      while (mpz_cmp(i, imax) <= 0) {
	      mpz_powm(pwm, n, i, r); // pwm = n ^ i % r
        if (mpz_cmp_ui(pwm, 1) != 0) {
          is_break = 1;
          break;
        }
	      mpz_add_ui(i, i, 1);
      }
      mpz_clear(pwm);
      mpz_clear(i);
      if (is_break) {
	      break;
      }
    }

    mpz_add_ui(r, r, 1);
    r_ui++;
  }

  mpz_clear(imax);
  destroy_sieve(&sieve);
  if (mpz_cmp(r, n) == 0) {
    mpz_clear(r);
    mpz_clear(logn);
    return PRIME;
  }

  
  /* Step 3: polynomial check */
  mpz_t amax;			/* Upper bound of a = 2 * sqrt(r) * logn */
  mpz_init_set_ui(amax, 2);
  mpz_t sqrtr;
  mpz_init(sqrtr);
  mpz_sqrt(sqrtr, r);
  mpz_mul(amax, amax, sqrtr);
  mpz_mul(amax, amax, logn);
  mpz_clear(sqrtr);
  mpz_clear(logn);

  mpz_t power_right;
  mpz_init(power_right);
  mpz_mod(power_right, n, r);
  unsigned int power_right_ui = mpz_get_ui(power_right);
  mpz_clear(r);
  mpz_clear(power_right);
  unsigned int is_returns = 0;
#ifndef USE_PARALLEL
    // sequential
    Polynomial* p_poly_right;
    Polynomial* p_poly_left; 
    Polynomial* p_poly_left_base;
    initialize_polynomial(&p_poly_right, power_right_ui);
    set_polynomial_coef_si(p_poly_right, power_right_ui, 1); /* X ^ (n % r) */
    initialize_polynomial(&p_poly_left_base, 1);
    set_polynomial_coef_si(p_poly_left_base, 1, 1); /* X */
    mpz_t a;
    mpz_init_set_ui(a, 1);
    mpz_t a_mod_n;
    mpz_init(a_mod_n);
    while (mpz_cmp(a, amax) <= 0) {
      mpz_mod(a_mod_n, a, n);
      set_polynomial_coef(p_poly_right, 0, &a_mod_n); /* X ^ (n % r) + a % n */
      set_polynomial_coef(p_poly_left_base, 0, &a); /* X + a */
      polynomial_modular_power(&p_poly_left, p_poly_left_base, n, r_ui);
      if (!is_equal_polynomial(p_poly_left, p_poly_right)) {
        destroy_polynomial(&p_poly_left);
        destroy_polynomial(&p_poly_right);
        destroy_polynomial(&p_poly_left_base);
        mpz_clear(amax);
        mpz_clear(a);
        mpz_clear(a_mod_n);
        return COMPOSITE;
      }
      destroy_polynomial(&p_poly_left);
      mpz_add_ui(a, a, 1);
    }
    destroy_polynomial(&p_poly_right);
    destroy_polynomial(&p_poly_left_base);
    mpz_clear(amax);
    mpz_clear(a);
    mpz_clear(a_mod_n);
#else
  mpz_t a;
#pragma omp parallel private(a)
{
  mpz_init_set_ui(a, omp_get_thread_num());

  for (; is_returns == 0 && mpz_cmp(a, amax) < 0; mpz_add_ui(a, a, omp_get_num_threads())) {
    mpz_t a_mod_n;
    mpz_init(a_mod_n);
    mpz_mod(a_mod_n, a, n);
    Polynomial* p_poly_right;
    Polynomial* p_poly_left;
    Polynomial* p_poly_left_base;
    initialize_polynomial(&p_poly_right, power_right_ui);
    set_polynomial_coef_si(p_poly_right, power_right_ui, 1);
    set_polynomial_coef(p_poly_right, 0, &a_mod_n);
    initialize_polynomial(&p_poly_left_base, 1);
    set_polynomial_coef_si(p_poly_left_base, 1, 1);
    set_polynomial_coef(p_poly_left_base, 0, &a);
    polynomial_modular_power(&p_poly_left, p_poly_left_base, n, r_ui);
    if (!is_equal_polynomial(p_poly_left, p_poly_right)) {
      #pragma omp atomic
      is_returns++;
    }
    mpz_clear(a_mod_n);
    destroy_polynomial(&p_poly_right);
    destroy_polynomial(&p_poly_left);
    destroy_polynomial(&p_poly_left_base);
  } /* end while a */
} /* end omp parallel block */
  mpz_clear(amax);
#endif

  return is_returns > 0 ? COMPOSITE : PRIME;;
}