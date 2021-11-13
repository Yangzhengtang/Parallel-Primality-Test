#include "test_utils.h"
#include <unistd.h>

void testbench_single(rm_int start, rm_int end, rm_int round, int f_index)
{
    my_srand();

    FILE *logfd;
    logfd = fopen("benchmarking.txt", "a+");
    
    fprintf(logfd, "\t* " _rm_pformat_ " to " _rm_pformat_ " for " _rm_pformat_ " round " , start, end, round);
    printf("\t* " _rm_pformat_ " to " _rm_pformat_ " for " _rm_pformat_ " round " , start, end, round);

    mr_func func_to_test = mr_func_table[f_index];

    clock_t t_start = clock(), diff;
    for (rm_int i = start; i < end; i++)
    {
        //  printf("Testing %llu\n", i);
        func_to_test(i, round);
    }
    diff = clock() - t_start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    fprintf(logfd, "%35s: Time taken %d seconds %d milliseconds\n", mr_func_name_table[f_index], msec / 1000, msec % 1000);
    printf("\t%s: Time taken %d seconds %d milliseconds\n", mr_func_name_table[f_index], msec / 1000, msec % 1000);

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
    printf("For parallel version, we are using %d threads.\n", PTHREAD);

    rm_int start  = 2;
    rm_int end    = 4;
    

    //  sleep(60);
    for(int i=0; i<64; i++){
        //  testbench_single(start, end, round, 3);
        //  testbench_single(start, end, round, 4);
        //  testbench_single(start, end, round, 6);
        //  testbench_single(start, end, round, 7);
        //  testbench_single(start, end, round, 8);
        end = start + (unsigned long long)(0x100);
        for(rm_int round=2; round < (rm_int)(0XFFFF); round<<=1){
            //  testbench_all(start, end, round);
            testbench_single(start, end, round, 0);
            testbench_single(start, end, round, 6);
        }
        start = start << 1;
    }
    
    //  testbench_all(500, 500000, 100);
    return 0;
}