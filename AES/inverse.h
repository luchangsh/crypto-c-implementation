#ifndef INVERSE_H
#define INVERSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablecheck.h"

#define POLY_HEX_LEN 8
#define POLY_LEN 4
#define TAB_ROWS 6
#define TAB_COLS 4
#define MAX_ROUND 6

/* check input, parse poly hexstring and call calculateInv() to calculate the multiplicative inverse */
void inverse(char *poly);

/* calculate the multiplicative inverse of poly */
void calculateInv(unsigned char *a, unsigned char *aInv);

/* get the degree of a polynomial at a row of the rem table */
int getDegree(unsigned char rem[TAB_ROWS][TAB_COLS], int row);

#endif
