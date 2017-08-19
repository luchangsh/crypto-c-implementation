#include "millerrabin.h"

void millerrabin(char *number, int maxitr, FILE *pfp)
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
		fprintf(stderr, "millerrabin: Create bn_n failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_n);
	
	if (BN_dec2bn(&bn_n, number) == 0) {
		fprintf(stderr, "millerrabin: Initialize bn_n failed.\n");
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

	// print n
	fprintf(stdout, "n = %s\n", BN_bn2dec(bn_n));
	
	// use Miller-Rabin to test the primality
	millerRabin(bn_n, maxitr, primes, fileSize, 2);

	// clean up
	BN_free(bn_n);
}

int millerRabin(BIGNUM *bn_n, int maxitr, unsigned int *primes, int arrLen, int numInd)
{
	int ret = 1; // return value

	// create BIGNUM for n-1
	BIGNUM *bn_n_minusOne = NULL;
	bn_n_minusOne = BN_new();
	if (bn_n_minusOne == NULL) {
		fprintf(stderr, "millerRabin: Create bn_n_minusOne failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_n_minusOne);
	// calculate n - 1
	if (!BN_sub(bn_n_minusOne, bn_n, BN_value_one())) {
		fprintf(stderr, "millerRabin: Calculate bn_n_minusOne failed.\n");
		exit(EXIT_FAILURE);
	}

	// create BIGNUM for r
	BIGNUM *bn_r = NULL;
	bn_r = BN_dup(bn_n_minusOne);
	if (bn_r == NULL) {
		fprintf(stderr, "millerRabin: Initilize bn_r to bn_n_minusOne failed.\n");
		exit(EXIT_FAILURE);
	}
	
	// calculate r and s, such that n - 1 = 2^s * r
	int s = 0;
	while (!BN_is_odd(bn_r)) {
		if (BN_rshift1(bn_r, bn_r) == 0) {
			fprintf(stderr, "millerRabin: Calculate bn_r failed.\n");
			exit(EXIT_FAILURE);
		}
		s++;
	}

	// print n-1, s and r
	int i;
	for (i = 0; i < numInd; i++)
		fprintf(stdout, " ");
	fprintf(stdout, "n-1 = %s\n", BN_bn2dec(bn_n_minusOne));
	for (i = 0; i < numInd; i++)
		fprintf(stdout, " ");
	fprintf(stdout, "s = %d\n", s);
	for (i = 0; i < numInd; i++)
		fprintf(stdout, " ");
	fprintf(stdout, "r = %s\n", BN_bn2dec(bn_r));

	// create BIGNUM for the prime number
	BIGNUM *bn_a = NULL;
	bn_a = BN_new();
	if (bn_a == NULL) {
		fprintf(stderr, "millerRabin: Create bn_a failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_a);

	// create BIGNUM for y
	BIGNUM *bn_y = NULL;
	bn_y = BN_new();
	if (bn_y == NULL) {
		fprintf(stderr, "millerRabin: Create bn_y failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_y);

	// create BIGNUM for temperary variable
	BN_CTX *bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if (bn_ctx == NULL) {
		fprintf(stderr, "millerRabin: Create bn_ctx failed.\n");
		exit(EXIT_FAILURE);
	}

	// Miller-Rabin test
	int j, k;
	for (i = 1; i <= maxitr; i++) {
		// initialize a prime to bn_a
		if (BN_bin2bn((unsigned char *)(&primes[i]), sizeof(unsigned int), bn_a) == NULL) {
			fprintf(stderr, "millerRabin: Initialize bn_a failed.\n");
			exit(EXIT_FAILURE);
		}
		
		// if a >= n-1, return "failure"
		if (BN_cmp(bn_a, bn_n_minusOne) != -1) {
			fprintf(stdout, "  Itr %d of %d, a = %s (failed)\n", i, maxitr, BN_bn2dec(bn_a));
			ret = -1;
			break;
		}
		
		// compute y = a^r mod n
		if (!BN_mod_exp(bn_y, bn_a, bn_r, bn_n, bn_ctx)) {
			fprintf(stderr, "millerRabin: Calculate bn_y failed.\n");
			exit(EXIT_FAILURE);
		}
			
		// compare y == 1 or y == n-1
		if (BN_cmp(bn_y, BN_value_one()) == 0) { // y == 1
			for (k = 0; k < numInd; k++)
				fprintf(stdout, " ");
			fprintf(stdout, "Itr %d of %d, a = %s, y = 1\n", i, maxitr, BN_bn2dec(bn_a));
		} else if (BN_cmp(bn_y, bn_n_minusOne) == 0) { // y == n-1
			for (k = 0; k < numInd; k++)
				fprintf(stdout, " ");
			fprintf(stdout, "Itr %d of %d, a = %s, y = %s (which is n-1)\n", i, maxitr, BN_bn2dec(bn_a), BN_bn2dec(bn_y));		
		} else { // y != 1 && y != n-1
			for (k = 0; k < numInd; k++)
				fprintf(stdout, " ");
			fprintf(stdout, "Itr %d of %d, a = %s, y = %s\n", i, maxitr, BN_bn2dec(bn_a), BN_bn2dec(bn_y));		
			
			for (j = 1; j <= s - 1; j++) {
				// compute y = y^2 mod n
				if (!BN_mod_sqr(bn_y, bn_y, bn_n, bn_ctx)) { 
					fprintf(stderr, "millerRabin: Calculate bn_y failed.\n");
					exit(EXIT_FAILURE);
				}				

				// print the value of j and y
				if (BN_cmp(bn_y, bn_n_minusOne) == 0) { // y == n-1
					for (k = 0; k < numInd + 2; k++)
						fprintf(stdout, " ");
					fprintf(stdout, "j = %d of %d, y = %s (which is n-1)\n", j, s - 1, BN_bn2dec(bn_y));
					break;			
				} else { // y != n-1
					for (k = 0; k < numInd + 2; k++)
						fprintf(stdout, " ");
					fprintf(stdout, "j = %d of %d, y = %s\n", j, s - 1, BN_bn2dec(bn_y));			
				}			

				// if (y == 1) return "composite"
				if (BN_cmp(bn_y, BN_value_one()) == 0) { 
					ret = 0;
					break;
				}
			}			

			// if y != n-1, return "composite"
			if (BN_cmp(bn_y, bn_n_minusOne) != 0) { 
				ret = 0;
				break;
			}		
		}
	}
	
	// print test result
	if (ret == 1) {
		for (k = 0; k < numInd - 2; k++)
			fprintf(stdout, " ");
		fprintf(stdout, "Miller-Rabin declares n to be a prime number\n");
	} else if (ret == 0) {
		for (k = 0; k < numInd - 2; k++)
			fprintf(stdout, " ");
		fprintf(stdout, "Miller-Rabin found a strong witness %s\n", BN_bn2dec(bn_a));
	} else { // ret == -1
		for (k = 0; k < numInd - 2; k++)
			fprintf(stdout, " ");
		fprintf(stdout, "Miller-Rabin failure, maxitr is too large\n");
	}

	// clean up
	free(bn_n_minusOne);
	free(bn_r);
	free(bn_a);
	free(bn_y);
	BN_CTX_free(bn_ctx);

	return ret;	
}

