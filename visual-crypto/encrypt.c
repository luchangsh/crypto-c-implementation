#include "encrypt.h"

void encrypt(const char *pphrase, const char *name, FILE *fp)
{
	int i, j, k; /* index */

	/* check the input file is a valid PBM file */
	
	/* check if the first line of the input file is "P4\n" */
	char magicNum[MNUM_SIZE];
	for (i = 0; i < MNUM_SIZE; i++) magicNum[i] = 0;	
	if (fgets(magicNum, MNUM_SIZE, fp) == NULL) {
		fprintf(stderr, "Read input file failed\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(magicNum, "P4\n") != 0) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);
	}

	/* check if the second line of the input file is "<width> <height>\n" */
	char dimension[DIM_SIZE];
	for (i = 0; i < DIM_SIZE; i++) dimension[i] = 0;	
	if (fgets(dimension, DIM_SIZE, fp) == NULL) {
		fprintf(stderr, "Read input file failed\n");
		exit(EXIT_FAILURE);
	}

	/* parse dimension line to obtain width and height */	
	char *pHeight;
	if ((pHeight = strchr(dimension, ' ')) == NULL) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);
	}
	*pHeight++ = '\0';
	char *p;
	if ((p = strchr(pHeight, '\n')) == NULL) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);		
	}
	*p = '\0';
	int width;
	if ((width = parseInt(dimension)) == -1) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);	
	}
	int height;
	if ((height = parseInt(pHeight)) == -1) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);
	}
	

	/* create buffer for the input file */
	int inRows = height;
	int inCols = (width % 8 == 0) ? width / 8 : width / 8 + 1;
	unsigned char in[inRows][inCols];
	for (i = 0; i < inRows; i++) {
		for (j = 0; j < inCols; j++) {
			fread(&in[i][j], sizeof(unsigned char), 1, fp);
		}
	}
	

	/* call stream function for key generation */
	int bits = width * height;
	int keyLen = (bits % 8 == 0) ? bits / 8 : bits / 8 + 1;
	unsigned char keyBuf[keyLen];
	stream(pphrase, keyLen, keyBuf);


	/* create buffer for the output */
	int outRows = inRows * 2;
	int outCols = (width * 2 % 8 == 0) ? width * 2 / 8 : width * 2 / 8 + 1;
	unsigned char out1[outRows][outCols];
	unsigned char out2[outRows][outCols];
	for (i = 0; i < outRows; i++) {
		for (j = 0; j < outCols; j++) {
			out1[i][j] = 0; /* initialization */
			out2[i][j] = 0;
		}
	}


	/* perform encryption algorithm */
	int r, c; /* row and col for the output buffer */
	int cnt; /* bit count of the current line */
	int total=0; /* total bit counts */
	unsigned char curr; /* current input character */
	unsigned char currKey; /* current key character */
	int pixel; /* input pixel */
	int key; /* key bit */
	int shift;
	for (i = 0; i < inRows; i++) {
		r = i * 2;
		cnt = 0; /* cnt: bit count per line, reset it when processing a new line */
		for (j = 0; j < inCols; j++) {
			curr = in[i][j];
			/* k is the shift of the curr to get current pixel */
			for (k = 7; cnt < width && k >= 0; k--, cnt++, total++) {
				pixel = (curr >> k) & 1; /* get input pixel */
				currKey = keyBuf[total / 8];
				key = (currKey >> (7 - (total % 8))) & 1;
				c = (k >= 4) ? 2 * j : 2 * j + 1; /* left 4 bits to the left char, right 4 bits to the right char */
				shift = (k % 4) * 2;
				if (pixel) {
					if (key) {	/* pixel=1, key=1 */
						out1[r][c] |= 1 << shift;
						out1[r+1][c] |= 2 << shift;
						out2[r][c] |= 2 << shift;
						out2[r+1][c] |= 1 << shift;
					} else {	/* pixel=1, key=0 */
						out1[r][c] |= 2 << shift;
						out1[r+1][c] |= 1 << shift;
						out2[r][c] |= 1 << shift;
						out2[r+1][c] |= 2 << shift;						
					}
				} else {
					if (key) {	/* pixel=0, key=1 */
						out1[r][c] |= 1 << shift;
						out1[r+1][c] |= 2 << shift;
						out2[r][c] |= 1 << shift;
						out2[r+1][c] |= 2 << shift;						
					} else {	/* pixel=0, key=0 */
						out1[r][c] |= 2 << shift;
						out1[r+1][c] |= 1 << shift;
						out2[r][c] |= 2 << shift;
						out2[r+1][c] |= 1 << shift;						
					}
				}
			}
		}
	}

	
	/* create the two encrypted PBM files */
	FILE *ofp = NULL;	
	int appendLen = strlen(".1.pbm");
	char outFileName[strlen(name) + appendLen + 1];
	
	/* output file 1: <name>.1.pbm */
	sprintf(outFileName, "%s.%d.%s", name, 1, "pbm");
	if ((ofp = fopen(outFileName, "wb")) == NULL) {
		fprintf(stderr, "Can't open file %s for writing\n", outFileName);
		exit(EXIT_FAILURE);
	}
	fprintf(ofp, "%s\n", "P4");
	fprintf(ofp, "%d %d\n", width * 2, height * 2);
	for (i = 0; i < outRows; i++) {
		for (j = 0; j < outCols; j++) {
			fwrite(&out1[i][j], sizeof(unsigned char), 1, ofp);
		}
	}	
	if (fclose(ofp) != 0) {
		fprintf(stderr, "Error in closing file %s\n", outFileName);
		exit(EXIT_FAILURE);
	}

	/* output file 2: <name>.2.pbm */
	sprintf(outFileName, "%s.%d.%s", name, 2, "pbm");
	if ((ofp = fopen(outFileName, "wb")) == NULL) {
		fprintf(stderr, "Can't open file %s for writing\n", outFileName);
		exit(EXIT_FAILURE);
	}
	fprintf(ofp, "%s\n", "P4");
	fprintf(ofp, "%d %d\n", width * 2, height * 2);
	for (i = 0; i < outRows; i++) {
		for (j = 0; j < outCols; j++) {
			fwrite(&out2[i][j], sizeof(unsigned char), 1, ofp);
		}
	}
	if (fclose(ofp) != 0) {
		fprintf(stderr, "Error in closing file %s\n", outFileName);
		exit(EXIT_FAILURE);
	}			
}
