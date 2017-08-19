#ifndef INVKEY_H
#define INVKEY_H

#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 27 /* buffer size */
#define CNT_SIZE 26 /* count buffer size */

/* Create an inverse permutation key file from keyfile
 * @Param kfp: keyfile pointer
 */
void invkey(FILE *kfp);

#endif
