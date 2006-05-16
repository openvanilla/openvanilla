/**
 * dict.h
 *
 * Copyright (c) 1999, 2000, 2001
 *	Lu-chuan Kung and Kang-pen Chen.
 *	All rights reserved.
 *
 * Copyright (c) 2004, 2005
 *	libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

#ifndef DICT_H
#define DICT_H

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

#include "global.h"

#define PHONE_PHRASE_NUM (162244)

int InitDict( const char * );
int GetPhraseFirst( Phrase *phr_ptr, int phone_phr_id );
int GetPhraseNext ( Phrase *phr_ptr );

#endif

