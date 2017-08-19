#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdio.h>
#include <stdlib.h>

#include "tablecheck.h"
#include "keyexpand.h"
#include "modprod.h"
#include "encrypt.h"

#define STAT_ROWS 4
#define STAT_COLS 4

/* perform AES decryption of the first 16 bytes in the input file ifp based on the key and a tablefile tfp */
void decrypt(char *key, FILE *tfp, FILE *ifp);

/* shift rows in the internal state */
void invShiftRows(unsigned char state[STAT_ROWS][STAT_COLS]);

/* perform one circular right shift for a row in the state */
void shiftOneRight(unsigned char state[STAT_ROWS][STAT_COLS], int row);

#endif
