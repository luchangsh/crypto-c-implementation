#ifndef MAURER_H
#define MAURER_H

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <openssl/bn.h>
#include <math.h>

#include "trialdiv.h"
#include "millerrabin.h"
#include "rndsearch.h"

#define round(X) (((X) >= 0) ? (int)((X)+0.5) : (int)((X)-0.5))

/*
 * Generate a numbits-bit provable prime using Maurer algorithm
 * Read all primes in the primes file into an array
 * Call maurerRecursive() to perform Maurer algorithm
 * @Param numbits: number of bits of the prime
 * @Param pfp: primesfile pointer
 * @Param rfp: rndfile pointer
 */
void maurer(int numbits, FILE *pfp, FILE *rfp);

/*
 * Generate a numbits-bit provable prime using Maurer algorithm
 * @Param k: number of bits of the generated number 
 * @Param bn_n: BIGNUM object of the generated number
 * @Param primes: an array of primes given by the primes file
 * @Param arrLen: the number of primes
 * @Param rfp: random file pointer
 * @Param level: level of recursion started from 0
 */
void maurerRecursive(int k, BIGNUM *bn_n, unsigned int *primes, int arrLen, FILE *rfp, int level);

/*
 * Read one byte from rndfile
 * @Param rfp: rndfile pointer
 */
unsigned char rndByte(FILE *rfp);

#endif
