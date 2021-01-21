#ifndef DOS_FILEIO_H
#define DOS_FILEIO_H

#ifndef ANSI
#include <stddef.h>     /* for compatibility to older version */
#endif  /* ANSI */

#define BUFFSIZE 1024

int dos_fopen(const char* file_name);
int dos_fclose(unsigned int handle);
int dos_fread(void __far *array, size_t size, size_t n, unsigned int handle);


#endif