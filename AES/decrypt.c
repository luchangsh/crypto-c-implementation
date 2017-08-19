#include "decrypt.h"

void decrypt(char *key, FILE *tfp, FILE *ifp)
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
	
	/* AES decrypt */
	buildState(state, inputBuf); /* state = in */
	fprintf(stdout, "round[%2d].iinput   ", 0);
	printState(state);

	addRoundKey(state, wordArr, NR * NB); /* addRoundKey */
	fprintf(stdout, "round[%2d].ik_sch   ", 0);
	printKeySchedule(wordArr, NR * NB);

	for (i = NR - 1; i > 0; i--) {
		fprintf(stdout, "round[%2d].istart   ", NR - i);
		printState(state);

		invShiftRows(state); /* invShiftRows */
		fprintf(stdout, "round[%2d].is_row   ", NR - i);
		printState(state);

		subBytes(state, INVS); /* invSubBytes */
		fprintf(stdout, "round[%2d].is_box   ", NR - i);
		printState(state);

		addRoundKey(state, wordArr, i * NB); /* addRoundKey */
		fprintf(stdout, "round[%2d].ik_sch   ", NR - i);
		printKeySchedule(wordArr, i * NB);

		fprintf(stdout, "round[%2d].ik_add   ", NR - i);
		printState(state);

		mixColumns(state, INVP); /* invMixColumns */
	}
	fprintf(stdout, "round[%2d].istart   ", NR - i);
	printState(state);

	invShiftRows(state); /* invShiftRows */
	fprintf(stdout, "round[%2d].is_row   ", NR - i);
	printState(state);

	subBytes(state, INVS); /* invSubBytes */
	fprintf(stdout, "round[%2d].is_box   ", NR - i);
	printState(state);

	addRoundKey(state, wordArr, i * NB); /* addRoundKey */
	fprintf(stdout, "round[%2d].ik_sch   ", NR - i);
	printKeySchedule(wordArr, i * NB);

	fprintf(stdout, "round[%2d].ioutput  ", NR - i); /* out = state */
	printState(state);
}

void invShiftRows(unsigned char state[STAT_ROWS][STAT_COLS])
{
	int i, j;
	for (i = 1; i < STAT_ROWS; i++)
		for (j = 0; j < i; j++)
			shiftOneRight(state, i);
}

void shiftOneRight(unsigned char state[STAT_ROWS][STAT_COLS], int row)
{
	unsigned char tmp = state[row][STAT_COLS - 1];
	int i;
	for (i = STAT_COLS - 1; i > 0; i--) state[row][i] = state[row][i - 1];
	state[row][0] = tmp;
}


