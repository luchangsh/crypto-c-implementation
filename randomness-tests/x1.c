#include "x1.h"

void x1(FILE *fp)
{
	unsigned char buffer[1];
	unsigned char c;
	int i, count = 0;
	long n = 0, n0 = 0, n1 = 0;
	size_t bytes = 0;
	
	while ((bytes = fread(buffer, sizeof(unsigned char), 1, fp)) > 0) {
		c = buffer[0];
		
		/* Hexdump the first 256 bytes */
		if (++count <= 256) {
			fprintf(stdout, "%02x", c);
			if (count % 16 == 0) fprintf(stdout, "\n"); /* 16 bytes per line */
		}
		
		/* Count n, n0, n1 */
		for (i = 0; i < 8; i++, c >>= 1) {
			if (c & 1) n1++;
			else n0++;
		}
		n += 8;
	}
	if (count < 256 && count % 16 != 0) fprintf(stdout, "\n");
	
	/* If n < 10, print error and exit */
	if (n < 10) {
		fprintf(stderr, "n = %li\n", n);
		fprintf(stderr, "x1: insufficient data\n");
		exit(EXIT_FAILURE);
	}
	
	/* Calculate x1 */
	double x1 = pow((double) (n0 - n1), 2.0) / n;
	
	/* Output test results */
	fprintf(stdout, "n = %li\n", n);
	fprintf(stdout, "n0 = %li\n", n0);
	fprintf(stdout, "n1 = %li\n", n1);
	fprintf(stdout, "x1 = %.6f\n", x1);
	fprintf(stdout, "pass/fail = %s\n", (x1 < 3.8415) ? "pass" : "fail");
}
