#include "x2.h"

void x2(FILE *fp)
{
	unsigned char buffer[1];
	unsigned char c, tmp, pre, cur, last;
	int i, count = 0;
	long n=0, n0=0, n1=0, n00=0, n01=0, n10=0, n11=0;
	size_t bytes = 0;
	
	while ((bytes = fread(buffer, sizeof(unsigned char), 1, fp)) > 0) {
		c = buffer[0];
		
		/* hexdump the first 256 bytes */
		if (++count <= 256) {
			fprintf(stdout, "%02x", c);
			if (count % 16 == 0) fprintf(stdout, "\n"); /* 16 bytes per line */
		}
		
		/* count n, n0, n1, n00, n01, n10, n11 */
		tmp = c & 1;
		for (i = 0; i < 8; i++, c >>= 1) {
			cur = c & 1; /* current bit */
			
			/* Count n0, n1 */
			if (cur) n1++;
			else n0++;
			
			/* Count n00, n01, n10, n11 within-byte */
			if (i > 0) {
				if (cur) {
					if (pre) n11++; /* the bit on the right */
					else n10++;
				} else {
					if (pre) n01++;
					else n00++;
				}
			}
			
			/* Count n00, n01, n10, n11 between-bytes */
			if (count > 1 && i == 7) {
				if (last) { /* least significant bit of the previous byte */
					if (cur) n11++;
					else n10++;
				} else {
					if (cur) n01++;
					else n00++;
				}
			}
			pre = cur;
		}
		last = tmp;
		n += 8; /* Count n */
	}
	if (count < 256 && count % 16 != 0) fprintf(stdout, "\n");
	
	/* If total number of bits < 21, print error and exit */
	if (n < 21) {
		fprintf(stderr, "n = %li\n", n);
		fprintf(stderr, "x2: insufficient data\n");
		exit(EXIT_FAILURE);
	}
	
	/* Calculate x2 */
	double sum1 = pow((double) n00, 2.0) + pow((double) n01, 2.0) +
				  pow((double) n10, 2.0) + pow((double) n11, 2.0);
	double sum2 = pow((double) n0, 2.0) + pow((double) n1, 2.0);
	double x2 = 4 * sum1 / (n - 1) - 2 * sum2 / n + 1;
	
	/* Output test results */
	fprintf(stdout, "n = %li\n", n);
	fprintf(stdout, "n0 = %li\n", n0);
	fprintf(stdout, "n1 = %li\n", n1);
	fprintf(stdout, "n00 = %li\n", n00);
	fprintf(stdout, "n01 = %li\n", n01);
	fprintf(stdout, "n10 = %li\n", n10);
	fprintf(stdout, "n11 = %li\n", n11);
	fprintf(stdout, "x2 = %.6f\n", x2);
	fprintf(stdout, "pass/fail = %s\n", (x2 < 5.9915) ? "pass" : "fail");
}
