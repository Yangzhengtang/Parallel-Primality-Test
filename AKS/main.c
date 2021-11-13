#include <getopt.h>
#include "aks.h"

void usage() {
  printf("Usage: aks [-options] [args]");
  printf("Options: \n");
  printf("\t -n [number]: test a single number's primality\n");
  printf("\t -f [filename]: test numbers from a given file. every line of this file has a single number.\n");
  printf("\t -p: parallelly execute AKS algorithm.\n");
}

void testFromFile(char* filename) {
  char n_str[1000];
  FILE* fp = fopen(filename, "r");
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
}

int main (int argc, char* argv[]) {
  char c;
  extern char *optarg;
  int fileFlag, numFlag, parallelFlag;
  char *num = NULL, *filename = NULL; 
  while ((c = getopt (argc, argv, "hpf:n:")) != -1) {
    switch (c) {
      case 'f':
        fileFlag = 1;        
        filename = optarg;
        break;
      case 'n':
        numFlag = 1;
        num = optarg;
        break;
      case 'p':
        parallelFlag = 1;
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
    printf("filename: %s \n", filename);
    testFromFile(filename);
  }

  if (numFlag) {
    mpz_t n;
    mpz_init_set_str(n, num, 10);
    printf("%d\n", aks(n));
    printf("n: %s\n", num);
    mpz_clear(n);
  }
  
  return 0;
}
