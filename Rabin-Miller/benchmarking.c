#include "test_utils.h"

void testbench_single(rm_int start, rm_int end, rm_int round, int f_index)
{
    my_srand();

    FILE *logfd;
    logfd = fopen("benchmarking.txt", "a+");

    printf("\n\t********** Now benchmarking **********\n");
    printf("\t* checking all numbers from " _rm_pformat_ "to " _rm_pformat_ "\n", start, end);
    printf("\t* each number will be tested for " _rm_pformat_ " round\n", round);
    printf("\t* For parallel version, we are using " _rm_pformat_ " threads.\n", PTHREAD);

    mr_func func_to_test = mr_func_table[f_index];

    clock_t t_start = clock(), diff;
    for (int i = start; i < end; i++)
    {
        func_to_test(i, round);
    }
    diff = clock() - t_start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    fprintf(logfd, "%35s: Time taken " _rm_pformat_ " seconds " _rm_pformat_ " milliseconds\n", mr_func_name_table[f_index], msec / 1000, msec % 1000);
    printf("\t%s: Time taken " _rm_pformat_ " seconds " _rm_pformat_ " milliseconds\n", mr_func_name_table[f_index], msec / 1000, msec % 1000);

    close(logfd);
}

void testbench_all(rm_int start, rm_int end, rm_int round)
{
    for (int i = 0; i < IMPL_COUNT; i++)
    {
        testbench_single(start, end, round, i);
    }
}

int main()
{
    
    rm_int start  = 2;
    rm_int end    = 4;
    rm_int round  = 1000;

    for(int i=0; i<62; i++){
        //  testbench_single(start, end, round, 0);
        //  testbench_single(start, end, round, 3);
        testbench_single(start, end, round, 0);
        end = end * 2;
        start = start * 2;
    }


    //  testbench_all(500, 500000, 100);
    return 0;
}