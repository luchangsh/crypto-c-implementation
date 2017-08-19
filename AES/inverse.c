#include "inverse.h"

void inverse(char *poly)
{
	/* check the integrity of poly hexstring */
	if (strlen(poly) != POLY_HEX_LEN) {
		fprintf(stderr, "The hexstring of input polynomial %s is not 8 characters long.\n", poly);
		exit(EXIT_FAILURE);
	}
	int index = 0;
	char *p = poly;
	while (*p != '\0') {
		if (!(*p >= '0' && *p <= '9') && !(*p >= 'a' && *p <= 'f')) {
			fprintf(stderr, "The hexstring of input polynomial %s contains invalid character \'%c\' at index %d.\n", poly, *p, index);
			exit(EXIT_FAILURE);
		}
		p++;
		index++;		
	}
	
	/* create input, output arrays */
	int i;
	p = poly;	
	unsigned char a[POLY_LEN]; /* poly=03010102, in={02}{01}{01}{03} */
	for (i = POLY_LEN - 1; i >= 0; i--) {
		a[i] = parseHexstring(p);
		p += 2;
	}
	unsigned char aInv[POLY_LEN]; /* poly_inv=0b0d090e, out={01}{00}{00}{00} */
	for (i = 0; i < POLY_LEN; i++) aInv[i] = 0;
	
	/* calculate multiplicative inverse and print */
	calculateInv(a, aInv);
	
}	

