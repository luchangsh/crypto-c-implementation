#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablecheck.h"

#define KEY_LEN 16 /* in hex string chars */
#define KEY_BUF_SIZE 8 /* in bytes */
#define NUM_ROUNDS 16
#define SUBKEY_BYTES 6
#define C_LEN 4 /* in bytes */
#define D_LEN 4 /* in bytes */
#define IN_BUF_SIZE 8 /* in bytes */
#define OUT_BUF_SIZE 8 /* in bytes */
#define L_BYTES 4
#define R_BYTES 4
#define T3_BYTES 8

#define S_ROWS 4
#define S_COLS 16

/* DES encryption based on key and tablefile */
void encrypt(char *key, FILE *tfp, FILE *ifp);

/* convert a key hex string to bytes array */
void buildKeyBuf(char *key, unsigned char *keyBuf);

/* generate 16 48bits subkeys and store them in subkeyBuf */
void generateSubkeys(unsigned char *keyBuf, unsigned char *V,
					 unsigned char *PC1, unsigned char *PC2,
					 unsigned char subkeyBuf[NUM_ROUNDS][SUBKEY_BYTES]);

/* left circular shift one bit */
void leftShift1(unsigned char *arr);

/* left circular shift two bits */
void leftShift2(unsigned char *arr);

/* DES encryption of inputBuf and store in outputBuf */
void des(unsigned char *inputBuf, unsigned char *outputBuf,
		 unsigned char *L, unsigned char *R, unsigned char *subkey,
		 unsigned char *T0, unsigned char *T1, unsigned char *T2,
		 unsigned char *T3, unsigned char *T4, unsigned char *T5,
		 unsigned char *rows, unsigned char *cols, int isFirst,
		 unsigned char subkeyBuf[NUM_ROUNDS][SUBKEY_BYTES],
		 unsigned char *IP, unsigned char *IP_INV,
		 unsigned char *E, unsigned char *P,
		 unsigned char S1[S_ROWS][S_COLS], unsigned char S2[S_ROWS][S_COLS], 
		 unsigned char S3[S_ROWS][S_COLS], unsigned char S4[S_ROWS][S_COLS], 
		 unsigned char S5[S_ROWS][S_COLS], unsigned char S6[S_ROWS][S_COLS],
		 unsigned char S7[S_ROWS][S_COLS], unsigned char S8[S_ROWS][S_COLS], 
		 unsigned char *V, unsigned char *PC1, unsigned char *PC2);

/* initial permuation of DES base on IP table */
void initialPermutation(unsigned char *inputBuf, unsigned char *L,
					   unsigned char *R, unsigned char *IP);

/* Expension 32 bits to 48 bits by E table */
void expand(unsigned char *T1, unsigned char *R, unsigned char *E);

/* Internal permutation P */
void permutation(unsigned char *T5, unsigned char *T4, unsigned char *P);

/* inverse of initial permutation, L left part of input, R right part of input */
void initialPermutationInverse(unsigned char *L, unsigned char *R, unsigned char *output, unsigned char *IP_INV);

#endif
