#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include "sieve.h"
#include "poly.h"

// return 0 for composite, 1 for prime
int aks(mpz_class n) {
    // Check if n is a perfect power: if n = a^b for integers a > 1 and b > 1, output composite.
    if(mpz_perfect_power_p(n.get_mpz_t())) {
        return 0;
    }

    sieve s;

    mpz_class r = 2;
    mpz_class logn = mpz_sizeinbase(n.get_mpz_t(),2);
    mpz_class limit = logn * logn;
    limit *= 4;

    /* Witness search */
    // search for r such that r is coprime to n
    while(r < n) {
        // n can be divided by r
        if(mpz_divisible_p(n.get_mpz_t(), r.get_mpz_t())) {
            return 0;
        }
        int failed = 0;
        if(s.isPrime(r)) {
            // r is prime
            // i is the multiplicative order of n modulo r
            for(mpz_class i = 1; i <= limit; i++) {
                mpz_class res = 0;
                // res = n^i mod r
                mpz_powm(res.get_mpz_t(), n.get_mpz_t(), i.get_mpz_t(), r.get_mpz_t());
                if(res == 1) {
                    failed = 1;
                    break;
                }
            }
            if(!failed)
                break;
        }
        r++;
    }

    if (r == n) {
        return 1;
    }

    /* Polynomial check */
    unsigned int a;
    mpz_class sqrtr;

    //actually the floor, add one later to get the ceil
    mpz_sqrt(sqrtr.get_mpz_t(), r.get_mpz_t());
    mpz_class polylimit = 2 * (sqrtr+1) * logn;

    unsigned int intr = mpz_get_ui(r.get_mpz_t());

    for(a = 1; a <= polylimit; a++) {
        mpz_class final_size = n % r;
        mpz_pX compare(mpz_get_ui(final_size.get_mpz_t()));
        compare.setCoef(1, mpz_get_ui(final_size.get_mpz_t()));
        compare.setCoef(a, 0);
        mpz_pX res(intr);
        mpz_pX base(1);
        base.setCoef(a,0);
        base.setCoef(1,1);

        mpz_pX_mod_power(res, base, n, n, intr);
        if(!res.isEqual(compare)) {
            return 0;
        }
    }
    return 1;
}


int main(int argc, char *argv[]) {
  mpz_class n = strtol(argv[1], nullptr, 0);
  cout << aks(n) << endl;
}
