#ifndef SOLVE_H
#define SOLVE_H

#include <stdio.h>
#include <stdlib.h>

#define CNT_SIZE 26 /* count buffer size */

/* Determine the period of a ciphertext */
void solve(int max_t, FILE *ifp);

/* Determine the peroid by Kasiski method
 * @Param ct: ciphertext
 * @Param n: length of the ciphertext
 */
void kasiski(const unsigned char *ct, int n);

/* Determine the peroid by the index of coincidence method
 * @Param ct: ciphertext
 * @Param n: length of the ciphertext
 * @Param max_t: maximum peroid
 */
void indexOfCoincidence(const unsigned char *ct, int n, int max_t);

/* Determine the peroid by the autocorrelation method
 * @Param ct: ciphertext
 * @Param n: length of the ciphertext
 * @Param max_t: maximum peroid 
 */
void autoCorrelation(const unsigned char *ct, int n, int max_t);

#endif
