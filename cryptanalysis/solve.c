#include "solve.h"
void solve(int max_t, FILE *ifp)
{
	/* Count the length of the cipher text */
	int cipherLen = 0; /* Length of the cipher text */
	int ch;
	while ((ch = fgetc(ifp)) != '\n') {
		cipherLen++;
	}
	fseek(ifp, 0L, SEEK_SET); /* Go to the beginning of the file */
	
	/* Store the cipher text into a buffer */
	unsigned char cipher[cipherLen];
	int i = 0;
	while ((ch = fgetc(ifp)) != '\n') {
		cipher[i++] = ch;
	}
	
	/* Generate report by 3 methods */
	kasiski(cipher, cipherLen);
	indexOfCoincidence(cipher, cipherLen, max_t);
	autoCorrelation(cipher, cipherLen, max_t);
}

void kasiski(const unsigned char *ct, int n)
{
	fprintf(stdout, "Kasiski Method\n");
	fprintf(stdout, "==============\n");
	int len = 4;
	int isEnd = 0, isFound = 0;
	int i, j, k;
	unsigned char *s = NULL;
	while (!isEnd) {
		s = (unsigned char *) malloc((len + 1) * sizeof(unsigned char));
		if (s == NULL) {
			fprintf(stderr, "kasiski(): Memory allocation failed.\n");
			exit(EXIT_FAILURE);
		}
		for (i = 0; i < len + 1; i++){
			s[i] = 0;
		}
		s[len] = '\0';
		for (i = 0; i < n - len; i++) {
			for (j = 0; j < len; j++) {
				if (ct[i + j] < 'a' || ct[i + j] > 'z') break;
				s[j] = ct[i + j];
			}
			if (j != len) continue;
			for (j = i + 1; j <= n - len; j++) {
				for (k = 0; k < len; k++) {
					if (ct[j + k] != s[k]) break;					
				}
				if (k == len) {
					fprintf(stdout, "len=%d, i=%d, j=%d, j-i=%d, %s\n", len, i, j, j-i, s);
					if (isFound == 0) {
						isFound = 1;
					}
				}
			}
		}
		if (!isFound) {
			fprintf(stdout, "len=%d, no more matches\n", len);
			isEnd = 1;
		} else {
			isFound = 0;
			len++;
		}
		free(s);
	}
	fprintf(stdout, "\n");
}

void indexOfCoincidence(const unsigned char *ct, int n, int max_t)
{
	fprintf(stdout, "Average Index of Coincidence\n");
	fprintf(stdout, "============================\n");
	int count[CNT_SIZE];  /* Count array */
	int i;
	for (i = 0; i < CNT_SIZE; i++) { /* Initialization */
		count[i] = 0;
	}
	int L = 0, ch;
	for (i = 0; i < n; i++) {
		ch = ct[i];
		if (ch >= 'a' && ch <= 'z') {
			L++;
			count[ch - 'a']++;
		}
	}
	fprintf(stdout, "L=%d\n", L);
	for (i = 0; i < CNT_SIZE; i++) {
		fprintf(stdout, "f('%c')=%d\n", 'a'+i, count[i]);
	}

	int num = 0;
	for (i = 0; i < CNT_SIZE; i++) {
		num += count[i] * (count[i] - 1);
	}
	double IC = num / (double) (L * (L - 1));
	fprintf(stdout, "IC=%.8lg\n", IC);
	
	const double kp = 0.0658;
	const double kr = 1.0 / CNT_SIZE;
	int t;
	for (t = 1; t <= max_t; t++) {
		fprintf(stdout, "t=%d, E(IC)=%.8lg\n", t
			, (1.0 / t) * ((double) (L - t) / (L - 1)) * kp 
				+ ((t - 1) / (double) t) * ((double) L / (L - 1)) * kr);
	}
	fprintf(stdout, "\n");
}

void autoCorrelation(const unsigned char *ct, int n, int max_t)
{
	fprintf(stdout, "Auto-correlation Method\n");
	fprintf(stdout, "=======================\n");
	int t, i, cnt, c1, c2;
	for (t = 1; t <= max_t; t++) {
		cnt = 0;
		for (i = 1; i <= n - t; i++) {
			c1 = ct[i - 1];
			c2 = ct[i + t - 1];		
			if (c1 >= 'a' && c2 >= 'a' && c1 <= 'z' && c2 <= 'z' && c1 == c2) {
				cnt++;
			}
		}
		fprintf(stdout, "t=%d, count=%d\n", t, cnt);
	}
	fprintf(stdout, "\n");	
}
