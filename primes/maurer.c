#include "maurer.h"

void maurer(int numbits, FILE *pfp, FILE *rfp)
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

	// create BIGNUM for the random number
	BIGNUM *bn_n = NULL;
	bn_n = BN_new();
	if (bn_n == NULL) {
		fprintf(stderr, "maurer: Create bn_n failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_n);

	// maurer algorithm
	maurerRecursive(numbits, bn_n, primes, fileSize, rfp, 0);
	
	// print n
	fprintf(stdout, "\n");
	fprintf(stdout, "Maurer's Algorithm found an %d-bit prime:\n", numbits);
	fprintf(stdout, "  n = %s\n", BN_bn2dec(bn_n));
	
	// clean up
	free(bn_n);	
}

void maurerRecursive(int k, BIGNUM *bn_n, unsigned int *primes, int arrLen, FILE *rfp, int level)
{
	// print level and k
	fprintf(stdout, "Maurer: level %d, k=%d\n", level, k);	

	// create a input buffer for rfp
	int rbuf_size = (k % 8 == 0) ? k / 8 : k / 8 + 1;
	unsigned char rbuf[rbuf_size];
	int i;
	for (i = 0; i < rbuf_size; i++) rbuf[i] = 0;

	// check if k is small enough (k <= 20)	
	if (k <= 20) {
		while (1) {
			RndOddNum(bn_n, k, rbuf, rbuf_size, rfp);
			fprintf(stdout, "  step 1.1, n = %s\n", BN_bn2dec(bn_n));
			if (trialDivision(bn_n, primes, arrLen, 4) == 1) return;		
		}	
	}

	int m = 20;

	// generate a fraction r, the size of q relative to n
	double r;
	if (k <= 2 * m) {
		r = 0.5;
		fprintf(stdout, "  step 4, r = 50%%\n");
	} else {
		unsigned char c;
		while (1) {
			c = rndByte(rfp);
			r = c / 255.0;
			r = 0.5 + r / 2.0;
			if (k * (1 - r) > m) break;
		}
		fprintf(stdout, "  step 4: random byte = %d, r = %d%%\n", c, round(100 * r));
	}

	// cal maurer function recursively
	maurerRecursive((int) floor(r * k) + 1, bn_n, primes, arrLen, rfp, level + 1);

	// create bn_q for q
	BIGNUM *bn_q = NULL;
	bn_q = BN_dup(bn_n);
	if (bn_q == NULL) {
		fprintf(stderr, "maurerRecursive: Create bn_q failed.\n");
		exit(EXIT_FAILURE);
	}	

	// print the current level k and q
	fprintf(stdout, "Maurer: back to level %d, k=%d, q=%s\n", level, k, BN_bn2dec(bn_q));

	int num_bits_in_q = BN_num_bits(bn_q);

	// bn_tmp to save 2^(k-2)
	BIGNUM *bn_tmp = NULL;
	bn_tmp = BN_new();
	if (bn_tmp == NULL) {
		fprintf(stderr, "maurerRecursive: Create bn_tmp failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_tmp);
	// compute 2^(k-2)
	if (BN_lshift(bn_tmp, BN_value_one(), k - 2) == 0)	{
		fprintf(stderr, "maurerRecursive: Compute 2^(k-2) failed.\n");
		exit(EXIT_FAILURE);
	}

	// create bn_i for I
	BIGNUM *bn_i = NULL;
	bn_i = BN_new();
	if (bn_i == NULL) {
		fprintf(stderr, "maurerRecursive: Create bn_i failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_i);

	// create a temperatory variable
	BN_CTX *bn_ctx = NULL;
	bn_ctx = BN_CTX_new();
	if (bn_ctx == NULL) {
		fprintf(stderr, "maurerRecursive: Create bn_ctx failed.\n");
		exit(EXIT_FAILURE);
	}

	// compute floor(2^(k-2) / q)
	if (BN_div(bn_i, NULL, bn_tmp, bn_q, bn_ctx) == 0)	{
		fprintf(stderr, "maurerRecursive: Compute floor(2^(k-2) / q) failed.\n");
		exit(EXIT_FAILURE);
	}

	// create bn_r for R
	BIGNUM *bn_r = NULL;
	bn_r = BN_new();
	if (bn_r == NULL) {
		fprintf(stderr, "maurerRecursive: Create bn_r failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_r);

	// create a buffer for rndfile
	int num_bits = k + 1 - num_bits_in_q;
	int rbuf_size_2 = (num_bits % 8 == 0) ? num_bits / 8 : num_bits / 8 + 1;
	unsigned char rbuf_2[rbuf_size_2];
	for (i = 0; i < rbuf_size_2; i++) rbuf_2[i] = 0;

	// create bn_a for a
	BIGNUM *bn_a = NULL;
	bn_a = BN_new();
	if (bn_a == NULL) {
		fprintf(stderr, "maurerRecursive: Create bn_a failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_a);

	// create bn_n_minusOne
	BIGNUM *bn_n_minusOne = NULL;
	bn_n_minusOne = BN_new();
	if (bn_n_minusOne == NULL) {
		fprintf(stderr, "maurerRecursive: Create bn_n_minusOne failed.\n");
		exit(EXIT_FAILURE);
	}

	// create bn_b for b
	BIGNUM *bn_b = NULL;
	bn_b = BN_new();
	if (bn_b == NULL) {
		fprintf(stderr, "maurerRecursive: Create bn_b failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_b);

	// create bn_d for d
	BIGNUM *bn_d = NULL;
	bn_d = BN_new();
	if (bn_d == NULL) {
		fprintf(stderr, "maurerRecursive: Create bn_d failed.\n");
		exit(EXIT_FAILURE);
	}
	BN_zero(bn_d);

	int itr = 1;
	while (1) {
		// get R
		RndOddNum(bn_r, num_bits, rbuf_2, rbuf_size_2, rfp);

		// compute R mod I
		if (BN_mod(bn_tmp, bn_r, bn_i, bn_ctx) == 0) {
			fprintf(stderr, "maurerRecursive: Compute (R mod I) failed.\n");
			exit(EXIT_FAILURE);
		}

		// compute R = (R mod I) + I + 1
		if (BN_add(bn_tmp, bn_tmp, bn_i) == 0) {
			fprintf(stderr, "maurerRecursive: Compute ((R mod I) + I) failed.\n");
			exit(EXIT_FAILURE);
		}
		if (BN_add(bn_r, bn_tmp, BN_value_one()) == 0) {
			fprintf(stderr, "maurerRecursive: Compute ((R mod I) + I + 1) failed.\n");
			exit(EXIT_FAILURE);
		}

		// compute R * q
		if (BN_mul(bn_tmp, bn_r, bn_q, bn_ctx) == 0) {
			fprintf(stderr, "maurerRecursive: Compute (R*q) failed.\n");
			exit(EXIT_FAILURE);
		}

		// compute 2 * R * q
		if (BN_lshift1(bn_tmp, bn_tmp) == 0) {
			fprintf(stderr, "maurerRecursive: Compute (2*R*q) failed.\n");
			exit(EXIT_FAILURE);
		}

		// compute 2 * R * q + 1
		if (BN_add(bn_n, bn_tmp, BN_value_one()) == 0) {
			fprintf(stderr, "maurerRecursive: Compute (2*R*q + 1) failed.\n");
			exit(EXIT_FAILURE);
		}

		// print iteration, R, and n
		fprintf(stdout, "  step 7, itr %d: R = %s, n = %s\n", itr, BN_bn2dec(bn_r), BN_bn2dec(bn_n));

		// trial division test
		if (trialDivision(bn_n, primes, arrLen, 4) != 0) {
			int num_bits_in_n = BN_num_bits(bn_n);

			// create a buffer for a
			int rbuf_size_a = (num_bits_in_n % 8 == 0) ? num_bits_in_n / 8 : num_bits_in_n / 8 + 1;
			unsigned char rbuf_a[rbuf_size_a];
			for (i = 0; i < rbuf_size_a; i++) rbuf_a[i] = 0;

			// compute n - 1			
			if (!BN_sub(bn_n_minusOne, bn_n, BN_value_one())) {
				fprintf(stderr, "maurerRecursive: Create bn_n_minusOne failed.\n");
				exit(EXIT_FAILURE);
			}			

			do {
				RndOddNum(bn_a, num_bits_in_n, rbuf_a, rbuf_size_a, rfp);
			} while (BN_cmp(bn_a, BN_value_one()) != 1 || BN_cmp(bn_a, bn_n_minusOne) != -1);

			// print a
			fprintf(stdout, "  step 7.2.1, itr %d: a = %s\n", itr, BN_bn2dec(bn_a));		
			
			// compute b = a ^ (n - 1) mod n
			if (!BN_mod_exp(bn_b, bn_a, bn_n_minusOne, bn_n, bn_ctx)) {
				fprintf(stderr, "maurerRecursive: Compute bn_b failed.\n");
				exit(EXIT_FAILURE);
			}

			// if (b == 1)
			if (BN_is_one(bn_b)) {
				// R -> 2R				
				if (!BN_lshift1(bn_r, bn_r)) {
					fprintf(stderr, "maurerRecursive: Compute (2*R) failed.\n");
					exit(EXIT_FAILURE);
				}

				// b = a ^ (2 * R) mod n
				if (!BN_mod_exp(bn_b, bn_a, bn_r, bn_n, bn_ctx)) {
					fprintf(stderr, "maurerRecursive: Compute (b = a^(2R) mod n) failed.\n");
					exit(EXIT_FAILURE);
				}

				// b = b - 1
				if (!BN_sub(bn_b, bn_b, BN_value_one())) {
					fprintf(stderr, "maurerRecursive: Create (b = b-1) failed.\n");
					exit(EXIT_FAILURE);
				}

				// d = gcd(b - 1, n)
				if (!BN_gcd(bn_d, bn_b, bn_n, bn_ctx)) {
					fprintf(stderr, "maurerRecursive: Compute (d = gcd(b-1, n)) failed.\n");
					exit(EXIT_FAILURE);
				}

				// if (d == 1) break;
				if (BN_is_one(bn_d)) break;				
			}
		}
		itr++;
	}	

	// clean up
	free(bn_q);
	free(bn_tmp);
	free(bn_i);
	BN_CTX_free(bn_ctx);
	free(bn_r);
	free(bn_a);
	free(bn_n_minusOne);
	free(bn_b);
	free(bn_d);	
}

unsigned char rndByte(FILE *rfp)
{
	unsigned char buf[1];
	buf[0] = 0;
	size_t bytes = 0;

	if ((bytes = fread(buf, sizeof(unsigned char), 1, rfp)) < 1) {
		fprintf(stderr, "rndByte: run out of data in the rndfile.\n");
		exit(EXIT_FAILURE);
	}
	
	return buf[0];
}
