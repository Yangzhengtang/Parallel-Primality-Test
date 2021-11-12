#include "rm_sequential.h"
#include "rm_parallel.h"

#define ARRAY_MAX 512
#define STR_MAX 128
typedef int (*mr_func) (unsigned int x, unsigned int y);


//  Make sure srand can only be called once
static char rand_initialized = 0;
static void my_srand(){
    if (rand_initialized == 0)
        return;
    srand(time(NULL));
    rand_initialized = 1;
    return;
}

//  Single test, check the primality of a number given by user, by default check 40 rounds
void test_single_input(){
    unsigned testing_round = 40;
    unsigned test_num;
    printf("Please enter the number to check:\n");
    scanf("%d", &test_num);
    print_primality_single_func(rm_shitty_parallel_pthread, test_num, testing_round);
}

void print_primality_single_func(mr_func func, unsigned int test_num, unsigned int test_round){
    my_srand();

    int result = func(test_num, test_round);
    int default_result = rabin_miller_sf(test_num, test_round);
    printf("Checking %d, got %s, default %s\n", 
        test_num, result==1 ? "prime" : "composite", default_result==1 ? "prime" : "composite");
}

int testCorrect(){
    srand(time(NULL));
    int (*func_ptrs[ARRAY_MAX]) (unsigned int x, unsigned int y) = {
        rabin_miller_sf,
        rabin_miller_shitty,
        rabin_miller_shitty_parallel,
        rabin_miller_v1_parallel,
        rm_shitty_parallel_pthread
    };

    for(int i=3; i<5; i++){
        int  (*MRfunc) (unsigned int x, unsigned int y) = func_ptrs[i];
        for(unsigned n=3; n<100; n++){
            if(MRfunc(n, 40))
                printf("%d\n", n);
        }
        printf("------------------------\n");
    }
}

int testBench(){
    my_srand();

    unsigned int test_start = 1000;
    unsigned int test_end   = 50000;
    unsigned int testing_round = 1000;

    printf("Now benchmarking...\nTesting from %d to %d, each number will be tested for %d round\n",
    test_start, test_end, testing_round);

    omp_set_dynamic(0);
    omp_set_num_threads(PTHREAD);

    printf("-- For parallel version, we are using %d threads.\n", PTHREAD);

    //  Array of function pointers
    int (*func_ptrs[ARRAY_MAX]) (unsigned int x, unsigned int y) = {
        rabin_miller_sf,
        //rabin_miller_shitty,
        rabin_miller_shitty_parallel,
        //rabin_miller_v1_parallel,
        rm_shitty_parallel_pthread
    };

    char func_names[ARRAY_MAX][STR_MAX] = {
        "rabin_miller_sf",
        //"rabin_miller_shitty",
        "rabin_miller_shitty_parallel",
        //"rabin_miller_v1_parallel",
        "rm_shitty_parallel_pthread"
    };
    int func_len = 3;


    for(int i=0; i<func_len; i++){
        clock_t start = clock(), diff;
        
        for(int j=test_start; j<test_end; j++){
            (*func_ptrs[i])(j, testing_round);
        }

        diff = clock() - start;
        int msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("%35s: Time taken %d seconds %d milliseconds\n", func_names[i], msec/1000, msec%1000);
    }

    return 0;
}

int main(){
    //  test_single_input();
    //  singleTest();
    //  testCorrect();
    testCorrect();
    //  testBench();
    return 0;

}