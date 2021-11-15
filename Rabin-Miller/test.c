#include "test_utils.h"
/*
    Test for the implementation:
        print_primality_single_func
        print_single_input
        print_prime_in_range
        print_prime_under_100_for_all
*/
void print_primality_single_func(mr_func func, rm_int test_num, rm_int test_round)
{
    my_srand();

    int result = func(test_num, test_round);
    
    printf("Checking " _rm_pformat_ ", got [%s]\n", test_num, result == 1 ? "prime" : "composite");
    //  int default_result = rabin_miller_sf(test_num, test_round);
    //  printf("Checking _rm_pformat_, got %s, default %s\n",
    //         test_num, result == 1 ? "prime" : "composite", default_result == 1 ? "prime" : "composite");
}

//  Single test, check the primality of a number given by user, by default check 40 rounds
void print_single_input()
{
    my_srand();

    rm_int testing_round = 40;
    rm_int test_num;
    printf("Please enter the number to check:\n");
    scanf(_rm_pformat_, &test_num);
    print_primality_single_func(rabin_miller_sf, test_num, testing_round);
}

void print_prime_in_range(rm_int start, rm_int end, rm_int round, int f_index)
{
    my_srand();

    mr_func func = mr_func_table[f_index];
    for (rm_int n = start; n < end; n++)
    {
        if (func(n, round))
            printf("\t\t" _rm_pformat_ "\n", n);
    }
}

void print_prime_under_100_for_all()
{
    my_srand();

    printf("----- Check the correctness for all Implementations -----\n");

    for (int i = 0; i < IMPL_COUNT; i++)
    {
        printf("\tNow checking [%s]...\n", mr_func_name_table[i]);
        print_prime_in_range(3, 100, 40, i);
    }
}

void print_prime_under_1000_for_all()
{
    my_srand();

    printf("----- Check the correctness for all Implementations -----\n");

    for (int i = 0; i < IMPL_COUNT; i++)
    {
        printf("\tNow checking [%s]...\n", mr_func_name_table[i]);
        print_prime_in_range(3, 1000, 40, i);
    }
}

void testFromInput() {
    //  bignum_init();
    char str[1024];
    printf("Please enter a large number\n");
    scanf("%s", str);
    mpz_t n;
    mpz_init(n);
    mpz_set_str(n, str, 10);
    
    int round = 128;
    int ans = bignum_rabin_miller_sf(n, round);

    mpz_clear(n);
    printf("Output: %d\n", ans);
}

void testForRange(unsigned long exp) {
    //  bignum_init();
    mpz_t n;
    mpz_init_set_ui(n, 2);
    mpz_pow_ui(n, n, exp);

    int round = 128;
    int range = 0x1000;

    clock_t t_start, diff;
    t_start = clock();

    for(int i=0; i<range; i++){
        int ans = bignum_rabin_miller_sf(n, round);
        //  if(ans) gmp_printf("Find prime: %Zd\n", n);
        mpz_add_ui(n, n, 1);
    }

    diff = clock() - t_start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    //  fprintf(logfd, "%d, ", msec);
    printf("%lu, %d\n", exp, msec);



    t_start = clock();
    mpz_t n2;
    mpz_init_set_ui(n2, 2);
    mpz_pow_ui(n2, n2, exp);
    init_local(round);
    for(int i=0; i<range; i++){
        int ans = bignum_rabin_miller_para(n2, round);
        //  if(ans) gmp_printf("Find prime: %Zd\n", n);
        mpz_add_ui(n2, n2, 1);
    }
    clear_local(round);
    diff = clock() - t_start;

    msec = diff * 1000 / CLOCKS_PER_SEC;
    //  fprintf(logfd, "%d, ", msec);
    printf("%lu, %d\n", exp, msec);

    mpz_clear(n);
    mpz_clear(n2);
}

int main()
{
    for(unsigned long i = 4; i<=4096; i*=2)
        testForRange(i);
    //testFromInput();
    //  New gmp version:
    
    //  The original one: deprecated...
    //  print_single_input();
    //  print_prime_under_1000_for_all();
    return 0;
}