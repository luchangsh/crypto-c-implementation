#include "keygen.h"
void keygen(const char *pphrase, int period)
{
	/* Generate random bytes using a stream cipher from HW2 */
	int bufLen = BYTES_PER_PERMUTATION * period;
	unsigned char buffer[bufLen];
	stream(pphrase, bufLen, buffer);
	
	char state[27]; /* Store 26 lowercase letters a-z + '\0' at the end */
	state[26] = '\0';
	int i, j, itr, start = 0;
	unsigned int L, X, R;
	
	for (i = 0; i < period; i++) {
		/* Reset state[] to original alphabetical order */
		for (j = 0; j < 26; j++) {
			state[j] = 'a' + j;
		}
		/* Create random permutation */
		for (itr = 0; itr < 2; itr++) {
			L = 26;
			while (L > 1) {
				X = RandomInteger(buffer, start); /* Pick 4 bytes from buffer[start] */
				R = X % L;
				if (R != L - 1) {
					swap(state, R, L - 1);
				}
				start += 4; /* Move forward 4 bytes */
				L--;
			}
		}
		/* Output result of one permutation */
		fprintf(stdout, "%s\n", state);
	}
}

unsigned int RandomInteger(const unsigned char *buffer, int start)
{
	unsigned int res = 0;
	int i;
	for (i = 0; i < 4; i++) {
		res <<= 8; /* Shift 8 bits == 1 byte */
		res |= (unsigned int) buffer[start + i];
	}
	return res;
}

void swap(char *s, int i, int j)
{
	char tmp = s[i];
	s[i] = s[j];
	s[j] = tmp;
}
