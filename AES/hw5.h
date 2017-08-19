#ifndef HW5_H
#define HW5_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablecheck.h"
#include "modprod.h"
#include "keyexpand.h"
#include "encrypt.h"
#include "decrypt.h"
#include "inverse.h"

/* print out the usage information */
void usage();

/* 
 * parse a string, s, with key=value format
 * return a pointer points to the start of the value string
 * or NULL if s doesn't contain '=' character therefore can't
 * be parsed
 * This function will change the input string s.
 */
char *parseOption(char *s);

#endif
