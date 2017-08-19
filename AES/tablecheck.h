#ifndef TABLECHECK_H
#define TABLECHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw5.h"
#include "modprod.h"

#define S_LEN 256
#define INVS_LEN 256
#define P_LEN 4
#define INVP_LEN 4
#define NUM_TABLES 3
#define TF_BUF_SIZE 600
#define S_HEX_LEN 512
#define P_HEX_LEN 8
#define INVP_HEX_LEN 8

/* declare tables needed and call buildTables() to initialize tables
 * @Param tfp: tablefile pointer
 */
void tablecheck(FILE *tfp);

/* parse the key-value pairs in the tablefile and call helper functions to build tables */
void buildTables(FILE *tfp, unsigned char *S, unsigned char *INVS,
				 unsigned char *P, unsigned char *INVP);

/* check input data and build S and INVS tables*/
void buildSandINVSTable(unsigned char *S, unsigned char *INVS, char *key, char *val);

/* check input data and build P table */
void buildPTable(unsigned char *P, char *key, char *val);

/* check input data and build INVP table */
void buildINVPTable(unsigned char *INVP, char *key, char *val);

/* parse a byte in hexstring format to integer
 * @Param p: pointer points to the start of the hexstring
 */
unsigned char parseHexstring(char *p);

#endif
