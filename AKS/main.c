#include <getopt.h>
#include <unistd.h>
#include "aks.h"

void usage() {
  printf("Usage: aks [-options] [args]");
  printf("Options: \n");
  printf("\t -n [number]: test a single number's primality\n");
  printf("\t -f [filename]: test numbers from a given file. every line of this file has a single number.\n");
  printf("\t -t [n: number of threads]: parallelly execute AKS algorithm with n threads.\n");
}

void testNum(mpz_t n) {
    // timer start
    #ifdef USE_PARALLEL
    double start = omp_get_wtime();
    #else
    clock_t start = clock();
    #endif

    // prime test
    int isPrime = aks(n);

    // timer end
    #ifdef USE_PARALLEL
        double diff = omp_get_wtime() - start;
    #else
        double diff = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
    #endif

    // stats
    unsigned int logn = mpz_sizeinbase(n, 2);
    gmp_printf("%Zd, %d, %d, %f\n", n, logn, isPrime, diff);
}

void testFromFile(char* filename) {
  char n_str[1000];
  FILE* fp = fopen(filename, "r");
  mpz_t n;
  mpz_init(n);
  while (fscanf(fp, "%s", n_str) != EOF) {
    mpz_set_str(n, n_str, 10);
    testNum(n);
  }
  mpz_clear(n);
  fclose(fp);
}

int main (int argc, char* argv[]) {
  char c;
  extern char *optarg;
  int fileFlag = 0, numFlag = 0;
  char *num = NULL, *filename = NULL; 
  while ((c = getopt (argc, argv, "hf:n:")) != -1) {
    switch (c) {
      case 'f':
        fileFlag = 1;        
        filename = optarg;
        break;
      case 'n':
        numFlag = 1;
        num = optarg;
        break;
      case 'h':
        usage();
        break;
      case '?':
        return 1;
    }
  }

  // invalid args
  if (!fileFlag && !numFlag) {
    usage();
  }

  if (fileFlag) {
    testFromFile(filename);
  }

  if (numFlag) {
    mpz_t n;
    mpz_init_set_str(n, num, 10);
    testNum(n);
  }
  return 0;
}
