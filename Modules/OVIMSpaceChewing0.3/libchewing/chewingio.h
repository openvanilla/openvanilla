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
CHEWING_API int chewing_handle_Space( ChewingContext * );
CHEWING_API int chewing_handle_Esc( ChewingContext * );
CHEWING_API int chewing_handle_Enter( ChewingContext * );
CHEWING_API int chewing_handle_Del( ChewingContext * );
CHEWING_API int chewing_handle_Backspace( ChewingContext * );
CHEWING_API int chewing_handle_Tab( ChewingContext * );
CHEWING_API int chewing_handle_ShiftLeft( ChewingContext * );
CHEWING_API int chewing_handle_Left( ChewingContext * );
CHEWING_API int chewing_handle_ShiftRight( ChewingContext * );
CHEWING_API int chewing_handle_Right( ChewingContext * );
CHEWING_API int chewing_handle_Up( ChewingContext * );
CHEWING_API int chewing_handle_Home( ChewingContext * );
CHEWING_API int chewing_handle_End( ChewingContext * );
CHEWING_API int chewing_handle_Down( ChewingContext * );
CHEWING_API int chewing_handle_Capslock( ChewingContext * );
CHEWING_API int chewing_handle_Default( ChewingContext *, int key );
CHEWING_API int chewing_handle_CtrlNum( ChewingContext *, int key );
CHEWING_API int chewing_handle_CtrlOption( ChewingContext *, int key );
CHEWING_API int chewing_handle_ShiftSpace( ChewingContext * );
CHEWING_API int chewing_handle_DblTab( ChewingContext * );
CHEWING_API int chewing_handle_Numlock( ChewingContext *, int key);

/**
 * @brief Set selectAreaLen, maxChiSymbolLen, selKey in pcd
 *
 * @param ctx Chewing IM context
 * @param pcd
 */
CHEWING_API int chewing_Configure( ChewingContext *ctx, ChewingConfigData *pcd );

/**
 * @brief create new handle of ChewingContext
 */
CHEWING_API ChewingContext *chewing_new();

/**
 * @brief release handle of ChewingContext
 *
 * Release internal memory allocation.
 */
CHEWING_API void chewing_free( ChewingContext *ctx );

/**
 * @brief Initialize chewing
 *
 * Init directory data.
 *
 * @param dataPath
 * @param hashPath
 * @retval 0 if succeed
 */
CHEWING_API int chewing_Init(
		const char *dataPath,
		const char *hashPath );

/**
 * @breif Reset all settings
 *
 * @param ctx
 * @return If successed than return 0
 */
CHEWING_API int chewing_Reset( ChewingContext *ctx );

/**
 * @breif Set keyboard type
 *
 * @param ctx
 * @param kbtype kbtype from KBStr2Num
 * @return If successed then return 0
 */
CHEWING_API int chewing_set_KBType( ChewingContext *ctx, int kbtype );

/**
 * @brief Terminate chewing
 *
 * Terminate the I/O routines for chewing.
 */
CHEWING_API void chewing_Terminate();

/**
 * @brief Set chewing's english, chinese mode
 *
 * @param ctx
 * @param mode CHINESE_MODE or ENGLISH_MODE
 */
CHEWING_API void chewing_set_ChiEngMode( ChewingContext *ctx, int mode );

/**
 * @brief Get chewing's current mode
 *
 * @param ctx
 * 
 * @return CHINESE_MODE or ENGLISH_MODE
 */
CHEWING_API int chewing_get_ChiEngMode( ChewingContext *ctx );

/**
 * @brief Set chewing's fullshape, halfshape mode
 *
 * @param ctx
 * @param mode FULLSHAPE_MODE or HALFSHAPE_MODE
 */
CHEWING_API void chewing_set_ShapeMode( ChewingContext *ctx, int mode );

/**
 * @brief Get chewing's current shape mode
 *
 * @param ctx
 * 
 * @return FULLSHAPE_MODE or HALFSHAPE_MODE
 */
CHEWING_API int chewing_get_ShapeMode( ChewingContext *ctx );

/**
 * @brief Get kbtype number from string
 *
 * @param str[] name of kbtype eg. "KB_HSU"
 */
CHEWING_API int chewing_KBStr2Num( char str[] );

#endif

