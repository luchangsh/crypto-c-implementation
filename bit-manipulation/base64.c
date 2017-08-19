#include "base64.h"
void encodeBase64(FILE *fp)
{
	/* base64 table */
	char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	size_t bytes = 0;
	int count = 0;
	unsigned char in[IN_SIZE_ENC]; /* input buffer */
	unsigned char out[OUT_SIZE_ENC]; /* output buffer */
		
	while ((bytes = fread(in, sizeof(char), IN_SIZE_ENC, fp)) > 0) {
	
		/* append '0' at the end of input file when bytes % 3 != 0 */
		if (bytes == 1) in[1] = in[2] = 0; /* one byte left */
		if (bytes == 2) in[2] = 0;         /* two bytes left */
		
		/* bit manipulation for encode 3 bytes input to 4 bytes output */		
		out[0] = base64[in[0] >> 2];
		out[1] = base64[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		out[2] = base64[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		out[3] = base64[in[2] & 0x3f];
		
		/* append '=' at the end of the output */		
		if (bytes == 1) out[2] = out[3] = '='; /* one byte left */
		if (bytes == 2) out[3] = '=';          /* two byte left */
		
		/* print encoded string */
		int i;
		for (i = 0; i < OUT_SIZE_ENC; i++)
			fprintf(stdout, "%c", out[i]);
		
		/* append new line character at the end of each line */
		count += bytes;
		if (count % (MAX_CHAR_PER_LINE / 4 * 3) == 0)
			fprintf(stdout, "\n");
	}
	
	/* append new line character for the last line if it is not a full line */
	if (count % MAX_CHAR_PER_LINE != 0) fprintf(stdout, "\n");
}

void decodeBase64(FILE *fp)
{
	unsigned char in[IN_SIZE_DEC]; /* input buffer */
	unsigned char out[OUT_SIZE_DEC]; /* output buffer */
	int ch = 0;
	int index = 0; /* index of input buffer */
	
	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '=') break; /* reach the end, handled below */
		if (ch == '\n') continue; /* skip new line character */
		
		/* chech if the input character is valid */
		if (!isValid(ch)) {
			fprintf(stderr, "ERROR: input contains invalid characters\n");
			exit(EXIT_FAILURE);
		}
		
		/* decode every 4 input bytes */
		in[index] = ch;
		if (index == IN_SIZE_DEC - 1)
			decodeBase64Helper(in, out, OUT_SIZE_DEC);
		
		index++;
		index %= IN_SIZE_DEC;
	}
	
	/* handle the condition of reaching the end of file */
	if (ch == EOF) {
		if (index != 0) { /* handle the error that the number of input characters is not multiplier of 4 */
			fprintf(stderr, "ERROR: the number of input characters is not multiplier of 4\n");
			exit(EXIT_FAILURE);			
		}
		return; /* normal termination */
	}
	
	/* handle the end of the input if there is '=' at the end */
	if (ch == '=') {
		if (index == 2) /* two = */
			decodeBase64Helper(in, out, OUT_SIZE_DEC - 2);
		else if (index == 3) /* one = */
			decodeBase64Helper(in, out, OUT_SIZE_DEC - 1);
		else { /*handle invalid ending format, '=' appears at index 0 or 1 */
			fprintf(stderr, "ERROR: invalid format of ending '='\n");
			exit(EXIT_FAILURE);
		}
	}
}

int isValid(int ch)
{
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || 
		(ch >= '0' && ch <= '9') ||	ch == '+' || ch == '/';
}

unsigned char revChar(char ch)
{
	if (ch >= 'A' && ch <= 'Z') return ch - 'A';
	else if (ch >= 'a' && ch <= 'z') return ch - 'a' + 26;
	else if (ch >= '0' && ch <= '9') return ch - '0' + 52;
	else if (ch == '+') return 62;
	return 63; /* ch == '/' */
}

void decodeBase64Helper(unsigned char in[], unsigned char out[], int len)
{
	/* bit manipulation for 4 bytes input to 3 bytes output */
	out[0] = (revChar(in[0]) << 2) | (revChar(in[1]) >> 4);
	out[1] = (revChar(in[1]) << 4) | (revChar(in[2]) >> 2);
	out[2] = (revChar(in[2]) << 6) | revChar(in[3]);
	
	/* print decoded bytes */
	int i;
	for (i = 0; i < len; i++) {
		fprintf(stdout, "%c", out[i]);
	}
}
