#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <openssl/md5.h>

/*
 * a simple stream cipher
 * 
 * p: passphrase
 * outLen: the number of bytes to output
 * keyBuf: a buffer to store generated keys, if this value is NULL, output to stdout
 */
void stream(const char *p, int outLen, unsigned char *keyBuf);

#endif
