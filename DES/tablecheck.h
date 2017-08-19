#ifndef TABLECHECK_H
#define TABLECHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw4.h"

#define TF_BUF_SIZE 200
#define NUM_TABLES 14
#define IP_LEN 64
#define IP_MAX 64
#define IP_MIN 1
#define E_LEN 48
#define E_MAX 32
#define E_MIN 1
#define P_LEN 32
#define P_MAX 32
#define P_MIN 1
#define S_SIZE 64
#define S_ROWS 4
#define S_COLS 16
#define S_MAX 15
#define S_MIN 0
#define V_LEN 16
#define V_MAX 2
#define V_MIN 1
#define V_NUM_OF_ONES 4
#define V_NUM_OF_TWOS 12
#define PC1_LEN 56
#define PC1_MAX 64 
#define PC1_MIN 1
#define PC2_LEN 48
#define PC2_MAX 56 
#define PC2_MIN 1

/* Declare various tables, call buildTables to initialize the tables */
void tablecheck(FILE *tfp);

/* Parse a tablefile pointed by tfp, parse each line, key=value, call the correspondent functions to build tables */
void buildTables(FILE *tfp, unsigned char *IP, unsigned char *IP_INV, unsigned char *E, unsigned char *P,
				 unsigned char S1[S_ROWS][S_COLS], unsigned char S2[S_ROWS][S_COLS], unsigned char S3[S_ROWS][S_COLS],
				 unsigned char S4[S_ROWS][S_COLS], unsigned char S5[S_ROWS][S_COLS], unsigned char S6[S_ROWS][S_COLS],
				 unsigned char S7[S_ROWS][S_COLS], unsigned char S8[S_ROWS][S_COLS], unsigned char *V,
				 unsigned char *PC1, unsigned char *PC2);

/* Check and build IP and IP_INV tables */
void buildIPandIP_INVTable(unsigned char *IP, unsigned char *IP_INV, char *buffer, char *val);

/* Check and build E tables */
void buildETable(unsigned char *E, char *buffer, char *val);

/* Check and build P tables */
void buildPTable(unsigned char *P, char *buffer, char *val);

/* Check and build S1~S8 tables */
void buildSTable(unsigned char S[S_ROWS][S_COLS], char *buffer, char *val);

/* Check and build V tables */
void buildVTable(unsigned char *V, char *buffer, char *val);

/* Check and build PC1 tables */
void buildPC1Table(unsigned char *PC1, char *buffer, char *val);

/* Check and build PC2 tables */
void buildPC2Table(unsigned char *PC2, char *buffer, char *val);

#endif
