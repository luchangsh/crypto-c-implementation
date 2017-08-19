#ifndef ENCRYPT3_H
#define ENCRYPT3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablecheck.h"
#include "encrypt.h"

/* EDE triple DES in ECB mode */
void encrypt3(char *key3, FILE *tfp, FILE *ifp);

#endif
