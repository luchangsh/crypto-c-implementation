#include "keyexpand.h"

void keyexpand(char *key, FILE *tfp)
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

	/* print results */	
	for (i = 0; i < WRD_ROWS; i++) {
		fprintf(stdout, "w[%2d]: ", i);
		for (j = 0; j < WRD_COLS; j++) fprintf(stdout, "%02x", wordArr[i][j]);
		fprintf(stdout, "\n");
	}	
}

void buildKeyArray(char *key, unsigned char *keyArr)
{
	/* check the integrity of the key hexstring */
	if (strlen(key) != KEY_HEX_LEN) {
		fprintf(stderr, "The hexstring of key %s is not 32 characters long.\n", key);
		exit(EXIT_FAILURE);
	}
	int index = 0;
	char *p = key;
	while (*p != '\0') {
		if (!(*p >= '0' && *p <= '9') && !(*p >= 'a' && *p <= 'f')) {
			fprintf(stderr, "The hexstring of key %s contains invalid character \'%c\' at index %d.\n", key, *p, index);
			exit(EXIT_FAILURE);
		}
		p++;
		index++;		
	}
	
	int i;
	p = key;
	for (i = 0; i < KEY_LEN; i++) {
		keyArr[i] = parseHexstring(p);
		p += 2;
	}
}

void keyExpansion(unsigned char *keyArr, unsigned char wordArr[WRD_ROWS][WRD_COLS],
				  unsigned char *S)
{
	int i, j, k = 0;

	/* create Rcon */
	unsigned char Rcon[RCON_ROWS][RCON_COLS];
	for (i = 0; i < RCON_ROWS; i++)
		for (j = 0; j < RCON_COLS; j++)
			Rcon[i][j] = 0;
	Rcon[1][0] = 0x01;
	for (i = 2; i < RCON_ROWS; i++) Rcon[i][0] = xtime(Rcon[i - 1][0]);

	/* populate w0 ~ w3 */
	for (i = 0; i < NK; i++)
		for (j = 0; j < WRD_COLS; j++)
			wordArr[i][j] = keyArr[k++];

	/* populate w4 ~ w43 */	
	unsigned char tmp[WRD_COLS];
	for (i = 0; i < WRD_COLS; i++) tmp[i] = 0;
	
	for (i = NK; i < WRD_ROWS; i++) {
		for (j = 0; j < WRD_COLS; j++) tmp[j] = wordArr[i - 1][j];
		if (i % NK == 0) {
			rotWord(tmp);
			subWord(tmp, S);
			for (j = 0; j < WRD_COLS; j++) tmp[j] ^= Rcon[i / NK][j];
		}	
		for (j = 0; j < WRD_COLS; j++) wordArr[i][j] = wordArr[i - NK][j] ^ tmp[j];		
	}
}

void rotWord(unsigned char *tmp)
{
	unsigned char temp = tmp[0];
	int i;
	for (i = 1; i < WRD_COLS; i++) tmp[i - 1] = tmp[i];
	tmp[WRD_COLS - 1] = temp;
}

void subWord(unsigned char *tmp, unsigned char *S)
{
	int i;
	for (i = 0; i < WRD_COLS; i++) tmp[i] = S[tmp[i]];
}

