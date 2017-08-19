#include "merge.h"

void merge(FILE *fp1, FILE *fp2)
{
	int i, j; /* index */
	
	/* check the input file is a valid PBM file */
	
	/* check if the first line of the input file is "P4\n" */
	char magicNum[MNUM_SIZE];
	
	/* check fp1 */
	for (i = 0; i < MNUM_SIZE; i++) magicNum[i] = 0;
	if (fgets(magicNum, MNUM_SIZE, fp1) == NULL) {
		fprintf(stderr, "Read input file failed\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(magicNum, "P4\n") != 0) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);
	}
	
	/* check fp2 */
	for (i = 0; i < MNUM_SIZE; i++) magicNum[i] = 0;
	if (fgets(magicNum, MNUM_SIZE, fp2) == NULL) {
		fprintf(stderr, "Read input file failed\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(magicNum, "P4\n") != 0) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);
	}

	/* check if the second line of the input file is "<width> <height>\n" */
	char dimension[DIM_SIZE];
	/* check fp1 */	
	for (i = 0; i < DIM_SIZE; i++) dimension[i] = 0;
	if (fgets(dimension, DIM_SIZE, fp1) == NULL) {
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
	int width1;
	if ((width1 = parseInt(dimension)) == -1) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);	
	}
	int height1;
	if ((height1 = parseInt(pHeight)) == -1) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);
	}
	/* check fp2 */	
	for (i=0; i<DIM_SIZE; i++) dimension[i] = 0;
	if (fgets(dimension, DIM_SIZE, fp2) == NULL) {
		fprintf(stderr, "Read input file failed\n");
		exit(EXIT_FAILURE);
	}
	if ((pHeight = strchr(dimension, ' ')) == NULL) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);
	}
	
	/* parse dimension line to obtain width and height */	
	*pHeight++ = '\0';
	if ((p = strchr(pHeight, '\n')) == NULL) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);		
	}
	*p = '\0';
	int width2;
	if ((width2 = parseInt(dimension)) == -1) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);	
	}
	int height2;
	if ((height2 = parseInt(pHeight)) == -1) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);
	}
	
	/* check if file1 and file2 have the same dimension */	
	if (width1 != width2 || height1 != height2) {
		fprintf(stderr, "Improper input file format\n");
		exit(EXIT_FAILURE);		
	}
	
	
	/* output the merged image */
	int numRows = height1; /* height1 == height2 */
	int numCols = (width1 % 8 == 0) ? width1 / 8 : width1 / 8 + 1; /* width1 == width2 */
	unsigned char c, c1, c2;

	fprintf(stdout, "%s\n", "P4");
	fprintf(stdout, "%d %d\n", width1, height1);
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			c1 = fgetc(fp1);
			c2 = fgetc(fp2);
			c = c1 | c2; /* overlay == OR bit operation */
			fwrite(&c, sizeof(unsigned char), 1, stdout);
		}
	}	
}
