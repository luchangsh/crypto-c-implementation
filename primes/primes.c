#include "primes.h"

void primes(int maxval)
{
	// maxval should be in the range of 2 to 2^24
	if (maxval < 2 || maxval > (1 << 24)) {
		fprintf(stderr, "primes: maxval (%d) is out of range (2 <= maxval <= 2^24).\n", maxval);
		exit(EXIT_FAILURE);
	}
	
	// declare an array for Sieve of Eratosthenes algorithm
	int aLen = (maxval % 8 == 0) ?  maxval / 8 : maxval / 8 + 1;
	unsigned char a[aLen];
	int i;
	for (i = 0; i < aLen; i++) a[i] = 0; // initialize all bits to 0
	
	// Sieve of Eratosthenes
	eratosthenes(maxval, a, aLen);
	
	// count how many primes in the range of 2 to maxval
	int j, cnt = 0;
	for (i = 0; i < aLen; i++) {
		for (j = 0; j <= 7; j++) {
			if ((a[i] >> j) & 1) cnt++;
		}
	}
	
	// declare an output buffer
	int outBufLen = cnt + 1; // length of the output buffer is cnt + 1 (maxval + primes)
	unsigned int outBuf[outBufLen];
	for (i = 0; i < outBufLen; i++) outBuf[i] = 0; // initialize to 0
	
	// the first integer is maxval, big-endian
	outBuf[0] = htonl((unsigned int) maxval);
	
	// followed by primes in increasing order with big-endian format
	int k = 1;
	for (i = 0; i < aLen; i++) {
		for (j = 0; j <= 7; j++) {
			if ((a[i] >> j) & 1) outBuf[k++] = htonl((unsigned int) (i * 8 + j + 1));
		}
	}
	
	// print out
	fwrite(outBuf, sizeof(unsigned int), outBufLen, stdout);
}

void eratosthenes(int n, unsigned char *a, int aLen)
{
	// bit 0 -> 1, bit 1 -> 2, bit n-1 -> n
	// set bits in the array, 0 means it's not a prime, 1 means we don't know if it's a prime or not yet
	// for all numbers > n, set bits to 0
	int i, index, shift;
	for (i = 2; i <= n; i++) {
		index = (i % 8 == 0) ? i / 8 - 1 : i / 8;
		shift = (i % 8 == 0) ? 7 : i % 8 - 1;
		a[index] |= (1 << shift);
	}
	
	int p = 2, j;
	while (p <= n / p) {
		j = p * p; // start from p^2
		while (j <= n) {
			index = (j % 8 == 0) ? j / 8 - 1 : j / 8;
			shift = (j % 8 == 0) ? 7 : j % 8 - 1;
			a[index] &= ~(1 << shift); // set all multiple of prime as composite
			j += p;
		}
		p++;
		while (getBit(a, p) == 0) p++; // find the next prime
	}
}

unsigned char getBit(unsigned char *a, int p)
{
	int index = (p % 8 == 0) ? p / 8 - 1 : p / 8;
	int shift = (p % 8 == 0) ? 7 : p % 8 - 1;
	return (a[index] >> shift) & 1;
}


