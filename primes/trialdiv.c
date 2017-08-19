#include "trialdiv.h"

void trialdiv(char *number, FILE *pfp)
{
	// chech if number is a valid decimal string
	char *p = number;
	char ch;
	while (*p != '\0') {
		ch = *p++;
		if (ch < '0' || ch > '9') {
			fprintf(stderr, "trialdiv: \"%s\" is a invalid number.\n", number);
			exit(EXIT_FAILURE);
		}
	}

	// create BIGNUM for number	
	BIGNUM *bn_n = NULL;
	bn_n = BN_new();
	if (bn_n == NULL) {
		fprintf(stderr, "trialdiv: Create bn_n failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_n);
	
	if (BN_dec2bn(&bn_n, number) == 0) {
		fprintf(stderr, "trialdiv: Initialize bn_n failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// create an array to store the content of the primesfile
	// create input buffer
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

	// use trial division to test the primality
	trialDivision(bn_n, primes, fileSize, 0);

	// clean up
	BN_free(bn_n);
}

int trialDivision(BIGNUM *bn_n, unsigned int *primes, int arrLen, int numInd)
{
	int ret = -1; // return value
	
	// create BIGNUM for the prime number
	BIGNUM *bn_a = NULL;
	bn_a = BN_new();
	if (bn_a == NULL) {
		fprintf(stderr, "trialDivision: Create bn_a failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_a);

	// create BIGNUM for the prime square
	BIGNUM *bn_a_squared = NULL;
	bn_a_squared = BN_new();
	if (bn_a_squared == NULL) {
		fprintf(stderr, "trialDivision: Create bn_a_squared failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_a_squared);
	
	// create BIGNUM for the remainder
	BIGNUM *bn_rem = NULL;
	bn_rem = BN_new();
	if (bn_rem == NULL) {
		fprintf(stderr, "trialDivision: Create bn_rem failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_rem);

	// create BIGNUM for temperary variable
	BN_CTX *bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if (bn_ctx == NULL) {
		fprintf(stderr, "trialDivision: Create bn_ctx failed.\n");
		exit(EXIT_FAILURE);
	}

	// trial division algorithm
	int i;
	for (i = 1; i < arrLen; i++) {
		// initialize a prime to bn_a
		if (BN_bin2bn((unsigned char *)(&primes[i]), sizeof(unsigned int), bn_a) == NULL) {
			fprintf(stderr, "trialDivision: Initialize bn_a failed.\n");
			exit(EXIT_FAILURE);
		}
		
		// calculate the square of the prime
		if (!BN_sqr(bn_a_squared, bn_a, bn_ctx)) {
			fprintf(stderr, "trialDivision: Calculate bn_a_squared failed.\n");
			exit(EXIT_FAILURE);
		}
		
		// calculate rem = n mod a
		if (!BN_mod(bn_rem, bn_n, bn_a, bn_ctx)) {
			fprintf(stderr, "trialDivision: Calculate bn_rem failed.\n");
			exit(EXIT_FAILURE);
		}
		
		// if rem == 0 => "fail" the trial division test
		if (BN_is_zero(bn_rem)) { // find a divisor
			for (i = 0; i < numInd; i++)
				fprintf(stdout, " ");
			fprintf(stdout, "n is composite by trial division (mod %s = 0)\n", BN_bn2dec(bn_a));
			ret = 0;
			break;
		}
		
		// "pass" the trial division test
		if (BN_cmp(bn_a_squared, bn_n) == 1) { // prime > floor(sqrt(number)), end the test
			for (i = 0; i < numInd; i++)
				fprintf(stdout, " ");
			fprintf(stdout, "n passes trial division test\n");
			ret = 1;
			break;
		}
	}
	
	// "error" case: not enough primes to test
	if (ret == -1) {
		for (i = 0; i < numInd; i++)
			fprintf(stdout, " ");
		fprintf(stdout, "n passes trial division test (not enough primes)\n");
	}
	
	// clean up
	BN_free(bn_a);
	BN_free(bn_a_squared);
	BN_free(bn_rem);
	BN_CTX_free(bn_ctx);
	
	return ret;
}


