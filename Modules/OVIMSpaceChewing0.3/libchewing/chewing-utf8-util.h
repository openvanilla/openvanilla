/**
 * chewing-utf8-util.h
 *
 * Copyright (c) 2005, 2006
 *	libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

#ifndef CHEWING_UTF8_UTILS_H
#define CHEWING_UTF8_UTILS_H

/* Return length of UTF-8 string */
int ueStrLen( char *str );

/* Return bytes of a UTF-8 character */
int ueBytesFromChar( unsigned char b );

#define STRNCPY_CLOSE 1
#define STRNCPY_NOT_CLOSE 0

/* Return how many bytes was copied */
int ueStrNCpy( char dest[], const char *src, size_t n, int end );

/* Return address from n length after src */
char *ueStrSeek( char *src, size_t n );

#endif /* CHEWING_UTF8_UTILS_H */
