#ifndef MERGE_H
#define MERGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw2.h"

#define MNUM_SIZE 4 /* buffer size of pbm file magic number line */
#define DIM_SIZE 15 /* buffer size of pbm file dimension line */

/* merge the two encrypted pictures fp1 and fp2
 * output a merged P4 PBM file 
 */
void merge(FILE *fp1, FILE *fp2);

#endif
