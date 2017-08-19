#include "encrypt.h"

void encrypt(char *key, FILE *tfp, FILE *ifp)
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
	
	/* Create various buffers */
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

		/* only print L, R for first 8 bytes */
		if (isFirst) isFirst = 0;
	}
}

void buildKeyBuf(char *key, unsigned char *keyBuf)
{
	if (strlen(key) != KEY_LEN) {
		fprintf(stderr, "Malformed key, key is too short or too long\n");
		exit(EXIT_FAILURE);
	}
	
	int i;
	char ch;
	for (i = 0; i < KEY_LEN; i++) {
		ch = key[i];
		if (!(ch - '0' >= 0 && ch - '0' <= 9) && !(ch - 'a' >= 0 && ch - 'a' <= 5)) {
			fprintf(stderr, "Malformed key, %c is not a valid character in the key\n", ch);
			exit(EXIT_FAILURE);
		}
	}
	
	for (i = 0; i < KEY_LEN; i++) {
		ch = key[i];
		switch (ch)	{
			case '0': keyBuf[i / 2] |= 0x00; break;
			case '1': keyBuf[i / 2] |= 0x01; break;
			case '2': keyBuf[i / 2] |= 0x02; break;
			case '3': keyBuf[i / 2] |= 0x03; break;
			case '4': keyBuf[i / 2] |= 0x04; break;
			case '5': keyBuf[i / 2] |= 0x05; break;
			case '6': keyBuf[i / 2] |= 0x06; break;
			case '7': keyBuf[i / 2] |= 0x07; break;
			case '8': keyBuf[i / 2] |= 0x08; break;
			case '9': keyBuf[i / 2] |= 0x09; break;
			case 'a': keyBuf[i / 2] |= 0x0a; break;
			case 'b': keyBuf[i / 2] |= 0x0b; break;
			case 'c': keyBuf[i / 2] |= 0x0c; break;
			case 'd': keyBuf[i / 2] |= 0x0d; break;
			case 'e': keyBuf[i / 2] |= 0x0e; break;
			case 'f': keyBuf[i / 2] |= 0x0f; break;
		}
		if (i % 2 == 0) keyBuf[i / 2] <<= 4;
	}
}

void generateSubkeys(unsigned char *keyBuf, unsigned char *V, unsigned char *PC1,
					 unsigned char *PC2, unsigned char subkeyBuf[NUM_ROUNDS][SUBKEY_BYTES])
{
	int i;
	unsigned char C[C_LEN];
	for (i = 0; i < C_LEN; i++) C[i] = 0;
	unsigned char D[D_LEN];
	for (i = 0; i < D_LEN; i++) D[i] = 0;
	
	int num;
	unsigned char mask;
	
	for (i = 0; i < PC1_LEN; i++) {
		num = PC1[i] - 1;
		mask = (keyBuf[num / 8] >> (7 - num % 8)) & 0x01;
		if (i < 4) C[0] |= (mask << (3 - i));
		else if (i < 12) C[1] |= (mask << (7 - (i - 4)));
		else if (i < 20) C[2] |= (mask << (7 - (i - 12)));
		else if (i < 28) C[3] |= (mask << (7 - (i - 20)));
		else if (i < 32) D[0] |= (mask << (3 - (i - 28)));
		else if (i < 40) D[1] |= (mask << (7 - (i - 32)));
		else if (i < 48) D[2] |= (mask << (7 - (i - 40)));
		else D[3] |= (mask << (7 - (i - 48)));
	}
	
	/* print C0,D0 */
	fprintf(stderr, "(C0,D0)=%1x", C[0]);
	for (i = 1; i < C_LEN; i++) fprintf(stderr, "%02x", C[i]);
	fprintf(stderr, "%1x", D[0]);
	for (i = 1; i < D_LEN; i++) fprintf(stderr, "%02x", D[i]);
	fprintf(stderr, "\n");
	
	int j;
	for (j = 0; j < NUM_ROUNDS; j++) {
		/* shift C and D base V table */
		if (V[j] == 1) { /* left circular shift 1 bit */
			leftShift1(C);
			leftShift1(D);
		} else { /* left circular shift 2 bits */
			leftShift2(C);
			leftShift2(D);
		}
		
		/* print (C,D) */
		fprintf(stderr, "(C%d,D%d)=%1x", j+1, j+1, C[0]);
		for (i = 1; i < C_LEN; i++) fprintf(stderr, "%02x", C[i]);
		fprintf(stderr, "%1x", D[0]);
		for (i = 1; i < D_LEN; i++) fprintf(stderr, "%02x", D[i]);
		fprintf(stderr, "\n");
		
		/* generate one subkey */
		for (i = 0; i < PC2_LEN; i++) {
			num = PC2[i] - 1;
			if (num < 4) mask = (C[0] >> (3 - num)) & 0x01;
			else if (num < 12) mask = (C[1] >> (7 - (num - 4))) & 0x01;
			else if (num < 20) mask = (C[2] >> (7 - (num - 12))) & 0x01;
			else if (num < 28) mask = (C[3] >> (7 - (num - 20))) & 0x01;
			else if (num < 32) mask = (D[0] >> (3 - (num - 28))) & 0x01;
			else if (num < 40) mask = (D[1] >> (7 - (num - 32))) & 0x01;
			else if (num < 48) mask = (D[2] >> (7 - (num - 40))) & 0x01;
			else mask = (D[3] >> (7 - (num - 48))) & 0x01;
			subkeyBuf[j][i / 8] |= (mask << (7 - i % 8));
		}
		
		/* print subkey */
		fprintf(stderr, "k%d=", j+1);
		for (i = 0; i < SUBKEY_BYTES; i++) fprintf(stderr, "%02x", subkeyBuf[j][i]);
		fprintf(stderr, "\n");
	}	
}

