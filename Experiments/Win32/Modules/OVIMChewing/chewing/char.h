/**
 * char.h
 *
 * Copyright (c) 1999, 2000, 2001
 *	Lu-chuan Kung and Kang-pen Chen.
 *	All rights reserved.
 *
 * Copyright (c) 2004
 *	libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

#ifndef CHAR_H
#define CHAR_H

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

#include "global.h"

/* wc -l data/ch_index.dat - 1 */
#define PHONE_NUM (1320)

typedef struct {
	char word[ 3 ];
} Word;

int InitChar( const char * );
int GetCharFirst( Word *, uint16 );
int GetCharNext ( Word * );

#endif

