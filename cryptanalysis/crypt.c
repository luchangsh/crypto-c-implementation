#include "crypt.h"
void crypt(FILE *kfp, FILE *ifp)
{
	int i, j; /* indexes */
	size_t bytes; /* number of bytes read into buffer */
	int line = 1; /* Line number */
	unsigned char buffer[BUF_SIZE]; /* Key buffer */
	for (i = 0; i < BUF_SIZE; i++) { /* Initialization */
		buffer[i] = 0;
	}
	int count[CNT_SIZE];  /* Count array */
	for (i = 0; i < CNT_SIZE; i++) { /* Initialization */
		count[i] = 0;
	}
	
	/* Verify the key file and determine the number of line (the period) */
	while ((bytes = fread(buffer, sizeof(char), BUF_SIZE, kfp)) > 0) {
		if (buffer[BUF_SIZE - 1] != '\n') { /* Number of characters is wrong */
			fprintf(stderr, "Malformed key file (Line %d): Wrong number of characters.\n", line);
			exit(EXIT_FAILURE);
		}
		for (i = 0; i < BUF_SIZE - 1; i++) {
			if (buffer[i] < 'a' || buffer[i] > 'z') { /* Character is NOT a-z */
				fprintf(stderr, "Malformed key file (Line %d): Contains invalid characters.\n", line);
				exit(EXIT_FAILURE);
			}
			count[buffer[i] - 'a']++; /* Count characters */
		}
		for (i = 0; i < CNT_SIZE; i++) {
			if (count[i] != 1) { /* Every character can only appear exactly one time */
				fprintf(stderr, "Malformed key file (Line %d): Missing or duplicated characters.\n", line);
				exit(EXIT_FAILURE);
			}
			count[i] = 0; /* Reset count[] */
		}

		/* Update variables */
		line++; /* Next line */
		for (i = 0; i < BUF_SIZE; i++) {
			buffer[i] = 0; /* Reset buffer */
		}
	}

	/* Go to the beginning of the file */
	fseek(kfp, 0L, SEEK_SET);
	
	/* Create a key file buffer to save key file information */
	int period = line - 1;
	unsigned char bufKeyfile[period][BUF_SIZE];
	for (i = 0; i < period; i++) {
		for (j = 0; j < BUF_SIZE; j++) {
			fread(&bufKeyfile[i][j], sizeof(char), 1, kfp);
		}
	}

	/* Encrypt or decrypt */
	int ch, cnt = 0;
	while ((ch = fgetc(ifp)) != '\n') {
		if (ch < 0x20 || ch > 0x7e) {
			fprintf(stderr, "Input text contains invalid characters.\n");
			exit(EXIT_FAILURE);
		} else if (ch >= 'a' && ch <= 'z') {
			fputc(bufKeyfile[cnt % period][ch - 'a'], stdout);
		} else {
			fputc(ch, stdout);
		}
		cnt++;
	}
	fprintf(stdout, "\n");
}