void leftShift2(unsigned char *arr)
{
	int i;
	for (i = 0; i < 3; i++) {
		arr[i] <<= 2;
		arr[i] |= (arr[i + 1] >> 6) & 0x03;
	}
	arr[3] <<= 2;
	arr[3] |= (arr[0] >> 4) & 0x03;
	arr[0] &= 0x0f;
}

void leftShift1(unsigned char *arr)
{
	int i;
	for (i = 0; i < 3; i++) {
		arr[i] <<= 1;
		arr[i] |= (arr[i + 1] >> 7) & 0x01;
	}
	arr[3] <<= 1;
	arr[3] |= (arr[0] >> 4) & 0x01;
	arr[0] &= 0x0f;
}

void des(unsigned char *inputBuf, unsigned char *outputBuf,
		 unsigned char *L, unsigned char *R, unsigned char *subkey,
		 unsigned char *T0, unsigned char *T1, unsigned char *T2,
		 unsigned char *T3, unsigned char *T4, unsigned char *T5,
		 unsigned char *rows, unsigned char *cols, int isFirst,
		 unsigned char subkeyBuf[NUM_ROUNDS][SUBKEY_BYTES],
		 unsigned char *IP, unsigned char *IP_INV,
		 unsigned char *E, unsigned char *P,
		 unsigned char S1[S_ROWS][S_COLS], unsigned char S2[S_ROWS][S_COLS], 
		 unsigned char S3[S_ROWS][S_COLS], unsigned char S4[S_ROWS][S_COLS], 
		 unsigned char S5[S_ROWS][S_COLS], unsigned char S6[S_ROWS][S_COLS],
		 unsigned char S7[S_ROWS][S_COLS], unsigned char S8[S_ROWS][S_COLS], 
		 unsigned char *V, unsigned char *PC1, unsigned char *PC2)
{
		int i, j, tmp;
		
		/* Initial Permutation */
		initialPermutation(inputBuf, L, R, IP);
		
		if (isFirst) {
			fprintf(stderr, "(L0,R0)=");
			for (i = 0; i < L_BYTES; i++) fprintf(stderr, "%02x", L[i]);
			for (i = 0; i < R_BYTES; i++) fprintf(stderr, "%02x", R[i]);
			fprintf(stderr, "\n");
		}
		
		/* 16 rounds diffusion and confusions */
		for (j = 0; j < NUM_ROUNDS; j++) {
			for (i = 0; i < SUBKEY_BYTES; i++) subkey[i] = subkeyBuf[j][i]; /* get subkey */
			for (i = 0; i < R_BYTES; i++) T0[i] = R[i]; /* T0 store the original R */
		
			/* internal f function starts here */
			expand(T1, R, E); /* T1 stores the expended R */
			
			for (i = 0; i < SUBKEY_BYTES; i++) T2[i] = subkey[i] ^ T1[i]; /*xor with subkey */
			
			/* substitution */
			T3[0] = T2[0] >> 2;
			T3[1] = ((T2[0] & 0x03) << 4) | ((T2[1] & 0xf0) >> 4);
			T3[2] = ((T2[1] & 0x0f) << 2) | ((T2[2] & 0xc0) >> 6);
			T3[3] = T2[2] & 0x3f;
			T3[4] = T2[3] >> 2;
			T3[5] = ((T2[3] & 0x03) << 4) | ((T2[4] & 0xf0) >> 4);
			T3[6] = ((T2[4] & 0x0f) << 2) | ((T2[5] & 0xc0) >> 6);
			T3[7] = T2[5] & 0x3f;
			
			/* determine rows and cols for S1~S8 tables, store in rows[] and cols[] */
			for (i = 0; i < T3_BYTES; i++) {
				tmp = T3[i] & 0x21;
				if (tmp == 0x00) rows[i] = 0;
				else if (tmp == 0x01) rows[i] = 1;
				else if (tmp == 0x20) rows[i] = 2;
				else  rows[i] = 3;
				cols[i] = (T3[i] & 0x1e) >> 1;
			}
			
			/* T4 stores 4bit * 8 = 32 bits */
			T4[0] = S1[rows[0]][cols[0]] << 4;
			T4[0] |= S2[rows[1]][cols[1]];
			T4[1] = S3[rows[2]][cols[2]] << 4;
			T4[1] |= S4[rows[3]][cols[3]];
			T4[2] = S5[rows[4]][cols[4]] << 4;
			T4[2] |= S6[rows[5]][cols[5]];
			T4[3] = S7[rows[6]][cols[6]] << 4;
			T4[3] |= S8[rows[7]][cols[7]];
			
			/* internal permutation */
			permutation(T5, T4, P);
			/* internal f function starts here */
			
			/* calculate Li and Ri and twist */
			for (i = 0; i < R_BYTES; i++) {
				R[i] = L[i] ^ T5[i];
				L[i] = T0[i];			
			}

			if (isFirst) {
				fprintf(stderr, "(L%d,R%d)=", j+1, j+1);
				for (i = 0; i < L_BYTES; i++) fprintf(stderr, "%02x", L[i]);
				for (i = 0; i < R_BYTES; i++) fprintf(stderr, "%02x", R[i]);
				fprintf(stderr, "\n");			
			}
			
			/* reset buffers */
			for (i = 0; i < R_BYTES; i++) T0[i] = 0;
			for (i = 0; i < SUBKEY_BYTES; i++) T1[i] = 0;
			for (i = 0; i < SUBKEY_BYTES; i++) T2[i] = 0;
			for (i = 0; i < T3_BYTES; i++) T3[i] = 0;
			for (i = 0; i < L_BYTES; i++) T4[i] = 0;
			for (i = 0; i < L_BYTES; i++) T5[i] = 0;
			for (i = 0; i < T3_BYTES; i++) rows[i] = 0;
			for (i = 0; i < T3_BYTES; i++) cols[i] = 0;
			tmp = 0;
		}
		
		/* final IP inverse permutation */
		initialPermutationInverse(R, L, outputBuf, IP_INV);
}

