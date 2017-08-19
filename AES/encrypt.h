#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <stdlib.h>

#include "tablecheck.h"
#include "keyexpand.h"
#include "modprod.h"

#define NK 4
#define NB 4
#define NR 10
#define IN_BUF_LEN 16
#define STAT_ROWS 4
#define STAT_COLS 4
#define WRD_ROWS 44
#define WRD_COLS 4

/* perform AES encryption of the first 16 bytes in the input file ifp based on the key and a tablefile tfp */
void encrypt(char *key, FILE *tfp, FILE *ifp);

/* build internal state from the input buffer */
void buildState(unsigned char state[STAT_ROWS][STAT_COLS], unsigned char *inputBuf);

/* perform addRoundKey operation using wordArr[fromRow] to wordArr[fromRow + 3] */
void addRoundKey(unsigned char state[STAT_ROWS][STAT_COLS],
				 unsigned char wordArr[WRD_ROWS][WRD_COLS], int fromRow);

/* subBytes using the s-box S */
void subBytes(unsigned char state[STAT_ROWS][STAT_COLS], unsigned char *S);

/* shift rows in the internal state */
void shiftRows(unsigned char state[STAT_ROWS][STAT_COLS]);

/* perform one circular left shift for a row in the state */
void shiftOneLeft(unsigned char state[STAT_ROWS][STAT_COLS], int row);

/* perform mixColumn operation based on P polynomial */
void mixColumns(unsigned char state[STAT_ROWS][STAT_COLS], unsigned char *P);

/* print internal state */
void printState(unsigned char state[STAT_ROWS][STAT_COLS]);

/* print key schedule */
void printKeySchedule(unsigned char wordArr[WRD_ROWS][WRD_COLS], int fromRow);

#endif
