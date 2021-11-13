#include "sequential.h"
#include "parallel.h"
#include "parallel_pthread.h"

#define ARRAY_MAX 512
#define STR_MAX 128

typedef int (*mr_func)(rm_int x, rm_int y);

#define IMPL_COUNT 9 //  Total functions implemented
static mr_func mr_func_table[IMPL_COUNT] = {
    rabin_miller_sf,                       //   0
    rabin_miller_shitty,                   //   1
    rabin_miller_shitty_parallel,          //   2
    rabin_miller_v1_parallel,              //   3
    rabin_miller_v1_parallel_opt,          //   4
    rabin_miller_v2_parallel,              //   5
    rabin_miller_v3_parallel_opt,              //   6
    rabin_miller_pthread_parallel,         //   7
    rabin_miller_pthread_parallel_opt};    //   8

static char mr_func_name_table[ARRAY_MAX][STR_MAX] = {
    "rabin_miller_sf",
    "rabin_miller_shitty",
    "rabin_miller_shitty_parallel",
    "rabin_miller_v1_parallel",
    "rabin_miller_v1_parallel_opt",
    "rabin_miller_v2_parallel",
    "rabin_miller_v3_parallel_opt",
    "rabin_miller_pthread_parallel",
    "rabin_miller_pthread_parallel_opt"};

//  Make sure srand can only be called once
static char rand_initialized = 0;
static inline void my_srand()
{
    if (rand_initialized == 0)
        return;
    //  omp_set_dynamic(0);
    omp_set_num_threads(PTHREAD);
    srand(time(NULL));
    rand_initialized = 1;
    return;
}
