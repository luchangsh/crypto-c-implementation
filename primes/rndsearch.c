#include "rndsearch.h"

void rndsearch(int numbits, int maxitr, FILE *pfp, FILE *rfp)
{
	// create an array to store the content of the primesfile
	// create input buffer for primesfile
	unsigned int buffer[1];
	buffer[0] = 0;
	
	// count primesfile size (unit: unsinged int) to determine the size of the array
	int fileSize = 0;
	size_t bytes = 0;
	while ((bytes = fread(buffer, sizeof(unsigned int), 1, pfp)) > 0) fileSize++;
	fseek(pfp, 0L, SEEK_SET); // move file pointer back to the begining of the file
	
	// create the array
	unsigned int primes[fileSize];
	int i;
	for (i = 0; i < fileSize; i++) primes[i] = 0;
	
	// populate the array
	i = 0;
	while ((bytes = fread(buffer, sizeof(unsigned int), 1, pfp)) > 0) primes[i++] = buffer[0];

	
	// create a input buffer for rfp
	int rbuf_size = (numbits % 8 == 0) ? numbits / 8 : numbits / 8 + 1;
	unsigned char rbuf[rbuf_size];
	for (i = 0; i < rbuf_size; i++) rbuf[i] = 0;

	// create BIGNUM for the random number
	BIGNUM *bn_n = NULL;
	bn_n = BN_new();
	if (bn_n == NULL) {
		fprintf(stderr, "rndsearch: Create bn_n failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_n);
	
	// random search algorithm
	i = 1;
	while (1) {
		fprintf(stdout, "RANDOM-SEARCH: iteration %d\n", i++);
		
		// generate a random odd number with numbits bits
		RndOddNum(bn_n, numbits, rbuf, rbuf_size, rfp);
		fprintf(stdout, "  n = %s\n", BN_bn2dec(bn_n));
		
		// Trial Division test
		if (trialDivision(bn_n, primes, fileSize, 2) == 0) continue; // failed, return to step 1: RndOddNum()
		
		// Miller-Rabin test
		if (millerRabin(bn_n, maxitr, primes, fileSize, 4) == 1) break; // find a prime
	}

	free(bn_n);
}

void RndOddNum(BIGNUM *bn_n, int k, unsigned char *rbuf, int rbuf_size, FILE *rfp)
{
	// clear rbuf
	int i;
	for (i = 0; i < rbuf_size; i++) rbuf[i] = 0;
	
	// read rbuf_size bytes from rfp into rbuf
	size_t bytes = 0;
	if ((bytes = fread(rbuf, sizeof(unsigned char), rbuf_size, rfp)) < rbuf_size) {
		fprintf(stderr, "RndOddNum: run out of data in the rndfile.\n");
		exit(EXIT_FAILURE);
	}
		
	// initialize bn_n
	if (BN_bin2bn(rbuf, rbuf_size, bn_n) == NULL) {
		fprintf(stderr, "RndOddNum: Initialize bn_n failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// set bit 0 to one
	if (BN_set_bit(bn_n, 0) == 0) {
		fprintf(stderr, "RndOddNum: Set bit 0 of bn_n to one failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// set bit k-1 to one
	if (BN_set_bit(bn_n, k - 1) == 0) {
		fprintf(stderr, "RndOddNum: Set bit k-1 of bn_n to one failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// set bit k and bits above k to zero
	for (i = k; i < rbuf_size * 8; i++) {
		if (BN_clear_bit(bn_n, i) == 0) {
			fprintf(stderr, "RndOddNum: Clear bit k and above of bn_n failed.\n");
			exit(EXIT_FAILURE);
		}
	}
}
