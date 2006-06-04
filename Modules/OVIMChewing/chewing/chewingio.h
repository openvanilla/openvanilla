/**
 * chewingio.h
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

#ifndef _CHEWING_IO_H
#define _CHEWING_IO_H
#include "global.h"

#define KEYSTROKE_IGNORE 1
#define KEYSTROKE_COMMIT 2
#define KEYSTROKE_BELL 4
#define KEYSTROKE_ABSORB 8

/**
 * @brief Function should be called when ESC emit
 */
int OnKeySpace( void *, ChewingOutput * );
int OnKeyEsc( void *, ChewingOutput * );
int OnKeyEnter( void *, ChewingOutput * );
int OnKeyDel( void *, ChewingOutput * );
int OnKeyBackspace( void *, ChewingOutput * );
int OnKeyTab( void *, ChewingOutput * );
int OnKeyShiftLeft( void*, ChewingOutput* );
int OnKeyLeft( void *, ChewingOutput * );
int OnKeyShiftRight( void*, ChewingOutput* );
int OnKeyRight( void *, ChewingOutput * );
int OnKeyUp( void *, ChewingOutput * );
int OnKeyHome( void *, ChewingOutput * );
int OnKeyEnd( void *, ChewingOutput * );
int OnKeyDown( void *, ChewingOutput * );
int OnKeyCapslock( void *, ChewingOutput * );
int OnKeyDefault( void *, int, ChewingOutput * );
int OnKeyCtrlNum( void *, int, ChewingOutput * );
int OnKeyCtrlOption( void *, int, ChewingOutput * );
int OnKeyShiftSpace( void *, ChewingOutput * );
int OnKeyDblTab( void *iccf, ChewingOutput * );
int OnKeyNumlock(void *iccf, int key, ChewingOutput *pgo);

/**
 * @brief Set selectAreaLen, maxChiSymbolLen, selKey in pcd
 *
 * @param iccf Static data structure
 * @param pcd
 */
int SetConfig( void *iccf, ConfigData *pcd );

/**
 * @brief Initialize chewing
 *
 * Set kbtype, choiceinfo, cursor... for chewing
 *
 * @param iccf
 * @param cf
 * @return If successed then return 0
 */
int InitChewing( void *iccf, ChewingConf *cf );

/**
 * @brief Terminate chewing
 *
 * Terminate the I/O routines for chewing.
 */
void TerminateChewing();

/**
 * @brief Set chewing's english, chinese mode
 *
 * @param iccf
 * @param mode CHINESE_MODE or ENGLISH_MODE
 */
void SetChiEngMode( void *iccf, int mode );

/**
 * @brief Get chewing's current mode
 *
 * @param iccf
 * 
 * @return CHINESE_MODE or ENGLISH_MODE
 */
int GetChiEngMode( void *iccf );

/**
 * @brief Get kbtype number from string
 *
 * @param str[] name of kbtype eg. "KB_HSU"
 */
int KBStr2Num( char str[] );

#endif

