#include "stream.h"

void stream(const char *p, int outLen, unsigned char *keyBuf)
{
	/* the following code is adapted from cs531 hw2 spec */
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	int len = strlen(p) + 2 + MD5_DIGEST_LENGTH;
	char *s = malloc(len + 1);
	int i = 0;	
	MD5((unsigned char *) p, strlen(p), md5_buf);
	while (outLen > 0) {
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, p);
		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
		MD5((unsigned char *) s, len, md5_buf);
		int currLen = ((sizeof(md5_buf) >> 1) > outLen) ? outLen : sizeof(md5_buf) >> 1;
		if (keyBuf == NULL) { /* output to stdout */
			fwrite(md5_buf, 1, currLen, stdout);
		} else {
			memcpy(keyBuf, md5_buf, currLen); /* output to a key buffer */
			keyBuf += currLen;
		}
		if (++i == 100) i = 0;
		outLen -= currLen;
	}
	free(s);
}
