#ifndef MILLERRABIN_H
#define MILLERRABIN_H

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <openssl/bn.h>

/*
 * Test the primality of number using Miller-Rabin
 * Check if number is a valid decimal string and create a BIGNUM object for number
 * Read all primes in the primes file into an array
 * Call millerRabin() to perform Miller-Rabin test
 * @Param number: decimal string representation of a number
 * @Param pfp: a file pointer of a primes file
 */
void millerrabin(char *number, int maxitr, FILE *pfp);

/*
 * Test the primality of number using Miller-Rabin
 * @Param bn_n: BIGNUM object for number to be tested
 * @Param primes: an array of primes given by the primes file
 * @Param arrLen: the number of primes
 * @Param numInd: the number of indented spaces before the print message
 */
int millerRabin(BIGNUM *bn_n, int maxitr, unsigned int *primes, int arrLen, int numInd);

#endif
