#include "aks.h"
/* int main (int argc, char* argv[]) */
/* { */
/*   mpz_t n; */
/*   mpz_init_set_str(n, argv[1], 10); */
/*   printf("%d\n", aks(n)); */
/*   mpz_clear(n); */
/*   return 0; */
/* } */



/* int main (int argc, char* argv[]) */
/* { */
/*   char n_str[1000]; */
/*   scanf("%s", n_str); */
/*   mpz_t n; */
/*   mpz_init_set_str(n, n_str, 10); */
/*   gmp_printf("%Zd\n", n); */
/*   printf("%d\n", aks(n)); */
/*   mpz_clear(n); */
/*   return 0; */
/* } */



int main (int argc, char* argv[])
{
  char n_str[1000];
  FILE* fp = fopen(argv[1], "r");
  mpz_t n;
  mpz_init(n);
  while (fscanf(fp, "%s", n_str) != EOF) {
    clock_t start = clock();
    mpz_set_str(n, n_str, 10);
    gmp_printf("%Zd: ", n);
    printf("%d\n", aks(n));
    printf("Time: %f\n", 
	   (double)(clock() - start) / (double)CLOCKS_PER_SEC);
  }
  mpz_clear(n);
  fclose(fp);
  return 0;
}
