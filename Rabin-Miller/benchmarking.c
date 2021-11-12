#include "test_utils.h"

void testbench_single(unsigned int start, unsigned int end, unsigned int round, int f_index)
{
    my_srand();

    FILE *logfd;
    logfd = fopen("benchmarking.txt", "a+");

    printf("\t********** Now benchmarking **********\n");
    printf("\t* checking all numbers from %d to %d\n", start, end);
    printf("\t* each number will be tested for %d round\n", round);
    printf("\t* For parallel version, we are using %d threads.\n", PTHREAD);

    mr_func func_to_test = mr_func_table[f_index];

    clock_t t_start = clock(), diff;
    for (int i = start; i < end; i++)
    {
        func_to_test(i, round);
    }
    diff = clock() - t_start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    fprintf(logfd, "%35s: Time taken %d seconds %d milliseconds\n", mr_func_name_table[f_index], msec / 1000, msec % 1000);
    printf("%35s: Time taken %d seconds %d milliseconds\n", mr_func_name_table[f_index], msec / 1000, msec % 1000);

    close(logfd);
}

void testbench_all(unsigned int start, unsigned int end, unsigned int round)
{
    for (int i = 0; i < IMPL_COUNT; i++)
    {
        testbench_single(start, end, round, i);
    }
}

int main()
{
    
    unsigned start  = 4;
    unsigned end    = 8;
    unsigned round  = 1000000;

    for(int i=0; i<30; i++){
        testbench_single(start, end, round, 0);
        testbench_single(start, end, round, 3);
        testbench_single(start, end, round, 4);
        end = end * 2;
        start = start * 2;
    }


    //  testbench_all(500, 500000, 100);
    return 0;
}