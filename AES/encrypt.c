#include "encrypt.h"

void encrypt(char *key, FILE *tfp, FILE *ifp)
{
	int i, j; /* index */
	
	/* build tables */
	unsigned char S[S_LEN];
	for (i = 0; i < S_LEN; i++) S[i] = 0;
	unsigned char INVS[S_LEN];
	for (i = 0; i < INVS_LEN; i++) INVS[i] = 0;
	unsigned char P[P_LEN];
	for (i = 0; i < P_LEN; i++) P[i] = 0;
	unsigned char INVP[INVP_LEN];
	for (i = 0; i < INVP_LEN; i++) INVP[i] = 0;
	buildTables(tfp, S, INVS, P, INVP);	

	/* build key array */	
	unsigned char keyArr[KEY_LEN];
	for (i = 0; i < KEY_LEN; i++) keyArr[i] = 0;
	buildKeyArray(key, keyArr);

	/* key expansion */
	unsigned char wordArr[WRD_ROWS][WRD_COLS];	
	for (i = 0; i < WRD_ROWS; i++)
		for (j = 0; j < WRD_COLS; j++)
			wordArr[i][j] = 0;
	keyExpansion(keyArr, wordArr, S);

	/* read the input file */
	unsigned char inputBuf[IN_BUF_LEN];
	for (i = 0; i < IN_BUF_LEN; i++) inputBuf[i] = 0;
	
	size_t bytes = 0;	
	if ((bytes = fread(inputBuf, sizeof(unsigned char), IN_BUF_LEN, ifp)) != IN_BUF_LEN) {
		fprintf(stderr, "The input file contains only %d bytes, not 16 bytes.\n", bytes);
		exit(EXIT_FAILURE);		
	}
	
	/* create internal state matrix */
	unsigned char state[STAT_ROWS][STAT_COLS];
	for (i = 0; i < STAT_ROWS; i++)
		for (j = 0; j < STAT_COLS; j++)
			state[i][j] = 0;
	
	/* AES encrypt */
	buildState(state, inputBuf); /* state = in */
	fprintf(stdout, "round[%2d].input    ", 0);
	printState(state);

	addRoundKey(state, wordArr, 0); /* addRoundKey */
	fprintf(stdout, "round[%2d].k_sch    ", 0);
	printKeySchedule(wordArr, 0);

	for (i = 1; i < NR; i++) {
		fprintf(stdout, "round[%2d].start    ", i);
		printState(state);

		subBytes(state, S); /* subBytes */
		fprintf(stdout, "round[%2d].s_box    ", i);
		printState(state);

		shiftRows(state); /* shiftRows */
		fprintf(stdout, "round[%2d].s_row    ", i);
		printState(state);

		mixColumns(state, P); /* mixColumns */
		fprintf(stdout, "round[%2d].m_col    ", i);
		printState(state);

		addRoundKey(state, wordArr, i * NB); /* addRoundKey */
		fprintf(stdout, "round[%2d].k_sch    ", i);
		printKeySchedule(wordArr, i * NB);
	}
	fprintf(stdout, "round[%2d].start    ", i);
	printState(state);

	subBytes(state, S); /* subBytes */
	fprintf(stdout, "round[%2d].s_box    ", i);
	printState(state);

	shiftRows(state); /* shiftRows */
	fprintf(stdout, "round[%2d].s_row    ", i);
	printState(state);

	addRoundKey(state, wordArr, i * NB); /* addRoundKey */
	fprintf(stdout, "round[%2d].k_sch    ", i);

	printKeySchedule(wordArr, i * NB); /* out = state */
	fprintf(stdout, "round[%2d].output   ", i);
	printState(state);
}

void buildState(unsigned char state[STAT_ROWS][STAT_COLS], unsigned char *inputBuf)
{
	int i, j;
	for (j = 0; j < STAT_COLS; j++)
		for (i = 0; i < STAT_ROWS; i++)
			state[i][j] = inputBuf[j * STAT_ROWS + i];
}

void addRoundKey(unsigned char state[STAT_ROWS][STAT_COLS],
				 unsigned char wordArr[WRD_ROWS][WRD_COLS], int fromRow)
{
	int i, j;
	for (j = 0; j < STAT_COLS; j++)
		for (i = 0; i < STAT_ROWS; i++)
			state[i][j] ^= wordArr[fromRow + j][i];	
}

void subBytes(unsigned char state[STAT_ROWS][STAT_COLS], unsigned char *S)
{
	int i, j;
	for (j = 0; j < STAT_COLS; j++)
		for (i = 0; i < STAT_ROWS; i++)
			state[i][j] = S[state[i][j]];	
}

void shiftRows(unsigned char state[STAT_ROWS][STAT_COLS])
{
	int i, j;
	for (i = 1; i < STAT_ROWS; i++)
		for (j = 0; j < i; j++)
			shiftOneLeft(state, i);
}

void shiftOneLeft(unsigned char state[STAT_ROWS][STAT_COLS], int row)
{
	unsigned char tmp = state[row][0];
	int i;
	for (i = 1; i < STAT_COLS; i++) state[row][i - 1] = state[row][i];
	state[row][STAT_COLS - 1] = tmp;
}

void mixColumns(unsigned char state[STAT_ROWS][STAT_COLS], unsigned char *P)
{
	int i, j;
	unsigned char in[STAT_ROWS];
	for (i = 0; i < STAT_ROWS; i++) in[i] = 0;
	unsigned char out[STAT_ROWS];
	for (i = 0; i < STAT_ROWS; i++) out[i] = 0;
	
	for (j = 0; j < STAT_COLS; j++) {
		for (i = 0; i < STAT_ROWS; i++) in[i] = state[i][j];
		circlex(P, in, out);
		for (i = 0; i < STAT_ROWS; i++) state[i][j] = out[i];	
	}
}

void printState(unsigned char state[STAT_ROWS][STAT_COLS])
{
	int i, j;
	for (j = 0; j < STAT_COLS; j++)
		for (i = 0; i < STAT_ROWS; i++)
			fprintf(stdout, "%02x", state[i][j]);
	fprintf(stdout, "\n");
}

void printKeySchedule(unsigned char wordArr[WRD_ROWS][WRD_COLS], int fromRow)
{
	int i, j;
	for (i = fromRow; i < fromRow + 4; i++)
		for (j = 0; j < WRD_COLS; j++)
			fprintf(stdout, "%02x", wordArr[i][j]);
	fprintf(stdout, "\n");
}


