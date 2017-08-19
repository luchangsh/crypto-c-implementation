#ifndef HW2_H
#define HW2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stream.h"
#include "encrypt.h"
#include "merge.h"
#include "decrypt.h"

/* print out the usage information */
void usage();

/* 
 * parse a string representation of an positive integer, s, to
 * int, return a positive integer or -1 if s can't be parsed
 * to a positive integer
 */
int parseInt(const char *s);

/* 
 * parse a string, s, with key=value format
 * return a pointer points to the start of the value string
 * or NULL if s doesn't contain '=' character therefore can't
 * be parsed
 * This function will change the input string s.
 */
char *parseOption(char *s);

#endif
