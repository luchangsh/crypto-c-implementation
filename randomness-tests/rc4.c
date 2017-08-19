#include "rc4.h"

void rc4(char *pphrase, int len)
{
	/* generate 256 bytes of key */
	unsigned char key[256];
	generateKey(pphrase, key);
	
	/* state information */
	unsigned char state[256], x = 0, y = 0;
	
	/* initialization algorithm */
	int i;
	unsigned char t, j, k = 0;
	for (i = 0; i < 256; i++) state[i] = i;
	for (i = 0, j = 0; i < 256; i++, j = (j + 1) % 256) {
		t = state[i];
		state[i] = state[k += key[j] + t];
		state[k] = t;
	}

	/* output algorithm */
	if (len == -1) { /* -l is not specified, output internal state */
		fwrite(state, 1, sizeof(state), stdout);
	} else { /* -l is specified, run output algorithm */
		for (i = 0; i < len; i++) {
			t = state[y += state[++x]];
			state[y] = state[x];
			state[x] = t;
			fwrite(state + ((state[x] + state[y]) % 256),
				   1, sizeof(unsigned char), stdout);
		}
	}	
}

void generateKey(char *p, unsigned char *key)
{
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	char *s = malloc(strlen(p) + 3);
	unsigned char *cur = key; /* points to the start location for copy */
	int i;
	for (i = 0; i < 16; i++) {
		sprintf(s, "%02d%s", i, p);
		MD5((unsigned char *) s, strlen(s), md5_buf);
		memcpy(cur, md5_buf, MD5_DIGEST_LENGTH); /* copy results to key */
		cur += MD5_DIGEST_LENGTH; /* move forward */
	}
	free(s);
}

