#ifndef PRIMES_H
#define PRIMES_H

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

/* Generate all prime numbers less than or equal to maxval */
void primes(int maxval);

/*
 * Generate all prime numbers less than or equal to n using Sieve of Eratosthenes
 * @Param n: maximum value of generated primes
 * @Param a: a buffer where each bit represents the primality of the corresponded number
 * @Param aLen: the length of the buffer a in bytes
 */
void eratosthenes(int n, unsigned char *a, int aLen);

/*
 * Get the bit at the position p in the array a
 */
unsigned char getBit(unsigned char *a, int p);

#endif
