#include "decrypt.h"

void decrypt(FILE *fp)
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
	if ((height = parseInt(pHeight)) == -1) {  /* parse width */
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);
	}
	
	
	/* create buffer for the input file */		
	int inRows = height;
	int bytes = (width % 8 == 0) ? width / 8 : width / 8 + 1;
	int inCols = (bytes % 2 == 0) ? bytes : bytes + 1; /* make inCols to even */
	unsigned char in[inRows][inCols];
	for (i = 0; i < inRows; i++) {
		for (j = 0; j < inCols; j++) {
			if (j < bytes) {
				fread(&in[i][j], sizeof(unsigned char), 1, fp);
			} else {
				in[i][j] = 0; /* pad the additional column */
			}
		}
	}
	
	
	/* create buffer for the output */
	int outRows = inRows / 2;
	int outCols = inCols / 2;
	unsigned char out[outRows][outCols];
	for (i = 0; i < outRows; i++) {
		for (j = 0; j < outCols; j++) {
			out[i][j] = 0; /* initialization */
		}
	}
	
	
	/* decrypt the input buffer */
	int row, col, shift, cnt; /* cnt: output bit count per line*/
	unsigned char c1, c2, mask = 3, top, btm; /* mask=00000011 */
	for (i = 0; i < inRows; i += 2) { /* process 2 rows per iteration */
		row = i;
		cnt = 0; /* reset bit count */
		for (j = 0; j < inCols; j += 2) { /* process 2 cols per iteration */
			for (k = 7; cnt < width / 2 && k >= 0; k--, cnt++) { /* bit shift for the output char, don't process padding bits */
				shift = (k % 4) * 2; /* bit shift for input chars */
				col = (k >= 4) ? j : j + 1; /* k = 4~7 the first char, k = 0~3 the second char */
				c1 = in[row][col];
				c2 = in[row + 1][col];
				top = (c1 >> shift) & mask; /* extract the top two bits of the merged pic */
				btm = (c2 >> shift) & mask; /* extract the btm two bits of the merged pic */
				if (top == 3 && btm == 3) { /* if the merged pic are all black */
					out[i / 2][j / 2] |= (1 << k); /* set the output bit to black */
				} else if (!(top == 2 && btm == 1) && !(top == 1 && btm == 2)) { /* handle invalid pattern */
					fprintf(stderr, "Errors in the input merged file\n");
					exit(EXIT_FAILURE);					
				}
			}
		}
	}
	
	
	/* output the P4 PBM file */
	fprintf(stdout, "%s\n", "P4");
	fprintf(stdout, "%d %d\n", width / 2, height / 2);
	for (i = 0; i < outRows; i++) {
		for (j = 0; j < outCols; j++) {
			fwrite(&out[i][j], sizeof(unsigned char), 1, stdout);
		}
	}
}
