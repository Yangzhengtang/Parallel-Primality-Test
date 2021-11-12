#include "sequential.h"
#include "parallel.h"

#define ARRAY_MAX 512
#define STR_MAX 128
typedef int (*mr_func)(unsigned int x, unsigned int y);

#define IMPL_COUNT 5 //  Total functions implemented
static mr_func mr_func_table[IMPL_COUNT] = {
    rabin_miller_sf,
    rabin_miller_shitty,
    rabin_miller_shitty_parallel,
    rabin_miller_v1_parallel,
    rm_shitty_parallel_pthread};

static char mr_func_name_table[ARRAY_MAX][STR_MAX] = {
    "rabin_miller_sf",
    "rabin_miller_shitty",
    "rabin_miller_shitty_parallel",
    "rabin_miller_v1_parallel",
    "rm_shitty_parallel_pthread"};

//  Make sure srand can only be called once
static char rand_initialized = 0;
static inline void my_srand()
{
    if (rand_initialized == 0)
        return;
    omp_set_dynamic(0);
    omp_set_num_threads(PTHREAD);
    srand(time(NULL));
    rand_initialized = 1;
    return;
}

/*
    Test for the implementation:
        print_primality_single_func
        print_single_input
        print_prime_in_range
        print_prime_under_100_for_all
*/
void print_primality_single_func(mr_func func, unsigned int test_num, unsigned int test_round)
{
    my_srand();

    int result = func(test_num, test_round);
    
    printf("Checking %d, got [%s]\n", test_num, result == 1 ? "prime" : "composite");
    //  int default_result = rabin_miller_sf(test_num, test_round);
    //  printf("Checking %d, got %s, default %s\n",
    //         test_num, result == 1 ? "prime" : "composite", default_result == 1 ? "prime" : "composite");
}

//  Single test, check the primality of a number given by user, by default check 40 rounds
void print_single_input()
{
    my_srand();

    unsigned testing_round = 40;
    unsigned test_num;
    printf("Please enter the number to check:\n");
    scanf("%d", &test_num);
    print_primality_single_func(rm_shitty_parallel_pthread, test_num, testing_round);
}

void print_prime_in_range(int start, int end, int round, int f_index)
{
    my_srand();

    mr_func func = mr_func_table[f_index];
    for (unsigned int n = start; n < end; n++)
    {
        if (func(n, round))
            printf("\t\t%d\n", n);
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

int main()
{
    print_single_input();
    return 0;
}