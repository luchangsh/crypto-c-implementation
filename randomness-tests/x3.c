#include "x3.h"

void x3(FILE *fp)
{
	const double threshold[] = {0, 3.8415, 7.8147, 14.0671, 24.9958, 44.9853,
		82.5287, 154.3015, 293.2478, 564.6961, 1098.5208};
	
	/* Create a temp file */
	char filename[] = "/tmp/fileXXXXXX";
	int fd;
	if ((fd = mkstemp(filename)) == -1) {
		fprintf(stderr, "x3: create a temp file %s failed.\n", filename);
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
			fprintf(stderr, "x3: write to a temp file %s failed.\n", filename);
			exit(EXIT_FAILURE);
		}
	}
	if (count < 256 && count % 16 != 0) fprintf(stdout, "\n");
	
	/* If n < 10, print error and exit */
	if (n < 10) {
		fprintf(stderr, "n = %li\n", n);
		fprintf(stderr, "x3: insufficient data\n");
		exit(EXIT_FAILURE);
	}

	/* Rewind to the begining of the temp file */
	if (lseek(fd, 0L, SEEK_SET) == -1) {
		fprintf(stderr, "x3: rewind to the begining of the temp file %s failed.\n", filename);
		exit(EXIT_FAILURE);
	}

	/* Calculate m and k */
	int m = 1;
	while (n / m >= 5 * pow(2.0, (double) m)) m++;
	m--;
	if (m >= 11) {
		fprintf(stderr, "n = %li\n", n);
		fprintf(stderr, "m = %d\n", m);
		fprintf(stderr, "x3: m is too large\n");
		exit(EXIT_FAILURE);
	}
	int k = n / m;
	
	/* Read the input file, trasform each bit into a byte, put into bits[] */
	unsigned short bits[n];
	int shift, i = 0;
	while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
		for (shift = 7; shift >= 0; shift--, i++)
			bits[i] = (buffer[0] >> shift) & 1;
	
	/* len = 2^m */
	int len = 1;
	for (i = 0; i < m; i++) len *= 2;

	/* Create ni[], for example: [n000, n001, ... , n111] */
	unsigned short ni[len];
	for (i = 0; i < len; i++) ni[i] = 0;
	
	/* Count ni */
	unsigned short tmp;
	int j;
	for (i = 0; i < k; i++) {
		tmp = 0;
		for (j = 0; j < m; j++)
			tmp = ((tmp << 1) | bits[i * m + j]);
		ni[tmp]++;
	}
	
	/* Calculate x3 */
	double sum = 0;
	for (i = 0; i < len; i++) sum += pow((double) ni[i], 2.0);
	double x3 = len * sum / k - k;	

	/* Output test results */
	fprintf(stdout, "n = %li\n", n);
	fprintf(stdout, "m = %d\n", m);
	fprintf(stdout, "k = %d\n", k);
	char s[m + 1];
	for (i = 0; i < len; i++)
		fprintf(stdout, "n%s = %hu\n", toBinStr(i, s, m + 1), ni[i]);
	fprintf(stdout, "x3 = %.6f\n", x3);
	fprintf(stdout, "pass/fail = %s\n", (x3 < threshold[m]) ? "pass" : "fail");
	
	/* Close file */
	close(fd);
}

char *toBinStr(int num, char *s, int sLen)
{
	int i;
	for (i = 0; i < sLen; i++) s[i] = '0';
	s[sLen - 1] = '\0';
	i = sLen - 2;
	while (num != 0) {
		s[i--] = (num % 2) ? '1' : '0';
		num /= 2;
	}
	return s;	
}


