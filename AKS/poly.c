#include "poly.h"

inline void get_polynomial_coef (mpz_t* p_coef, Polynomial* p_poly, unsigned int order) {
  if (order > p_poly->deg) {
    mpz_init_set_ui(*p_coef, 0);
    return;
  }
  mpz_set(*p_coef, p_poly->coef[order]);
}

void initialize_polynomial (Polynomial** pp_poly, unsigned int deg)
{
  (*pp_poly) = (Polynomial*)malloc(sizeof(Polynomial));
  (*pp_poly)->coef = (mpz_t*)malloc((deg + 1) * sizeof(mpz_t));
  (*pp_poly)->deg = deg;
  unsigned int i;
  for (i = 0; i <= deg; i++) {
    mpz_init_set_ui((*pp_poly)->coef[i], 0);
  }
}

void destroy_polynomial (Polynomial** pp_poly)
{
  unsigned int i;
  for (i = 0; i <= (*pp_poly)->deg; i++) {
    mpz_clear((*pp_poly)->coef[i]);
  }
  free((*pp_poly)->coef);
  (*pp_poly)->deg = 0;
  free(*pp_poly);
}

void clone_polynomial(Polynomial** pp_poly_cloned, Polynomial* p_poly)
{
  (*pp_poly_cloned) = (Polynomial*)malloc(sizeof(Polynomial));
  (*pp_poly_cloned)->coef = (mpz_t*)malloc((p_poly->deg + 1) * sizeof(mpz_t));
  (*pp_poly_cloned)->deg = p_poly->deg;
  unsigned int i;
  for (i = 0; i <= p_poly->deg; i++) {
    mpz_init_set((*pp_poly_cloned)->coef[i], p_poly->coef[i]);
  }
}

void compact_polynomial (Polynomial* p_poly)
{
  unsigned int i;
  for (i = p_poly->deg; i > 0; i--) {
    if (mpz_cmp_ui(p_poly->coef[i], 0) != 0) {
      break;
    }
  }
  if (i < p_poly->deg) {
    unsigned int j;
    for (j = i + 1; j <= p_poly->deg; j++) {
      mpz_clear(p_poly->coef[j]);
    }
    p_poly->coef = (mpz_t*)realloc(p_poly->coef, sizeof(mpz_t) * (i + 1));
    p_poly->deg = i;
  }
}

/* Return 1 if two polynomials are equal */
int is_equal_polynomial (Polynomial* p_poly0, Polynomial* p_poly1)
{
  if (p_poly0->deg != p_poly1->deg) {
    return 0;
  }
  unsigned int i;
  for (i = 0; i <= p_poly0->deg; i++) {
    if (mpz_cmp(p_poly0->coef[i], p_poly1->coef[i]) != 0) {
      return 0;
    }
  }  
  return 1;
}

void set_polynomial_coef (Polynomial* p_poly, unsigned int order, mpz_t* p_coef)
{
  if (order <= p_poly->deg) {
    mpz_set(p_poly->coef[order], *p_coef);
    return;
  }
  p_poly->coef = (mpz_t*)realloc(p_poly->coef, sizeof(mpz_t) * (order + 1));
  unsigned int i;
  for (i = p_poly->deg + 1; i < order; i++) {
    mpz_init_set_ui(p_poly->coef[i], 0);
  }
  mpz_init_set(p_poly->coef[order], *p_coef);
  p_poly->deg = order;
}

void set_polynomial_coef_si (Polynomial* p_poly, unsigned int order, int coef_si)
{
  if (order <= p_poly->deg) {
    mpz_set_si(p_poly->coef[order], coef_si);
    return;
  }
  p_poly->coef = (mpz_t*)realloc(p_poly->coef, sizeof(mpz_t) * (order + 1));
  unsigned int i;
  for (i = p_poly->deg + 1; i < order; i++) {
    mpz_init_set_ui(p_poly->coef[i], 0);
  }
  mpz_init_set_si(p_poly->coef[order], coef_si);
  p_poly->deg = order;
}

void polynomial_modular_multiplication (Polynomial** pp_poly_res, 
					Polynomial* p_poly0, Polynomial* p_poly1, 
					mpz_t n, unsigned int r)
{
  unsigned int max_output_deg = p_poly0->deg + p_poly1->deg < r? 
    p_poly0->deg + p_poly1->deg: r - 1;
  initialize_polynomial(pp_poly_res, max_output_deg);
  mpz_t coef;
  mpz_init(coef);
  unsigned int i;
  for (i = 0; i < r; i++) {	/* For every result coefficient */
    mpz_set_ui(coef, 0);
    mpz_t c0, c1;
    mpz_init(c0);
    mpz_init(c1);
    unsigned int jmin = i > p_poly1->deg? i - p_poly1->deg: 0;
    unsigned int jmax = i < p_poly0->deg? i: p_poly0->deg;
    unsigned int j;
    for (j = jmin; j <= jmax; j++) { /* For all a(j) * b(i - j) */
      get_polynomial_coef(&c0, p_poly0, j);
      get_polynomial_coef(&c1, p_poly1, i - j);
      mpz_mul(c0, c0, c1);
      mpz_add(coef, c0, coef);	/* coef += c0 * c1 */
    }
    jmin = i + r - p_poly1->deg;
    jmax = p_poly0->deg;
    for (j = jmin; j <= jmax; j++) { /* For all a(j) * b(i - j + r) */
      get_polynomial_coef(&c0, p_poly0, j);
      get_polynomial_coef(&c1, p_poly1, i + r - j);
      mpz_mul(c0, c0, c1);
      mpz_add(coef, c0, coef);	/* coef += c0 * c1 */
    }
    mpz_clear(c0);
    mpz_clear(c1);
    mpz_mod(coef, coef, n);	/* coef = coef % n */
    if (mpz_cmp_ui(coef, 0) != 0) {
      set_polynomial_coef((*pp_poly_res), i, &coef);
    }
  }
  compact_polynomial((*pp_poly_res));
  mpz_clear(coef);
}

/* Compute ((*p_poly_base) ^ n) % (X ^ r - 1) */
void polynomial_modular_power (Polynomial** pp_poly_res, Polynomial* p_poly_base, 
			       mpz_t n, unsigned int r)
{
  initialize_polynomial(pp_poly_res, 0);
  set_polynomial_coef_si((*pp_poly_res), 0, 1);
  Polynomial* p_poly_temp = NULL;
  unsigned int i;
  for (i = mpz_sizeinbase(n, 2) + 1; i> 0; ) {
    clone_polynomial(&p_poly_temp, (*pp_poly_res));
    destroy_polynomial(pp_poly_res);
    polynomial_modular_multiplication(pp_poly_res, p_poly_temp, p_poly_temp, n, r);
    destroy_polynomial(&p_poly_temp);
    if (mpz_tstbit(n, --i) == 1) {
      clone_polynomial(&p_poly_temp, (*pp_poly_res));
      destroy_polynomial(pp_poly_res);
      polynomial_modular_multiplication(pp_poly_res, p_poly_temp, p_poly_base, n, r);
      destroy_polynomial(&p_poly_temp);
    }
  }
}
