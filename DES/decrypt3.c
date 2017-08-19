#include "decrypt3.h"

void decrypt3(char *key3, FILE *tfp, FILE *ifp)
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

	/* divides key3 to three keys for 3 stages */
	if (strlen(key3) != 3 * KEY_LEN) {
		fprintf(stderr, "Malformed key, key3 is too short or too long\n");
		exit(EXIT_FAILURE);
	}

	char k1[KEY_LEN + 1];
	char k2[KEY_LEN + 1];
	char k3[KEY_LEN + 1];
	char *ptr = key3;

	strncpy(k1, ptr, KEY_LEN);
	k1[KEY_LEN] = '\0';
	ptr += KEY_LEN;
	strncpy(k2, ptr, KEY_LEN);
	k2[KEY_LEN] = '\0';
	ptr += KEY_LEN;
	strncpy(k3, ptr, KEY_LEN);
	k3[KEY_LEN] = '\0';

	/* convert hex string of keys to bytes arrays */
	unsigned char key1Buf[KEY_BUF_SIZE];
	for (i = 0; i < KEY_BUF_SIZE; i++) key1Buf[i] = 0;
	unsigned char key2Buf[KEY_BUF_SIZE];
	for (i = 0; i < KEY_BUF_SIZE; i++) key2Buf[i] = 0;
	unsigned char key3Buf[KEY_BUF_SIZE];
	for (i = 0; i < KEY_BUF_SIZE; i++) key3Buf[i] = 0;
	
	buildKeyBuf(k1, key1Buf);
	buildKeyBuf(k2, key2Buf);
	buildKeyBuf(k3, key3Buf);
	
	/* generate subkeys by key schedule algorithm */
	unsigned char subkey1Buf[NUM_ROUNDS][SUBKEY_BYTES];
	for (i = 0; i < NUM_ROUNDS; i++)
		for (j = 0; j < SUBKEY_BYTES; j++)
			subkey1Buf[i][j] = 0;
	unsigned char subkey2Buf[NUM_ROUNDS][SUBKEY_BYTES];
	for (i = 0; i < NUM_ROUNDS; i++)
		for (j = 0; j < SUBKEY_BYTES; j++)
			subkey2Buf[i][j] = 0;
	unsigned char subkey3Buf[NUM_ROUNDS][SUBKEY_BYTES];
	for (i = 0; i < NUM_ROUNDS; i++)
		for (j = 0; j < SUBKEY_BYTES; j++)
			subkey3Buf[i][j] = 0;

	generateSubkeys(key1Buf, V, PC1, PC2, subkey1Buf);
	generateSubkeys(key2Buf, V, PC1, PC2, subkey2Buf);
	generateSubkeys(key3Buf, V, PC1, PC2, subkey3Buf);
	

	/* reverse the order of subkeys in the subkey1Buf and subkey3Buf*/
	reverseOrderOfSubkeys(subkey1Buf);	
	reverseOrderOfSubkeys(subkey3Buf);	

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
	unsigned char output1Buf[OUT_BUF_SIZE];
	for (i = 0; i < OUT_BUF_SIZE; i++) output1Buf[i] = 0;
	unsigned char output2Buf[OUT_BUF_SIZE];
	for (i = 0; i < OUT_BUF_SIZE; i++) output2Buf[i] = 0;
	unsigned char output3Buf[OUT_BUF_SIZE];
	for (i = 0; i < OUT_BUF_SIZE; i++) output3Buf[i] = 0;

	size_t bytes = 0;
	int isFirst = 1;	

	while ((bytes = fread(inputBuf, sizeof(unsigned char), IN_BUF_SIZE, ifp)) > 0) {
		if (bytes < IN_BUF_SIZE) {
			for (i = bytes; i < IN_BUF_SIZE; i++) {
				inputBuf[i] = 0;
			}
		}
		
		/* decrypt with key 3 */
		des(inputBuf, output1Buf, L, R, subkey, T0, T1, T2, T3, T4, T5, rows, cols, isFirst, 
			subkey3Buf, IP, IP_INV, E, P, S1, S2, S3, S4, S5, S6, S7, S8, V, PC1, PC2);
		for (i = 0; i < L_BYTES; i++) L[i] = 0;
		for (i = 0; i < R_BYTES; i++) R[i] = 0;
		for (i = 0; i < SUBKEY_BYTES; i++) subkey[i] = 0;

		/* encrypt with key 2 */		
		des(output1Buf, output2Buf, L, R, subkey, T0, T1, T2, T3, T4, T5, rows, cols, isFirst, 
			subkey2Buf, IP, IP_INV, E, P, S1, S2, S3, S4, S5, S6, S7, S8, V, PC1, PC2);
		for (i = 0; i < L_BYTES; i++) L[i] = 0;
		for (i = 0; i < R_BYTES; i++) R[i] = 0;
		for (i = 0; i < SUBKEY_BYTES; i++) subkey[i] = 0;
		
		/* decrypt with key 1 */
		des(output2Buf, output3Buf, L, R, subkey, T0, T1, T2, T3, T4, T5, rows, cols, isFirst, 
			subkey1Buf, IP, IP_INV, E, P, S1, S2, S3, S4, S5, S6, S7, S8, V, PC1, PC2);
		for (i = 0; i < L_BYTES; i++) L[i] = 0;
		for (i = 0; i < R_BYTES; i++) R[i] = 0;
		for (i = 0; i < SUBKEY_BYTES; i++) subkey[i] = 0;

		/* out put binary data to stdout */		
		fwrite(output3Buf, sizeof(unsigned char), OUT_BUF_SIZE, stdout);		

		/* reset buffers */
		for (i = 0; i < IN_BUF_SIZE; i++) inputBuf[i] = 0;
		for (i = 0; i < OUT_BUF_SIZE; i++) output1Buf[i] = 0;
		for (i = 0; i < OUT_BUF_SIZE; i++) output2Buf[i] = 0;
		for (i = 0; i < OUT_BUF_SIZE; i++) output3Buf[i] = 0;

		/* only output L, R for the first 8 bytes */
		if (isFirst) isFirst = 0;
	}
}
