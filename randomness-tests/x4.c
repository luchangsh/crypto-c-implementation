#include "x4.h"

void x4(FILE *fp)
{
	const double threshold[] = {0, 0, 5.9915, 9.4877, 12.5916, 15.5073, 18.3070,
								21.0261, 23.6848, 26.2962, 28.8693, 31.4104};
	
	/* Create a temp file */
	char filename[] = "/tmp/fileXXXXXX";
	int fd;
	if ((fd = mkstemp(filename)) == -1) {
		fprintf(stderr, "x4: create a temp file %s failed.\n", filename);
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
			fprintf(stderr, "x4: write to a temp file %s failed.\n", filename);
			exit(EXIT_FAILURE);
		}
	}
	if (count < 256 && count % 16 != 0) fprintf(stdout, "\n");
	
	/* If n < 79, print error and exit */
	if (n < 79) {
		fprintf(stderr, "n = %li\n", n);
		fprintf(stderr, "x4: insufficient data\n");
		exit(EXIT_FAILURE);
	}

	/* Rewind to the begining of the temp file */
	if (lseek(fd, 0L, SEEK_SET) == -1) {
		fprintf(stderr, "x4: rewind to the begining of the temp file %s failed.\n", filename);
		exit(EXIT_FAILURE);
	}
	
	/* Calculate k and ei */
	double e[12];
	int i;
	for (i = 0; i < 12; i++) e[i] = 0;	
	int k = 1;
	while ((e[k] = (n - k + 3) / pow(2.0, (double) (k + 2))) >= 5) k++;
	k--;

	/* Read the temp file, trasform each bit into a byte, put into bits[] */
	unsigned char bits[n];
	int shift;
	i = 0;
	while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
		for (shift = 7; shift >= 0; shift--, i++)
			bits[i] = (buffer[0] >> shift) & 1;
	
	/* Count Bi and Gi */
	unsigned int B[12], G[12];
	for (i = 0; i < 12; i++) B[i] = G[i] = 0;
	int cnt = 1;
	unsigned char cur = bits[0];

	for (i = 1; i < n; i++) {
		if (bits[i] == cur) cnt++;
		else {
			if (cnt < 12) { /* max of consecutive 0 or 1 is 11 */
				if (cur) B[cnt]++;
				else G[cnt]++;
			}
			cnt = 1;
			cur = bits[i];
		}
	}
	if (cnt < 12) { /* the last bit */
		if (cur) B[cnt]++;
		else G[cnt]++;
	}
	
	/* Calculate x4 */	
	double x4 = 0;
	for (i = 1; i <= k; i++)
		x4 += pow(B[i] - e[i], 2.0) / e[i] + pow(G[i] - e[i], 2.0) / e[i];
	
	/* Output test results */
	fprintf(stdout, "n = %li\n", n);
	fprintf(stdout, "k = %d\n", k);
	for (i = 1; i <= k; i++) {
		fprintf(stdout, "e%d = %.6f\n", i, e[i]);
		fprintf(stdout, "B%d = %u\n", i, B[i]);
		fprintf(stdout, "G%d = %u\n", i, G[i]);
	}
	fprintf(stdout, "x4 = %.6f\n", x4);
	fprintf(stdout, "pass/fail = %s\n", (x4 < threshold[k]) ? "pass" : "fail");
	
	/* Close temp file */
	close(fd);
}
