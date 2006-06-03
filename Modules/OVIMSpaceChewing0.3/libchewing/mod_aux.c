/**
 * mod_aux.c
 *
 * Copyright (c) 2005, 2006
 *	libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

/**
 * @file mod_aux.c
 * @brief Auxiliary module
 */

#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "chewingio.h"
#include "private.h"

/**
 * @param ctx handle to Chewing IM context
 * @retval TRUE if it currnet input state is at the "end-of-a-char"
 */
CHEWING_API int chewing_commit_Check( ChewingContext *ctx )
{
	return (ctx->output->keystrokeRtn & KEYSTROKE_COMMIT);
}

/**
 * @param ctx handle to Chewing IM context
 *
 * retrun current commit string, regardless current input state.
 * Alwasy returns a char pointer, caller must free it.
 */
CHEWING_API char *chewing_commit_String( ChewingContext *ctx )
{
	int i;
	char *s = (char *) calloc(
		1 + ctx->output->nCommitStr,
		sizeof(char) * MAX_UTF8_SIZE );
	for ( i = 0; i < ctx->output->nCommitStr; i++ ) {
		strcat( s, (char *) (ctx->output->commitStr[ i ].s) );
	}
	return s;
}

CHEWING_API int chewing_buffer_Check( ChewingContext *ctx )
{
	return (ctx->output->chiSymbolBufLen != 0);
}

/* Hacked by zonble */

CHEWING_API char *chewing_buffer_String( ChewingContext *ctx, int from, int to )
{
	int i;
	char *s = (char *) calloc(
		1 + ctx->output->chiSymbolBufLen,
		sizeof(char) * MAX_UTF8_SIZE );
	if(from >= 0 && to < ctx->output->chiSymbolBufLen ) {
	   for ( i = from; i <= to; i++ ) {
	      strcat( s, (char *) (ctx->output->chiSymbolBuf[ i ].s) );
	   }
	}
	return s;
}

CHEWING_API char *chewing_buffer_String_End( ChewingContext *ctx, int from) {
	return chewing_buffer_String(ctx, from, ctx->output->chiSymbolBufLen -1 );
}

/**
 * @param ctx handle to Chewing IM context
 * @param zuin_count pointer to the integer of available Zuin preedit string
 *
 * Always returns a char pointer, caller must free it.
 */
CHEWING_API char *chewing_zuin_String( ChewingContext *ctx, int *zuin_count )
{
	int i;
	*zuin_count = 0;
	char *s = (char*) calloc(
		1 + ZUIN_SIZE,
		sizeof(char) * WCH_SIZE );
	for ( i = 0; i < ZUIN_SIZE; i++ ) {
		if ( ctx->output->zuinBuf[ i ].s[ 0 ] != '\0' ) {
			strcat( s, (char *) (ctx->output->zuinBuf[ i ].s) );
			zuin_count++;
		}
	}
	return s;
}

CHEWING_API int chewing_zuin_Check( ChewingContext *ctx )
{
	int ret = 0;
	if ( ctx->output->zuinBuf[ 0 ].s[ 0 ] == '\0' ) {
		ret = 1;
	}
	return ret;
}

CHEWING_API int chewing_cursor_Current( ChewingContext *ctx )
{
	return (ctx->output->chiSymbolCursor);
}

CHEWING_API int chewing_point_Start( ChewingContext *ctx )
{
	return (ctx->output->PointStart);
}

CHEWING_API int chewing_point_End( ChewingContext *ctx )
{
	return (ctx->output->PointEnd);
}


CHEWING_API int chewing_cand_CheckDone( ChewingContext *ctx )
{
	return (! ctx->output->pci);
}

CHEWING_API int chewing_cand_TotalPage( ChewingContext *ctx )
{
	return (ctx->output->pci ? ctx->output->pci->nPage : 0);
}

CHEWING_API int chewing_cand_ChoicePerPage( ChewingContext *ctx )
{
	return (ctx->output->pci ? ctx->output->pci->nChoicePerPage : 0);
}

CHEWING_API int chewing_cand_TotalChoice( ChewingContext *ctx )
{
	return (ctx->output->pci ? ctx->output->pci->nTotalChoice : 0);
}

CHEWING_API int chewing_cand_CurrentPage( ChewingContext *ctx )
{
	return (ctx->output->pci ? ctx->output->pci->pageNo : -1);
}

static int cand_no_max = 9999;
CHEWING_API void chewing_cand_Enumerate( ChewingContext *ctx )
{
	ctx->cand_no = ctx->output->pci->pageNo * ctx->output->pci->nChoicePerPage;
	cand_no_max = ctx->output->pci->nTotalChoice;
}

CHEWING_API int chewing_cand_hasNext( ChewingContext *ctx )
{
	return (ctx->cand_no < ctx->output->pci->nTotalChoice);	
}

CHEWING_API char *chewing_cand_String( ChewingContext *ctx )
{
	char *s = strdup( "" );
	if ( chewing_cand_hasNext( ctx ) || (ctx->cand_no < cand_no_max) ) {
		s = strdup( ctx->output->pci->totalChoiceStr[ ctx->cand_no ] );
		ctx->cand_no++;
	}
	return s;
}

CHEWING_API int chewing_aux_Check( ChewingContext *ctx )
{
	return (ctx->output->bShowMsg);
}

CHEWING_API int chewing_aux_Length( ChewingContext *ctx )
{
	return (ctx->output->bShowMsg ? ctx->output->showMsgLen : 0);
}

CHEWING_API char *chewing_aux_String( ChewingContext *ctx )
{
	int i;
	char *msg = (char *) calloc(
		1 + ctx->output->showMsgLen,
		sizeof(char) * MAX_UTF8_SIZE );
	for ( i = 0; i < ctx->output->showMsgLen; ++i )
		strcat( msg, (char *)(ctx->output->showMsg[ i ].s) );
	return msg;

}

CHEWING_API int chewing_keystroke_CheckIgnore( ChewingContext *ctx )
{ 
	  return (ctx->output->keystrokeRtn & KEYSTROKE_IGNORE);
} 

CHEWING_API int Chewing_keystroke_CheckAbsorb( ChewingContext *ctx )
{ 
	  return (ctx->output->keystrokeRtn & KEYSTROKE_ABSORB);
}
