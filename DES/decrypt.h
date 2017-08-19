#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablecheck.h"
#include "encrypt.h"

#define NUM_ROUNDS 16
#define SUBKEY_BYTES 6

/* DES decryption based on key and tablefile */
void decrypt(char *key, FILE *tfp, FILE *ifp);

/* reverse the order of subkeys for decryption */
void reverseOrderOfSubkeys(unsigned char subkeyBuf[NUM_ROUNDS][SUBKEY_BYTES]);

/* swap i and j rows in subkeyBuf */
void swap(unsigned char subkeyBuf[NUM_ROUNDS][SUBKEY_BYTES], int i, int j);

#endif
