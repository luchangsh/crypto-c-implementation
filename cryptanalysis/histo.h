#ifndef HISTO_H
#define HISTO_H

#include <stdio.h>

#define CNT_SIZE 26 /* count buffer size */

/* Produce a histogram of ciphertext character frequencies
 * @Param period: period of the key
 * @Param which: one-based index for the alphabets
 * @Param ifp: input file point
 */
void histo(int period, int which, FILE *ifp);

#endif
