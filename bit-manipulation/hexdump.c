#include "hexdump.h"
void hexdump(FILE *fp)
{
	size_t bytes = 0;
	unsigned char buffer[BUFSIZE];
	int address = 0;
	
	/* read the input file line by line into buffer */
	while ((bytes = fread(buffer, sizeof(char), BUFSIZE, fp)) > 0) {
					
		/* print address */
		fprintf(stdout, "%06x: ", address);
		
		/* print hexadecimal */
		int i, count = bytes;
		for (i = 0; i < BUFSIZE; i++) {
			if (i == 8) fprintf(stdout, " "); /* an extra space after the first 8 bytes */
			if (--count < 0) fprintf(stdout, "-- "); /* reach the end of file */
			else fprintf(stdout, "%02x ", buffer[i]);
		}		
		fprintf(stdout, " "); /* an extra space after hexadecimal bytes */
		
		/* print ASCII */		
		count = bytes; /* reset count */
		for (i = 0; i < BUFSIZE; i++) {
			unsigned char c = buffer[i];
			if (--count < 0) fprintf(stdout, " "); /* reach the end of file */
			else if (c < 32 || c == 127) fprintf(stdout, "."); /* c < 0x20 or c == 0x7f*/
			else if (c <= 126) fprintf(stdout, "%c", c); /* c >= 0x20 and c <= 0x7e */
			else fprintf(stdout, "~"); /* c > 0x7e */
		}
		
		fprintf(stdout, "\n");
		address += BUFSIZE;
	}		
}
