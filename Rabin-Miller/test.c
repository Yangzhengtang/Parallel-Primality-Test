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
    print_primality_single_func(rabin_miller_v2_parallel, test_num, testing_round);
}

void print_prime_in_range(rm_int start, rm_int end, rm_int round, int f_index)
{
    my_srand();

    mr_func func = mr_func_table[f_index];
    for (rm_int n = start; n < end; n++)
    {
        if (func(n, round))
            printf("\t\t_rm_pformat_\n", n);
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
    //  print_prime_under_100_for_all();
    return 0;
}