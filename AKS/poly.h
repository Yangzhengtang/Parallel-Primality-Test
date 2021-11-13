#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

typedef struct {
  mpz_t* coef;
  unsigned int deg;
} Polynomial;

void initialize_polynomial (Polynomial** pp_poly, unsigned int deg);
void destroy_polynomial (Polynomial** pp_poly);
void clone_polynomial(Polynomial** pp_poly_cloned, Polynomial* p_poly);
void compact_polynomial (Polynomial* p_poly);
int  is_equal_polynomial (Polynomial* p_poly0, Polynomial* p_poly1);
void set_polynomial_coef (Polynomial* p_poly, unsigned int order, mpz_t* p_coef);
void set_polynomial_coef_si (Polynomial* p_poly, unsigned int order, int coef_si);
void polynomial_modular_multiplication (Polynomial** pp_poly_res, 
					Polynomial* p_poly0, Polynomial* p_poly1, 
					mpz_t n, unsigned int r);
void polynomial_modular_power (Polynomial** pp_poly_res, Polynomial* p_poly_base, 
			    mpz_t n, unsigned int r);
inline void get_polynomial_coef (mpz_t* p_coef, Polynomial* p_poly, unsigned int order) {
  if (order > p_poly->deg) {
    mpz_init_set_ui(*p_coef, 0);
    return;
  }
  mpz_set(*p_coef, p_poly->coef[order]);
}