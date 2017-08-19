#ifndef MODPROD_H
#define MODPROD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablecheck.h"

#define P1_HEX_LEN 8
#define P2_HEX_LEN 8
#define A_LEN 4
#define B_LEN 4
#define D_LEN 4

/* check input, parse hexstrings and call circlex() to perform modular product of poly1 and poly2 */
void modprod(char *poly1, char *poly2);

/* perform modular product, d(x) = a(x) circlex b(x) */
void circlex(unsigned char *a, unsigned char *b, unsigned char *d);

/* return x * b(x) */
unsigned char xtime(unsigned char b);

/* return a(x) bigdot b(x) */
unsigned char bigdot(unsigned char a, unsigned char b);

#endif
