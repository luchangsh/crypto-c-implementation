#ifndef DECRYPT3_H
#define DECRYPT3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablecheck.h"
#include "encrypt.h"

/* decryption using 3DES in ECB mode */
void decrypt3(char *key3, FILE *tfp, FILE *ifp);

#endif
