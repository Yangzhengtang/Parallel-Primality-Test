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
    unsigned round  = 100;

    for(int i=0; i<30; i++){
        testbench_single(start, end, round, 0);
        end = end * 2;
        start = start * 2;
    }

    //  testbench_all(500, 5000000, 100);
    return 0;
}