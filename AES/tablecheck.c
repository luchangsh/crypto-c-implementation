#include "tablecheck.h"

void tablecheck(FILE *tfp)
{
	int i; /* index */
	
	/* declare tables */
	unsigned char S[S_LEN];
	for (i = 0; i < S_LEN; i++) S[i] = 0;
	unsigned char INVS[S_LEN];
	for (i = 0; i < INVS_LEN; i++) INVS[i] = 0;
	unsigned char P[P_LEN];
	for (i = 0; i < P_LEN; i++) P[i] = 0;
	unsigned char INVP[INVP_LEN];
	for (i = 0; i < INVP_LEN; i++) INVP[i] = 0;

	/* build tables */	
	buildTables(tfp, S, INVS, P, INVP);	
}

void buildTables(FILE *tfp, unsigned char *S, unsigned char *INVS,
				 unsigned char *P, unsigned char *INVP)
{
	int i;
	
	/* A boolean array to track if all tables needed are specified in the tablefile */
	unsigned char found[NUM_TABLES]; /* [S, P, INVP] */
	for (i = 0; i < NUM_TABLES; i++) found[i] = 0;
	
	char tablefileBuf[TF_BUF_SIZE]; /* buffer for reading the tablefile */
	for (i = 0; i < TF_BUF_SIZE; i++) tablefileBuf[i] = 0;
	
	char *val = NULL;
	int numberOfLine = 0;
	while (fgets(tablefileBuf, TF_BUF_SIZE, tfp) != NULL)	{
		numberOfLine++;
		
		/* parse the key=value string */
		val = parseOption(tablefileBuf);
		if (val == NULL) {
			fprintf(stderr, "Malformed tablefile, line %d contains no \"=\".\n", numberOfLine);
			exit(EXIT_FAILURE);
		}
		if (*val == '\0') {
			fprintf(stderr, "Malformed tablefile, line %d contains no value.\n", numberOfLine);
			exit(EXIT_FAILURE);
		}
		*(val + strlen(val) - 1) = '\0'; /* replace the ending '\n' with '\0' */

		/* check if the value is a hexstring */
		int pos = 0;
		char *p = val;
		for (; *p != '\0'; p++, pos++) {
			if (!(*p >= '0' && *p <= '9') && !(*p >= 'a' && *p <= 'f')) {
				fprintf(stderr, "Value of table %s contains invalid character \'%c\' at index %d.\n", tablefileBuf, *p, pos);
				exit(EXIT_FAILURE);
			}
		}

		/* build tables based on the key */
		if (strcmp(tablefileBuf, "S") == 0) {
			if (found[0]) {
				fprintf(stderr, "Table %s has already been specified.\n", tablefileBuf);
				exit(EXIT_FAILURE);
			}
			found[0] = 1;
			if (strlen(val) != S_HEX_LEN) {
				fprintf(stderr, "The hexstring of table %s is not 512 characters long.\n", tablefileBuf);
				exit(EXIT_FAILURE);
			}			
			buildSandINVSTable(S, INVS, tablefileBuf, val);
		} else if (strcmp(tablefileBuf, "P") == 0) {
			if (found[1]) {
				fprintf(stderr, "Table %s has already been specified.\n", tablefileBuf);
				exit(EXIT_FAILURE);
			}
			found[1] = 1;
			if (strlen(val) != P_HEX_LEN) {
				fprintf(stderr, "The hexstring of table %s is not 8 characters long.\n", tablefileBuf);
				exit(EXIT_FAILURE);
			}			
			buildPTable(P, tablefileBuf, val);
		} else if (strcmp(tablefileBuf, "INVP") == 0) {
			if (found[2]) {
				fprintf(stderr, "Table %s has already been specified.\n", tablefileBuf);
				exit(EXIT_FAILURE);
			}
			found[2] = 1;
			if (strlen(val) != INVP_HEX_LEN) {
				fprintf(stderr, "The hexstring of table %s is not 8 characters long.\n", tablefileBuf);
				exit(EXIT_FAILURE);
			}
			buildINVPTable(INVP, tablefileBuf, val);
		} else {
			fprintf(stderr, "Line %d contains unknown key %s.\n", numberOfLine, tablefileBuf);
			exit(EXIT_FAILURE);
		}

		/* reset buffer */
		for (i = 0; i < TF_BUF_SIZE; i++) tablefileBuf[i] = 0;
	}
	
	/* check if all tables are specified */
	if (numberOfLine != NUM_TABLES) {
		fprintf(stderr, "Tablefile contains only %d lines.\n", numberOfLine);
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < NUM_TABLES; i++) {
		if (found[i] == 0) {
			fprintf(stderr, "Some tables are not specified in the tablefile.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/* check if the product of P and INVP is {00}{00}{00}{01} */
	unsigned char out[P_LEN];
	for (i = 0; i < P_LEN; i++) out[i] = 0;

	circlex(P, INVP, out);

	if (out[0] != 1) {
		fprintf(stderr, "Malformed tablefile: the product of P and INVP is not {00}{00}{00}{01}.\n");
		exit(EXIT_FAILURE);		
	}
	for (i = 1; i < P_LEN; i++) {
		if (out[i] != 0) {
			fprintf(stderr, "Malformed tablefile: the product of P and INVP is not {00}{00}{00}{01}.\n");
			exit(EXIT_FAILURE);		
		}
	}
}

void buildSandINVSTable(unsigned char *S, unsigned char *INVS, char *key, char *val)
{
	int i;
	unsigned char num;
	unsigned char counts[S_LEN];
	for (i = 0; i < S_LEN; i++) counts[i] = 0;	

	char *p = val;
	for (i = 0; i < S_LEN; i++) {
		num = parseHexstring(p);
		counts[num]++;
		if (counts[num] != 1) {
			fprintf(stderr, "The hexstring of table %s contains duplicated value %02x at index %d.\n", key, num, 2 * i);
			exit(EXIT_FAILURE);
		}
		S[i] = num;
		p += 2;	
	}

	/* build INVS */
	for (i = 0; i < INVS_LEN; i++) INVS[S[i]] = i;
}

void buildPTable(unsigned char *P, char *key, char *val)
{
	int i;
	char *p = val;
	for (i = P_LEN - 1; i >= 0; i--) { /* val=03010102, P={02}{01}{01}{03} */
		P[i] = parseHexstring(p);
		p += 2;
	}
}

void buildINVPTable(unsigned char *INVP, char *key, char *val)
{
	int i;
	char *p = val;
	for (i = INVP_LEN - 1; i >= 0; i--) { /* val=0b0d090e, INVP={0e}{09}{0d}{0b} */
		INVP[i] = parseHexstring(p);
		p += 2;
	}
}

unsigned char parseHexstring(char *p)
{
	char ch;
	unsigned char res = 0;
	int i;
	for (i = 0; i < 2; i++) {
		ch = *p++;
		switch (ch) {
			case '0': res |= 0x00; break;
			case '1': res |= 0x01; break;
			case '2': res |= 0x02; break;
			case '3': res |= 0x03; break;
			case '4': res |= 0x04; break;
			case '5': res |= 0x05; break;
			case '6': res |= 0x06; break;
			case '7': res |= 0x07; break;
			case '8': res |= 0x08; break;
			case '9': res |= 0x09; break;
			case 'a': res |= 0x0a; break;
			case 'b': res |= 0x0b; break;
			case 'c': res |= 0x0c; break;
			case 'd': res |= 0x0d; break;
			case 'e': res |= 0x0e; break;
			case 'f': res |= 0x0f; break;			
		}
		if (i == 0) res <<= 4;
	}
	return res;
}