void calculateInv(unsigned char *a, unsigned char *aInv)
{
	/* hard-coded multiplicative inverse of {00} through {ff} */
	unsigned char invByte[] = {
		0x00, 0x01, 0x8d, 0xf6, 0xcb, 0x52, 0x7b, 0xd1, 0xe8, 0x4f, 0x29, 0xc0, 0xb0, 0xe1, 0xe5, 0xc7,
		0x74, 0xb4, 0xaa, 0x4b, 0x99, 0x2b, 0x60, 0x5f, 0x58, 0x3f, 0xfd, 0xcc, 0xff, 0x40, 0xee, 0xb2,
		0x3a, 0x6e, 0x5a, 0xf1, 0x55, 0x4d, 0xa8, 0xc9, 0xc1, 0x0a, 0x98, 0x15, 0x30, 0x44, 0xa2, 0xc2,
		0x2c, 0x45, 0x92, 0x6c, 0xf3, 0x39, 0x66, 0x42, 0xf2, 0x35, 0x20, 0x6f, 0x77, 0xbb, 0x59, 0x19,
		0x1d, 0xfe, 0x37, 0x67, 0x2d, 0x31, 0xf5, 0x69, 0xa7, 0x64, 0xab, 0x13, 0x54, 0x25, 0xe9, 0x09,
		0xed, 0x5c, 0x05, 0xca, 0x4c, 0x24, 0x87, 0xbf, 0x18, 0x3e, 0x22, 0xf0, 0x51, 0xec, 0x61, 0x17,
		0x16, 0x5e, 0xaf, 0xd3, 0x49, 0xa6, 0x36, 0x43, 0xf4, 0x47, 0x91, 0xdf, 0x33, 0x93, 0x21, 0x3b,
		0x79, 0xb7, 0x97, 0x85, 0x10, 0xb5, 0xba, 0x3c, 0xb6, 0x70, 0xd0, 0x06, 0xa1, 0xfa, 0x81, 0x82,
		0x83, 0x7e, 0x7f, 0x80, 0x96, 0x73, 0xbe, 0x56, 0x9b, 0x9e, 0x95, 0xd9, 0xf7, 0x02, 0xb9, 0xa4,
		0xde, 0x6a, 0x32, 0x6d, 0xd8, 0x8a, 0x84, 0x72, 0x2a, 0x14, 0x9f, 0x88, 0xf9, 0xdc, 0x89, 0x9a,
		0xfb, 0x7c, 0x2e, 0xc3, 0x8f, 0xb8, 0x65, 0x48, 0x26, 0xc8, 0x12, 0x4a, 0xce, 0xe7, 0xd2, 0x62,
		0x0c, 0xe0, 0x1f, 0xef, 0x11, 0x75, 0x78, 0x71, 0xa5, 0x8e, 0x76, 0x3d, 0xbd, 0xbc, 0x86, 0x57,
		0x0b, 0x28, 0x2f, 0xa3, 0xda, 0xd4, 0xe4, 0x0f, 0xa9, 0x27, 0x53, 0x04, 0x1b, 0xfc, 0xac, 0xe6,
		0x7a, 0x07, 0xae, 0x63, 0xc5, 0xdb, 0xe2, 0xea, 0x94, 0x8b, 0xc4, 0xd5, 0x9d, 0xf8, 0x90, 0x6b,
		0xb1, 0x0d, 0xd6, 0xeb, 0xc6, 0x0e, 0xcf, 0xad, 0x08, 0x4e, 0xd7, 0xe3, 0x5d, 0x50, 0x1e, 0xb3,
		0x5b, 0x23, 0x38, 0x34, 0x68, 0x46, 0x03, 0x8c, 0xdd, 0x9c, 0x7d, 0xa0, 0xcd, 0x1a, 0x41, 0x1c
	};

	/* create rem, quo, and aux tables */
	unsigned char rem[TAB_ROWS][TAB_COLS];
	unsigned char quo[TAB_ROWS][TAB_COLS];
	unsigned char aux[TAB_ROWS][TAB_COLS];
	int i, j;
	for (i = 0; i < TAB_ROWS; i++)
		for (j = 0; j < TAB_COLS; j++)
			rem[i][j] = quo[i][j] = aux[i][j] = 0;
	
	/* initialize the first and second rows */
	rem[0][0] = 0x01;
	for (i = 0; i < POLY_LEN; i++) rem[1][i] = a[i];
	aux[1][0] = 0x01;

	/* print the first and second rows */
	for (i = 0; i < 2; i++) {
		fprintf(stdout, "i=%d, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={%02x}{%02x}{%02x}{%02x}, aux[i]={%02x}{%02x}{%02x}{%02x}\n",
				i + 1, rem[i][3], rem[i][2], rem[i][1], rem[i][0], quo[i][3], quo[i][2], quo[i][1], quo[i][0],
				aux[i][3], aux[i][2], aux[i][1], aux[i][0]);
	}	
	
	/* Handle special case when a(x) is zero-order */
	if (rem[1][0] && !rem[1][1] && !rem[1][2] && !rem[1][3]) {
		rem[2][0] = 0x01;
		quo[2][0] = invByte[rem[1][0]];
		aux[2][0] = invByte[rem[1][0]];

		fprintf(stdout, "i=%d, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={%02x}{%02x}{%02x}{%02x}, aux[i]={%02x}{%02x}{%02x}{%02x}\n",
		3, rem[2][3], rem[2][2], rem[2][1], rem[2][0], quo[2][3], quo[2][2], quo[2][1], quo[2][0],
		aux[2][3], aux[2][2], aux[2][1], aux[2][0]);

		fprintf(stdout, "Multiplicative inverse of {%02x}{%02x}{%02x}{%02x} is {%02x}{%02x}{%02x}{%02x}.\n",
		rem[1][3], rem[1][2], rem[1][1], rem[1][0], aux[2][3], aux[2][2], aux[2][1], aux[2][0]);
		return;
	}
	
	/* Table method */
	int k;
	int degB, degA, degQ; /* degree of b(x), a(x), and q(x) where b(x) = a(x) * q(x) + rem(x) */
	int isFinal = 0; /* flag to indicate the final round */
	
	for (i = 2; i < MAX_ROUND && !isFinal; i++) {
		degB = (i == 2) ? 4 : getDegree(rem, i - 2); /* degree of rem[i-2] */
		degA = getDegree(rem, i - 1); /* degree of rem[i-1] */
		if (degA == 0) isFinal = 1; /* if zero order, then this is the final round */
		degQ = degB - degA; /* degree of quo[i] */
		
		/* quo[i] */
		for (j = degQ; j >= 0; j--) {			
			/* calculate quo[i][j] */
			if (i == 2) { /* rem[2] need special handle because b(x) = {01}{00}{00}{00}{01} */
				quo[i][j] = (j == degQ) ? invByte[rem[i - 1][degA]] : bigdot(invByte[rem[i - 1][degA]], rem[i][degA]);	
			} else {
				quo[i][j] = (j == degQ) ? bigdot(invByte[rem[i - 1][degA]], rem[i - 2][degB]) : 
										  (isFinal && j == 0) ? bigdot(invByte[rem[i - 1][degA]], rem[i][degA] ^ 0x01) : 
										  						bigdot(invByte[rem[i - 1][degA]], rem[i][degA]);
			}
			
			/* use rem[i] to put the intermidiate result */
			for (k = 0; k <= degA; k++) { 
				rem[i][k] = (j == degQ) ? bigdot(quo[i][j], rem[i - 1][k]) ^ rem[i - 2][k + j] : 
										  bigdot(quo[i][j], rem[i - 1][k]) ^ rem[i][k];
			}
			
			/* remove the most significant byte and append the last significant byte */
			if (j > 0) {
				for (k = degA; k > 0; k--) rem[i][k] = rem[i][k - 1];
				rem[i][0] = rem[i - 2][j - 1];	
			}			
		}
	
		/* calculate aux[i] */
		aux[i][0] = bigdot(aux[i-1][0], quo[i][0]) ^ bigdot(aux[i-1][3], quo[i][1]) ^ bigdot(aux[i-1][2], quo[i][2]) ^ bigdot(aux[i-1][1], quo[i][3]);
		aux[i][1] = bigdot(aux[i-1][1], quo[i][0]) ^ bigdot(aux[i-1][0], quo[i][1]) ^ bigdot(aux[i-1][3], quo[i][2]) ^ bigdot(aux[i-1][2], quo[i][3]);
		aux[i][2] = bigdot(aux[i-1][2], quo[i][0]) ^ bigdot(aux[i-1][1], quo[i][1]) ^ bigdot(aux[i-1][0], quo[i][2]) ^ bigdot(aux[i-1][3], quo[i][3]);
		aux[i][3] = bigdot(aux[i-1][3], quo[i][0]) ^ bigdot(aux[i-1][2], quo[i][1]) ^ bigdot(aux[i-1][1], quo[i][2]) ^ bigdot(aux[i-1][0], quo[i][3]);
		for (j = 0; j < TAB_COLS; j++) aux[i][j] ^= aux[i - 2][j];

		/* print out imtermidiate results */
		fprintf(stdout, "i=%d, rem[i]={%02x}{%02x}{%02x}{%02x}, quo[i]={%02x}{%02x}{%02x}{%02x}, aux[i]={%02x}{%02x}{%02x}{%02x}\n",
		i + 1, rem[i][3], rem[i][2], rem[i][1], rem[i][0], quo[i][3], quo[i][2], quo[i][1], quo[i][0],
		aux[i][3], aux[i][2], aux[i][1], aux[i][0]);
		
		/* check {00}{00}{00}{00} */
		if (!rem[i][0] && !rem[i][1] && !rem[i][2] && !rem[i][3]) {
			fprintf(stdout, "{%02x}{%02x}{%02x}{%02x} does not have a multiplicative inverse.\n", rem[1][3], rem[1][2], rem[1][1], rem[1][0]);
			return;
		}
	}

	/* print the final result */
	fprintf(stdout, "Multiplicative inverse of {%02x}{%02x}{%02x}{%02x} is {%02x}{%02x}{%02x}{%02x}\n",
	rem[1][3], rem[1][2], rem[1][1], rem[1][0], aux[i - 1][3], aux[i - 1][2], aux[i - 1][1], aux[i - 1][0]);
}

int getDegree(unsigned char rem[TAB_ROWS][TAB_COLS], int row)
{
	int deg = 3;
	while (rem[row][deg] == 0) deg--;
	return deg;
}
