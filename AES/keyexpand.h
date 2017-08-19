#ifndef KEYEXPAND_H
#define KEYEXPAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablecheck.h"

#define NK 4
#define NB 4
#define NR 10
#define KEY_HEX_LEN 32
#define KEY_LEN 16
#define WRD_ROWS 44
#define WRD_COLS 4
#define RCON_ROWS 11
#define RCON_COLS 4

/* perform key expansion algorithm */
void keyexpand(char *key, FILE *tfp);

/* check integrity of key hexstring and build key arrray */
void buildKeyArray(char *key, unsigned char *keyArr);

/* create subkeys and store in wordArr using key array and a s-box */
void keyExpansion(unsigned char *keyArr, unsigned char wordArr[WRD_ROWS][WRD_COLS],
				  unsigned char *S);

/* perform circular left shift on a word */
void rotWord(unsigned char *tmp);

/* substitute bytes in a word based on the s-box */
void subWord(unsigned char *tmp, unsigned char *S);

#endif
