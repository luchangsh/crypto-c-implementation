#include "invkey.h"
void invkey(FILE *kfp)
{
	int i; /* indexes */
	size_t bytes; /* number of bytes read into buffer */
	int line = 1; /* Line number */
	unsigned char buffer[BUF_SIZE]; /* Original key buffer */
	for (i = 0; i < BUF_SIZE; i++) { /* Initialization */
		buffer[i] = 0;
	}
	int count[CNT_SIZE];  /* Count array */
	for (i = 0; i < CNT_SIZE; i++) { /* Initialization */
		count[i] = 0;
	}
	unsigned char invBuf[BUF_SIZE];  /* Inverted key buffer */
	for (i = 0; i < BUF_SIZE; i++) { /* Initialization */
		invBuf[i] = 0;
	}
	
	while ((bytes = fread(buffer, sizeof(char), BUF_SIZE, kfp)) > 0) {
		/* Check if this line is valid, if yes, process it, exit otherwise */
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
		
		/* Process this line */
		invBuf[BUF_SIZE - 1] = '\n';
		for (i = 0; i < BUF_SIZE - 1; i++) {
			invBuf[buffer[i] - 'a'] = i + 'a';
		}
		for (i = 0; i < BUF_SIZE; i++) {
			fprintf(stdout, "%c", invBuf[i]);
		}
		
		/* Update variables */
		line++; /* Next line */
		for (i = 0; i < BUF_SIZE; i++) {
			buffer[i] = 0; /* Reset buffer */
			invBuf[i] = 0; /* Reset invBuf */
		}
	}
}