void initialPermutation(unsigned char *inputBuf, unsigned char *L,
					   unsigned char *R, unsigned char *IP)
{
	int i, num, mask;
	for (i = 0; i < IP_LEN; i++) {
		num = IP[i] - 1;
		mask = (inputBuf[num / 8] >> (7 - num % 8)) & 0x01;
		if (i < 8) L[0] |= (mask << (7 - i));
		else if (i < 16) L[1] |= (mask << (7 - (i - 8)));
		else if (i < 24) L[2] |= (mask << (7 - (i - 16)));
		else if (i < 32) L[3] |= (mask << (7 - (i - 24)));
		else if (i < 40) R[0] |= (mask << (7 - (i - 32)));
		else if (i < 48) R[1] |= (mask << (7 - (i - 40)));
		else if (i < 56) R[2] |= (mask << (7 - (i - 48)));
		else R[3] |= (mask << (7 - (i - 56)));
	}
}

void expand(unsigned char *T1, unsigned char *R, unsigned char *E)
{
	int i, num, mask;
	for (i = 0; i < E_LEN; i++) {
		num = E[i] - 1;
		mask = (R[num / 8] >> (7 - num % 8)) & 0x01;
		if (i < 8) T1[0] |= (mask << (7 - i));
		else if (i < 16) T1[1] |= (mask << (7 - (i - 8)));
		else if (i < 24) T1[2] |= (mask << (7 - (i - 16)));
		else if (i < 32) T1[3] |= (mask << (7 - (i - 24)));
		else if (i < 40) T1[4] |= (mask << (7 - (i - 32)));
		else T1[5] |= (mask << (7 - (i - 40)));
	}
}

void permutation(unsigned char *T5, unsigned char *T4, unsigned char *P)
{
	int i, num, mask;
	for (i = 0; i < P_LEN; i++) {
		num = P[i] - 1;
		mask = (T4[num / 8] >> (7 - num % 8)) & 0x01;
		if (i < 8) T5[0] |= (mask << (7 - i));
		else if (i < 16) T5[1] |= (mask << (7 - (i - 8)));
		else if (i < 24) T5[2] |= (mask << (7 - (i - 16)));
		else T5[3] |= (mask << (7 - (i - 24)));
	}
}

void initialPermutationInverse(unsigned char *L, unsigned char *R,
					   unsigned char *output, unsigned char *IP_INV)
{
	int i, num, mask;
	for (i = 0; i < IP_LEN; i++) {
		num = IP_INV[i] - 1;
		mask = (num < 32) ? ((L[num / 8] >> (7 - num % 8)) & 0x01) : 
							((R[(num - 32) / 8] >> (7 - (num - 32) % 8)) & 0x01);
		if (i < 8) output[0] |= (mask << (7 - i));
		else if (i < 16) output[1] |= (mask << (7 - (i - 8)));
		else if (i < 24) output[2] |= (mask << (7 - (i - 16)));
		else if (i < 32) output[3] |= (mask << (7 - (i - 24)));
		else if (i < 40) output[4] |= (mask << (7 - (i - 32)));
		else if (i < 48) output[5] |= (mask << (7 - (i - 40)));
		else if (i < 56) output[6] |= (mask << (7 - (i - 48)));
		else output[7] |= (mask << (7 - (i - 56)));
	}
}




