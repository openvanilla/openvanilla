/**
 * hanyupinyin.h 
 *
 * Copyright (c) 2005, 2006
 *      libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

/* @(#)hanyupinyin.h
 */

#ifndef _HANYUPINYIN_H
#define _HANYUPINYIN_H

/*
  This is a key-sequense map.
  From pinyin sequence to a default-layout sequence.
  Eg: Zhang -> {"zh","5"}, {"ang",";"}
 */
struct keymap {
	char pinyin[7];
	char zuin[4];
};
typedef struct keymap keymap;


int HanyuPinYinToZuin( char *pinyinKeySeq, char *zuinKeySeq );

#endif /* _HANYUPINYIN_H */

