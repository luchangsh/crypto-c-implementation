#ifndef HEXDUMP_H
#define HEXDUMP_H

/* headers */
#include <stdio.h>

/* constants */
#define BUFSIZE 16 /* buffer size */

/* functions */
/* produce a hexdump of file or stdin */
void hexdump(FILE *fp);

#endif
