#include <stdio.h>
#include <gmp.h>

int main (void)
{
    mpz_t x;
    
    /* init */
    mpz_init(x);

    /* factorial */
    mpz_fac_ui(x, 640);

    /* output */
    printf("640! in base 32 is: ");
    mpz_out_str(stdout, 32, x);
    printf("\n");

    /* free */
    mpz_clear(x);
    return(0);
}

/* COMPILE ME -->
need to include the shared object file!
gcc adding.c /usr/lib/libgmp.so.3.1.1 -o adding
*/
