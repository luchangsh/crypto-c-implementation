#ifndef X3_H
#define X3_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/* Perform poker test on the input
 * @param fp input file pointer or stdin
 */
void x3(FILE *fp);

/* convert a number to it's binary string representation
 * @param num number
 * @param str output string
 * @param sLen the length of the string
 */
char *toBinStr(int num, char *str, int sLen);

#endif
