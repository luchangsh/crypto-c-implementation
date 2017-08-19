#ifndef RC4_H
#define RC4_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

/* Simulate the RC4 stream cipher
 * @param pphrase passphrase
 * @param len the number of bytes to output if -l is specified
 * if -l is not specified, len = 256 bytes
 */
void rc4(char *pphrase, int len);

/* Generate 256 bytes of key for key scheduling algorithm
 * @param p passphrase
 * @param key 256 bytes of key
 */
void generateKey(char *p, unsigned char *key);

#endif
