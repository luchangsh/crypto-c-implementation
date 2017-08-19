#ifndef TRIALDIV_H
#define TRIALDIV_H

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <openssl/bn.h>

/*
 * Test the primality of number using trial division
 * Check if number is a valid decimal string and create a BIGNUM object for number
 * Read all primes in the primes file into an array
 * Call trialDivition() to perform trial division test
 * @Param number: decimal string representation of a number
 * @Param pfp: a file pointer of a primes file
 */
void trialdiv(char *number, FILE *pfp);

/*
 * Test the primality of number using trial division
 * @Param bn_n: BIGNUM object for number to be tested
 * @Param primes: an array of primes given by the primes file
 * @Param arrLen: the number of primes
 * @Param numInd: the number of indented spaces before the print message
 */
int trialDivision(BIGNUM *bn_n, unsigned int *primes, int arrLen, int numInd);

#endif
