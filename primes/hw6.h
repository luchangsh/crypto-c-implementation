#ifndef HW6_H
#define HW6_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "primes.h"
#include "trialdiv.h"
#include "millerrabin.h"
#include "rndsearch.h"
#include "maurer.h"

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

/* 
 * parse a string representation of an positive integer, s, to
 * int, return a positive integer or -1 if s can't be parsed
 * to a positive integer
 */
int parseInt(char *s);

#endif
