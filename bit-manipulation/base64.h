#ifndef BASE64_H
#define BASE64_H

/* headers */
#include <stdio.h>
#include <stdlib.h>

/* constants */
#define IN_SIZE_ENC 3 /* size of encode input buffer */
#define OUT_SIZE_ENC 4 /* size of encode output buffer */
#define MAX_CHAR_PER_LINE 64 /* max characters per line of encodeBase64 output */

#define IN_SIZE_DEC 4 /* size of decode input buffer */
#define OUT_SIZE_DEC 3 /* size of decode output buffer */

/* functions */
/* base64 encode file or stdin */
void encodeBase64(FILE *fp);

/* base64 decode file or stdin */
void decodeBase64(FILE *fp);

/* check if ch is a valid character of base64 encoding */
int isValid(int ch);

/* reverse a base64 encode character to it's int value */
unsigned char revChar(char ch);

/*
 * decode 4 bytes in input buffer to 3 bytes output buffer and print them
 * in: input buffer
 * out output buffer
 * len: length of the output bytes to print
 */
void decodeBase64Helper(unsigned char in[], unsigned char out[], int len);

#endif
