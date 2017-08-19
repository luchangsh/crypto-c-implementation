#ifndef CRYPT_H
#define CRYPT_H

#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 27
#define CNT_SIZE 26

/* Encrypts (or decrypts) an input ASCII text file with the key
 * specified in keyfile.
 * @Param kfp: keyfile pointer
 * @Param ifp: input file pointer
 */
void crypt(FILE *kfp, FILE *ifp);

#endif
