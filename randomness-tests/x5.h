#ifndef X5_H
#define X5_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/* Perform autocorrelation test on the input
 * @param fp input file pointer or stdin
 */
void x5(int dval, FILE *fp);

#endif
