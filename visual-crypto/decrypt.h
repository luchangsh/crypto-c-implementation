#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw2.h"

#define MNUM_SIZE 4 /* buffer size of pbm file magic number line */
#define DIM_SIZE 15 /* buffer size of pbm file dimension line */

/* perform the reverse of the encryption algorithm
 * fp: file pointer of the input file
 * output a decrypted P4 PBM file
 */
void decrypt(FILE *fp);

#endif
