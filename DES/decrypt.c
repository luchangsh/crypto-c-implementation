#include "decrypt.h"

void decrypt(char *key, FILE *tfp, FILE *ifp)
{
	int i, j; /* indice */

	/* build tables */
	unsigned char IP[IP_LEN];
	for (i = 0; i < IP_LEN; i++) IP[i] = 0;
	unsigned char IP_INV[IP_LEN];
	for (i = 0; i < IP_LEN; i++) IP_INV[i] = 0;
	unsigned char E[E_LEN];
	for (i = 0; i < E_LEN; i++) E[i] = 0;
	unsigned char P[P_LEN];
	for (i = 0; i < P_LEN; i++) P[i] = 0;
	unsigned char S1[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S1[i][j] = 0;
	unsigned char S2[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S2[i][j] = 0;
	unsigned char S3[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S3[i][j] = 0;
	unsigned char S4[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S4[i][j] = 0;
	unsigned char S5[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S5[i][j] = 0;
	unsigned char S6[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S6[i][j] = 0;
	unsigned char S7[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S7[i][j] = 0;
	unsigned char S8[S_ROWS][S_COLS];
	for (i = 0; i < S_ROWS; i++)
		for (j = 0; j < S_COLS; j++)
			S8[i][j] = 0;
	unsigned char V[V_LEN];
	for (i = 0; i < V_LEN; i++) V[i] = 0;
	unsigned char PC1[PC1_LEN];
	for (i = 0; i < PC1_LEN; i++) PC1[i] = 0;
	unsigned char PC2[PC2_LEN];
	for (i = 0; i < PC2_LEN; i++) PC2[i] = 0;
	
	buildTables(tfp, IP, IP_INV, E, P, S1, S2, S3, S4, 
				S5, S6, S7, S8, V, PC1, PC2);
	
	/* build key */
	unsigned char keyBuf[KEY_BUF_SIZE];
	for (i = 0; i < KEY_BUF_SIZE; i++) keyBuf[i] = 0;
	buildKeyBuf(key, keyBuf);
	
	/* generate subkeys by key schedule algorithm */
	unsigned char subkeyBuf[NUM_ROUNDS][SUBKEY_BYTES];
	for (i = 0; i < NUM_ROUNDS; i++)
		for (j = 0; j < SUBKEY_BYTES; j++)
			subkeyBuf[i][j] = 0;
	generateSubkeys(keyBuf, V, PC1, PC2, subkeyBuf);
	
	/* reverse the order of subkeys in the subkeyBuf */
	reverseOrderOfSubkeys(subkeyBuf);	/* DES decryption only need to used subkeys in reversed order */
	
	/* create buffers */
	unsigned char inputBuf[IN_BUF_SIZE];
	for (i = 0; i < IN_BUF_SIZE; i++) inputBuf[i] = 0;
	unsigned char L[L_BYTES];
	for (i = 0; i < L_BYTES; i++) L[i] = 0;
	unsigned char R[R_BYTES];
	for (i = 0; i < R_BYTES; i++) R[i] = 0;
	unsigned char subkey[SUBKEY_BYTES];
	for (i = 0; i < SUBKEY_BYTES; i++) subkey[i] = 0;
	unsigned char T0[R_BYTES];
	for (i = 0; i < R_BYTES; i++) T0[i] = 0;
	unsigned char T1[SUBKEY_BYTES];
	for (i = 0; i < SUBKEY_BYTES; i++) T1[i] = 0;
	unsigned char T2[SUBKEY_BYTES];
	for (i = 0; i < SUBKEY_BYTES; i++) T2[i] = 0;
	unsigned char T3[T3_BYTES];
	for (i = 0; i < T3_BYTES; i++) T3[i] = 0;
	unsigned char T4[L_BYTES];
	for (i = 0; i < L_BYTES; i++) T4[i] = 0;
	unsigned char T5[L_BYTES];
	for (i = 0; i < L_BYTES; i++) T5[i] = 0;
	unsigned char rows[T3_BYTES];
	for (i = 0; i < T3_BYTES; i++) rows[i] = 0;
	unsigned char cols[T3_BYTES];
	for (i = 0; i < T3_BYTES; i++) cols[i] = 0;
	unsigned char outputBuf[OUT_BUF_SIZE];
	for (i = 0; i < OUT_BUF_SIZE; i++) outputBuf[i] = 0;

	size_t bytes = 0;
	int isFirst = 1;	

	while ((bytes = fread(inputBuf, sizeof(unsigned char), IN_BUF_SIZE, ifp)) > 0) {
		if (bytes < IN_BUF_SIZE) {
			for (i = bytes; i < IN_BUF_SIZE; i++) {
				inputBuf[i] = 0;
			}
		}
		
		des(inputBuf, outputBuf, L, R, subkey, T0, T1, T2, T3, T4, T5, rows, cols, isFirst, 
			subkeyBuf, IP, IP_INV, E, P, S1, S2, S3, S4, S5, S6, S7, S8, V, PC1, PC2);

		fwrite(outputBuf, sizeof(unsigned char), OUT_BUF_SIZE, stdout);
		
		/* reset buffers */
		for (i = 0; i < OUT_BUF_SIZE; i++) outputBuf[i] = 0;
		for (i = 0; i < IN_BUF_SIZE; i++) inputBuf[i] = 0;
		for (i = 0; i < L_BYTES; i++) L[i] = 0;
		for (i = 0; i < R_BYTES; i++) R[i] = 0;
		for (i = 0; i < SUBKEY_BYTES; i++) subkey[i] = 0;

		if (isFirst) isFirst = 0;
	}
}

void reverseOrderOfSubkeys(unsigned char subkeyBuf[NUM_ROUNDS][SUBKEY_BYTES])
{
	int i;
	for (i = 0; i < NUM_ROUNDS / 2; i++) swap(subkeyBuf, i, NUM_ROUNDS - 1 - i);
}

void swap(unsigned char subkeyBuf[NUM_ROUNDS][SUBKEY_BYTES], int i, int j)
{
	unsigned char tmp;
	int k;
	for (k = 0; k < SUBKEY_BYTES; k++) {
		tmp = subkeyBuf[i][k];
		subkeyBuf[i][k] = subkeyBuf[j][k];
		subkeyBuf[j][k] = tmp;
	}
}
