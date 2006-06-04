/**
 * hanyupinying.h 
 *
 * Copyright (c) 2005
 *      libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

/* @(#)hanyupinying.h
 */

#ifndef _HANYUPINYING_H
#define _HANYUPINYING_H

/*
  This is a key-sequense map.
  From pinyin sequence to a default-layout sequence.
  Eg: Zhang -> {"zh","5"}, {"ang",";"}
 */
typedef struct {
	char pinying[ 5 ];
	char zuin[ 4 ];
} PinYingZuinMap;

int HanyuPinYingToZuin( char *pinyingKeySeq, char *zuinKeySeq );

#endif /* _HANYUPINYING_H */

