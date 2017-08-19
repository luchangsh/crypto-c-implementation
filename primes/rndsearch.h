#ifndef RNDSEARCH_H
#define RNDSEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <openssl/bn.h>

#include "trialdiv.h"
#include "millerrabin.h"

/*
 * Generate a numbits-bit probable prime using Random-Search
 * @Param numbits: number of bits of the prime
 * @Param maxitr: maximum iteration
 * @Param pfp: primes file pointer
 * @Param rfp: random file pointer
 */
void rndsearch(int numbits, int maxitr, FILE *pfp, FILE *rfp);

/*
 * Generate a random odd number
 * @Param bn_n: BIGNUM object of the generated number
 * @Param k: number of bits of the generated number
 * @Param rbuf: buffer for a random number read from the random file
 * @Param rbuf_size: the size of rbuf in bytes
 * @Param rfp: random file pointer
 */
void RndOddNum(BIGNUM *bn_n, int k, unsigned char *rbuf, int rbuf_size, FILE *rfp);

#endif
