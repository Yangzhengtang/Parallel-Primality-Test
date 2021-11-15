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

void testForRange(unsigned long exp, int round) {
    //  bignum_init();
    mpz_t n;
    mpz_init_set_ui(n, 2);
    mpz_pow_ui(n, n, exp);

    //  int range = 0x1000;
    int range = 50;

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
    //printf("%lu, %d\n", exp, msec);



    t_start = clock();
    mpz_t n2;
    mpz_init_set_ui(n2, 2);
    mpz_pow_ui(n2, n2, exp);
    for(int i=0; i<range; i++){
        int ans = bignum_rabin_miller_para(n2, round);
        //  if(ans) gmp_printf("Find prime: %Zd\n", n);
        mpz_add_ui(n2, n2, 1);
    }
    diff = clock() - t_start;

    int msec2 = diff * 1000 / CLOCKS_PER_SEC;
    //  fprintf(logfd, "%d, ", msec);
    //  printf("%lu, %d\n", exp, msec);
    printf("For %lu, round %d, got: %f\n", exp, round, ((double)(msec2-msec))/msec);

    mpz_clear(n);
    mpz_clear(n2);
}



static unsigned int prime_list[200] = {
2	, 3	, 5	,7,	    11,	    13	,17	,19	,23	,29,
31	,37	,41	,43,	47,	    53	,59	,61	,67	,71,
73	,79	,83	,89,	97,	    101,	103,	107,	109,	113,            //  25
127	,131,	137,	139,	149,	151,	157,	163,	167	,173,
179	,181,	191,	193,	197,	199,	211,	223,	227	,229,
233	,239,	241,	251,	257,	263,	269,	271,	277	,281,
283	,293,	307,	311,	313,	317,	331,	337,	347	,349,
353	,359,	367,	373,	379,	383,	389,	397,	401	,409,
419	,421,	431,	433,	439,	443,	449,	457,	461	,463,
467	,479,	487,	491,	499,	503,	509,	521,	523	,541,
547	,557,	563,	569,	571,	577,	587,	593,	599	,601,
607	,613,	617,	619,	631,	641,	643,	647,	653	,659,
661	,673,	677,	683,	691,	701,	709,	719,	727	,733,
739	,743,	751,	757,	761,	769,	773,	787,	797	,809,
811	,821,	823,	827,	829,	839,	853,	857,	859	,863,
877	,881,	883,	887,	907,	911,	919,	929,	937	,941,
947	,953,	967,	971,	977,	983,	991,	997,	1009,	1013,   //  
1019,	1021,	1031,	1033,	1039,	1049,	1051,	1061,	1063,	1069,
1087,	1091,	1093,	1097,	1103,	1109,	1117,	1123,	1129,	1151,
1153,	1163,	1171,	1181,	1187,	1193,	1201,	1213,	1217,	1223
};

static unsigned int composite_list[1300] = {0};


void testForPrime_Compo(unsigned long exp) {
    //  bignum_init();
    mpz_t n;
    mpz_init_set_ui(n, 2);
    mpz_pow_ui(n, n, exp);

    int round = 128;
    int range = 0x1000;

    clock_t t_start, diff;
    t_start = clock();
    for(int j=0; j<100; j++){
        for(int i=0; i<200; i++){
            //  mpz_set_ui(n, prime_list[i]);
            mpz_set_ui(n, 7919);
            int ans = bignum_rabin_miller_para(n, round);
        }
    }
    diff = clock() - t_start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    //  fprintf(logfd, "%d, ", msec);
    printf("%lu, %d\n", exp, msec);

//  Get composite list
unsigned a = 4;
int count = 0;
/* while(a < 1000){
    if(rabin_miller_sf(a, 200) == 0){
        composite_list[count] = a;
        count += 1;
    }
    a += 1;
} */
printf("Got composite %d.\n", count);
    t_start = clock();
    mpz_t n2;
    mpz_init_set_ui(n2, 2);
    mpz_pow_ui(n2, n2, exp);
    for(int j=0; j<100; j++){
        for(int i=0; i<200; i++){
            //  mpz_set_ui(n2, composite_list[i]);
            mpz_set_ui(n2, 7909);
            int ans = bignum_rabin_miller_para(n2, round);
        }
    }
    diff = clock() - t_start;

    msec = diff * 1000 / CLOCKS_PER_SEC;
    //  fprintf(logfd, "%d, ", msec);
    printf("%lu, %d\n", exp, msec);

    mpz_clear(n);
    mpz_clear(n2);
}


int last_first = 0;
int last_total = 0;
void testProb(unsigned long exp) {
    //  bignum_init();
    mpz_t n;
    mpz_init_set_ui(n, 2);
    mpz_pow_ui(n, n, exp);

    int round = 16;
    int range = 0x1000;

    for(int i=0; i<range; i++){
        int ans = bignum_rabin_miller_helper(n, round);
        //  if(ans) gmp_printf("Find prime: %Zd\n", n);
        mpz_add_ui(n, n, 1);
    }

    int all_sum = 0;
    int first = breakcount[0] - last_first;
    for(int i=0; i<128; i++){
        all_sum += breakcount[i];
    }
    all_sum = all_sum - last_total;
    last_total += all_sum;
    last_first += first;
    printf("Break on %d : %d, total break: %d, percent: %f\n", 0, first, all_sum, ((double)(first))/all_sum);
    
    for(int i=0; i<200; i++){
        printf("Prime break on %d : %d\n", i, bk_prime[i]);
    }
    
    mpz_clear(n);
}




void newTestForRange(unsigned long exp) {
    //  bignum_init();
    mpz_t n;
    mpz_init_set_ui(n, 10);
    mpz_pow_ui(n, n, exp);

    int round = 16;
    //  int range = 0x1000;
    int range = 1000;

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

    mpz_clear(n);
}




int main()
{
    //  testForPrime_Compo(32);
    
    for(unsigned long i = 1; i<=0xFFFFF; i*=2)
        //  newTestForRange(i); */
        {
            for(int j=1; j<=0x1000; j*=2)
                testForRange(i,j);
        }
    //testFromInput();
    //  New gmp version:
    
    /* for(unsigned long i = 4; i<=4096; i*=2){
        printf("***** Checking %lu *****\n", i);
        testProb(i);
    } */
    //  The original one: deprecated...
    //  print_single_input();
    //  print_prime_under_1000_for_all();
    return 0;
}