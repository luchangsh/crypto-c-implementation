#include "x5.h"

void x5(int dval, FILE *fp)
{
	/* Create a temp file */
	char filename[] = "/tmp/fileXXXXXX";
	int fd;
	if ((fd = mkstemp(filename)) == -1) {
		fprintf(stderr, "x5: create a temp file %s failed.\n", filename);
		exit(EXIT_FAILURE);
	}

	unlink(filename);
	
	/* Read the content of the input file into the temp file */
	unsigned char buffer[1];
	int count = 0;
	long n = 0;
	size_t bytes = 0;
	
	while ((bytes = fread(buffer, sizeof(unsigned char), 1, fp)) > 0) {
		/* Hexdump the first 256 bytes */
		if (++count <= 256) {
			fprintf(stdout, "%02x", buffer[0]);
			if (count % 16 == 0) fprintf(stdout, "\n"); /* 16 bytes per line */
		}

		/* Count n */
		n += 8;

		/* Write to the temp file */
		if (write(fd, buffer, sizeof(buffer)) == -1) {
			fprintf(stderr, "x5: write to a temp file %s failed.\n", filename);
			exit(EXIT_FAILURE);
		}
	}
	if (count < 256 && count % 16 != 0) fprintf(stdout, "\n");
	
	/* If n < d + 10, print error and exit */
	if (n < dval + 10) {
		fprintf(stderr, "n = %li\n", n);
		fprintf(stderr, "x5: insufficient data\n");
		exit(EXIT_FAILURE);
	}
	
	/* If d < 1 or d > n / 2, print error and exit */
	if (dval < 1 || dval > (n >> 1)) {
		fprintf(stderr, "n = %li\n", n);
		fprintf(stderr, "x5: d should be in the range [%d,%li]\n", 1, n >> 1);
		exit(EXIT_FAILURE);
	}
	
	/* Rewind to the begining of the temp file */
	if (lseek(fd, 0L, SEEK_SET) == -1) {
		fprintf(stderr, "x5: rewind to the begining of the temp file %s failed.\n", filename);
		exit(EXIT_FAILURE);
	}

	/* Read the content of the temp file into s[] */
	int i = 0, sLen = n / 8;
	unsigned char s[sLen];
	while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
		s[i++] = buffer[0];

	/* Calculate A(d) */
	long A = 0;
	for (i = 0; i < n - dval; i++)
		A += ((s[i / 8] >> (7 - (i % 8))) & 1) ^ 
			 ((s[(i + dval) / 8] >> (7 - ((i + dval) % 8))) & 1);
	
	/* Calculate x5 */
	double x5 = 2 * (A - (n - dval) / 2.0) / sqrt((double) (n - dval));
	
	/* Output test results */
	fprintf(stdout, "n = %li\n", n);
	fprintf(stdout, "d = %d\n", dval);
	fprintf(stdout, "A(d) = %li\n", A);
	fprintf(stdout, "x5 = %.6f\n", x5);
	fprintf(stdout, "pass/fail = %s\n", (fabs(x5) < 1.96) ? "pass" : "fail");
	
	/* Close file */
	close(fd);
}

