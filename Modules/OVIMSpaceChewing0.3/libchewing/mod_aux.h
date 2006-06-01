/**
 * mod_aux.c
 *
 * Copyright (c) 2005
 *	libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

#ifndef CHEWING_MOD_AUX_H
#define CHEWING_MOD_AUX_H

/**
 * @file mod_aux.h
 * @brief Auxiliary module
 */

#include "global.h"

/**
 * @param ctx handle to Chewing IM context
 * @retval TRUE if it currnet input state is at the "end-of-a-char"
 */
int chewing_commit_Check( ChewingContext *ctx );

/**
 * @param ctx handle to Chewing IM context
 *
 * retrun current commit string, regardless current input state.
 * Alwasy returns a char pointer, caller must free it.
 */
char *chewing_commit_String( ChewingContext *ctx );

char *chewing_buffer_String( ChewingContext *ctx , int from, int to);
char *chewing_buffer_String_End( ChewingContext *ctx , int from);
int chewing_buffer_Check( ChewingContext *ctx );

/**
 * @param ctx handle to Chewing IM context
 * @param zuin_count pointer to the integer of available Zuin preedit string
 *
 * Always returns a char pointer, caller must free it.
 */
char *chewing_zuin_String( ChewingContext *ctx, int *zuin_count );

int chewing_zuin_Check( ChewingContext *ctx );


int chewing_cursor_Current( ChewingContext *ctx );
int chewing_point_Start( ChewingContext *ctx );
int chewing_point_End( ChewingContext *ctx );


int chewing_cand_CheckDone( ChewingContext *ctx );
int chewing_cand_TotalPage( ChewingContext *ctx );
int chewing_cand_ChoicePerPage( ChewingContext *ctx );
int chewing_cand_TotalChoice( ChewingContext *ctx );
int chewing_cand_CurrentPage( ChewingContext *ctx );
void chewing_cand_Enumerate( ChewingContext *ctx );
int chewing_cand_hasNext( ChewingContext *ctx );
char *chewing_cand_String( ChewingContext *ctx );

int chewing_aux_Check( ChewingContext *ctx );
int chewing_aux_Length( ChewingContext *ctx );

char *chewing_aux_String( ChewingContext *ctx );

int chewing_keystroke_CheckIgnore( ChewingContext *ctx );
int Chewing_keystroke_CheckAbsorb( ChewingContext *ctx );

#endif
