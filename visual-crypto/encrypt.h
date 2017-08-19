#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw2.h"
#include "stream.h"

#define MNUM_SIZE 4 /* buffer size of pbm file magic number line */
#define DIM_SIZE 15 /* buffer size of pbm file dimension line */

/* Encrypt an imput PBM file, no output, side effect is to create
 * two files in the P4 PBM format.
 * pphrase: passphrase
 * name: output file name
 * fp: input P4 PBM file
 */
void encrypt(const char *pphrase, const char *name, FILE *fp);

#endif
