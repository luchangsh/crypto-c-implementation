#include "modprod.h"

void modprod(char *poly1, char *poly2)
{
	/* check the integrity of poly1 and poly2 */
	if (strlen(poly1) != P1_HEX_LEN) {
		fprintf(stderr, "The hexstring of p1 %s is not 8 characters long.\n", poly1);
		exit(EXIT_FAILURE);		
	}
	if (strlen(poly2) != P2_HEX_LEN) {
		fprintf(stderr, "The hexstring of p2 %s is not 8 characters long.\n", poly2);
		exit(EXIT_FAILURE);		
	}
	int index = 0;
	char *p = poly1;
	while (*p != '\0') {
		if (!(*p >= '0' && *p <= '9') && !(*p >= 'a' && *p <= 'f')) {
			fprintf(stderr, "The hexstring of p1 %s contains invalid character \'%c\' at index %d.\n", poly1, *p, index);
			exit(EXIT_FAILURE);
		}
		p++;
		index++;		
	}
	index = 0;
	p = poly2;
	while (*p != '\0') {
		if (!(*p >= '0' && *p <= '9') && !(*p >= 'a' && *p <= 'f')) {
			fprintf(stderr, "The hexstring of p2 %s contains invalid character \'%c\' at index %d.\n", poly2, *p, index);
			exit(EXIT_FAILURE);
		}
		p++;
		index++;		
	}
	
	/* create a, b, d arrays */
	int i;
	p = poly1;	
	unsigned char a[A_LEN]; /* poly1=03010102, a={02}{01}{01}{03} */
	for (i = A_LEN - 1; i >= 0; i--) {
		a[i] = parseHexstring(p);
		p += 2;
	}
	p = poly2;	
	unsigned char b[B_LEN]; /* poly2=0b0d090e, b={0e}{09}{0d}{0b} */
	for (i = B_LEN - 1; i >= 0; i--) {
		b[i] = parseHexstring(p);
		p += 2;
	}
	unsigned char d[D_LEN]; /* poly3=00000001, d={01}{00}{00}{00} */
	for (i = 0; i < D_LEN; i++) d[i] = 0;

	/* calculate d = circlex(a, b) */
	circlex(a, b, d);
	
	/* print out */
	fprintf(stdout, "{%02x}{%02x}{%02x}{%02x} CIRCLEX {%02x}{%02x}{%02x}{%02x} = {%02x}{%02x}{%02x}{%02x}\n",
			a[3], a[2], a[1], a[0], b[3], b[2], b[1], b[0], d[3], d[2], d[1], d[0]);
	
}

void circlex(unsigned char *a, unsigned char *b, unsigned char *d)
{
	d[0] = bigdot(a[0], b[0]) ^ bigdot(a[3], b[1]) ^ bigdot(a[2], b[2]) ^ bigdot(a[1], b[3]);
	d[1] = bigdot(a[1], b[0]) ^ bigdot(a[0], b[1]) ^ bigdot(a[3], b[2]) ^ bigdot(a[2], b[3]);
	d[2] = bigdot(a[2], b[0]) ^ bigdot(a[1], b[1]) ^ bigdot(a[0], b[2]) ^ bigdot(a[3], b[3]);
	d[3] = bigdot(a[3], b[0]) ^ bigdot(a[2], b[1]) ^ bigdot(a[1], b[2]) ^ bigdot(a[0], b[3]);	
}

unsigned char xtime(unsigned char b)
{
	return ((b >> 7) & 1) ? (b << 1) ^ 0x1b : b << 1;
}

unsigned char bigdot(unsigned char a, unsigned char b)
{
	unsigned char mask = b;
	unsigned char p = (mask & 1) ? a : 0;
	unsigned char v = a;
	
	int i;
	for (i = 1; i < 8; i++) {
		v = xtime(v);
		if ((mask >>= 1) & 1) p ^= v;
	}	
	return p;
}
