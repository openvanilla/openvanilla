/**
 * chewingio.c
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

/**
 * @file choice.c
 * @brief Choice module
 */

#include <string.h>

#include "global.h"
#include "dict.h"
#include "char.h"
#include "chewingutil.h"
#include "userphrase.h"

#define CEIL_DIV( a, b ) 	( ( a + b - 1 ) / b )


TreeType tree[ TREE_SIZE ];

void ChangeSelectIntervalAndBreakpoint( ChewingData *pgdata, int from, int to, char *str )
{
	int i;
	int user_alloc;

	IntervalType inte;

	inte.from = from;
	inte.to = to;
	for ( i = 0; i < pgdata->nSelect; i++ ) {
		if ( IsIntersect( inte, pgdata->selectInterval[ i ] ) ) {
			RemoveSelectElement( i, pgdata );
			i--;
		}
	}

	pgdata->selectInterval[ pgdata->nSelect ].from = from;
	pgdata->selectInterval[ pgdata->nSelect ].to = to;

	/* No available selection */
	if ( ( user_alloc = ( to - from ) ) == 0 )
		return;

	memcpy( 
		pgdata->selectStr[ pgdata->nSelect ], 
		str, 
		user_alloc * 2 * sizeof( char ) );
	pgdata->selectStr[ pgdata->nSelect ][ user_alloc * 2 * sizeof( char ) ] = '\0';
	pgdata->nSelect++;

	if ( user_alloc > 1 ) {
		memset( &pgdata->bUserArrBrkpt[ from + 1 ], 0, sizeof( int ) * ( user_alloc - 1 ) );
		memset( &pgdata->bUserArrCnnct[ from + 1 ], 0, sizeof( int ) * ( user_alloc - 1 ) );
	}
}

/** @brief Loading all possible phrases after the cursor from long to short into AvailInfo structure.*/
void SetAvailInfo( 
		AvailInfo *pai, const uint16 phoneSeq[], 
		int nPhoneSeq, int begin, const int bSymbolArrBrkpt[] )
{
	int end, pho_id;
	int diff;
	uint16 userPhoneSeq[ MAX_PHONE_SEQ_LEN ];

	pai->nAvail = 0;

	for ( end = begin; end < nPhoneSeq; end++ ) {
		diff = end - begin;
		if ( diff > 0 && bSymbolArrBrkpt[ end ] ) 
			break;

		pho_id = TreeFindPhrase( begin, end, phoneSeq );
		if ( pho_id != -1 ) {
			/* save it! */
			pai->avail[ pai->nAvail ].len = diff + 1;
			pai->avail[ pai->nAvail ].id = pho_id;
			pai->nAvail++;
		}
		else {
			memcpy(
				userPhoneSeq, 
				&phoneSeq[ begin ], 
				sizeof( uint16 ) * ( diff + 1 ) ) ;
			userPhoneSeq[ diff + 1 ] = 0;
			if ( UserGetPhraseFirst( userPhoneSeq ) ) {
				/* save it! */
				pai->avail[ pai->nAvail ].len = diff + 1;
				pai->avail[ pai->nAvail ].id = -1;
			} else {
				pai->avail[ pai->nAvail ].len = 0;
				pai->avail[ pai->nAvail ].id = -1;
			}
		}
	}
}

int ChoiceTheSame( ChoiceInfo *pci, char *str, int len )
{
	int i;

	for ( i = 0; i < pci->nTotalChoice; i++ )
		if ( ! memcmp( pci->totalChoiceStr[ i ], str, len ) ) 
			return 1;
	return 0;
}

/** @brief Loading all possible phrases of certain length.
 *
 *	   Loading all possible phrases of certain length into ChoiceInfo structure from static
 *	   and dynamic dictionaries,\n
 *	   including number of total pages and the number of current page.\n
 */
void SetChoiceInfo(
		ChoiceInfo *pci,AvailInfo *pai, uint16 *phoneSeq, int cursor,
		int selectAreaLen )
{
	Word tempWord;
	Phrase tempPhrase;
	int len;
	UserPhraseData *pUserPhraseData;
	uint16 userPhoneSeq[ MAX_PHONE_SEQ_LEN ];

	pci->nTotalChoice = 0;
	len = pai->avail[ pai->currentAvail ].len;

	/* secondly, read tree phrase */
	if ( len == 1 ) { /* single character */
		GetCharFirst( &tempWord, phoneSeq[ cursor ] );
		do {
			if ( ChoiceTheSame( pci, tempWord.word, 2 * sizeof( char ) ) ) 
				continue;
			memcpy( 
				pci->totalChoiceStr[ pci->nTotalChoice ],
				tempWord.word, 2 * sizeof( char ) );
			pci->totalChoiceStr[ pci->nTotalChoice ][ 2 ] = '\0';
			pci->nTotalChoice++;
		} while( GetCharNext( &tempWord ) );
	}
	/* phrase */
	else {
		if ( pai->avail[ pai->currentAvail ].id != -1 ) {
			GetPhraseFirst( &tempPhrase, pai->avail[ pai->currentAvail ].id );
			do {
				if ( ChoiceTheSame( 
					pci, 
					tempPhrase.phrase, 
					len * 2 * sizeof( char ) ) ) {
					continue;
				}
				memcpy( 
				pci->totalChoiceStr[ pci->nTotalChoice ], 
				tempPhrase.phrase, len * 2 * sizeof( char ) ) ;
				pci->totalChoiceStr[ pci->nTotalChoice ][ len * 2 ] = '\0';
				pci->nTotalChoice++;
			} while( GetPhraseNext( &tempPhrase ) );
		}

		memcpy( userPhoneSeq, &phoneSeq[ cursor ], sizeof( uint16 ) * len );
		userPhoneSeq[ len ] = 0;
		pUserPhraseData = UserGetPhraseFirst( userPhoneSeq );
		if ( pUserPhraseData ) {
			do {
				/* check if the phrase is already in the choice list */
				if ( ChoiceTheSame( 
					pci, 
					pUserPhraseData->wordSeq, 
					len * 2 * sizeof( char ) ) )
					continue;
				/* otherwise store it */
				memcpy( 
					pci->totalChoiceStr[ pci->nTotalChoice ], 
					pUserPhraseData->wordSeq, 
					sizeof( char ) * len * 2 );
				pci->totalChoiceStr[ pci->nTotalChoice ][ len * 2 ] = '\0';
				pci->nTotalChoice++;
			} while( ( pUserPhraseData = 
				UserGetPhraseNext( userPhoneSeq ) ) != NULL );
		}

	}

	/* magic number */
	pci->nChoicePerPage = ( selectAreaLen - 5 ) / ( len * 2 + 3 );
	if ( pci->nChoicePerPage > MAX_SELKEY )
		pci->nChoicePerPage = MAX_SELKEY;
	pci->nPage = CEIL_DIV( pci->nTotalChoice, pci->nChoicePerPage );
	pci->pageNo = 0;
}

/** @brief Enter choice mode and relating initialisations. */
int ChoiceFirstAvail( ChewingData *pgdata )
{
	/* save old cursor position */
	pgdata->choiceInfo.oldCursor = pgdata->cursor;
	pgdata->choiceInfo.oldChiSymbolCursor = pgdata->chiSymbolCursor;

	/* see if there is some word in the cursor position */
	if ( pgdata->nPhoneSeq == pgdata->cursor )
		pgdata->cursor--;
	if ( pgdata->chiSymbolBufLen == pgdata->chiSymbolCursor )
		pgdata->chiSymbolCursor--;

	pgdata->bSelect = 1;

	SetAvailInfo( 
		&( pgdata->availInfo ), 
		pgdata->phoneSeq, 
		pgdata->nPhoneSeq,
		pgdata->cursor, 
		pgdata->bSymbolArrBrkpt );
	pgdata->availInfo.currentAvail = pgdata->availInfo.nAvail - 1;
	/* zonble */	
	pgdata->PointStart = pgdata->chiSymbolCursor;
	pgdata->PointEnd = pgdata->availInfo.currentAvail + 1;
	
	SetChoiceInfo(
		&( pgdata->choiceInfo ), 
		&( pgdata->availInfo ), 
		pgdata->phoneSeq, 
		pgdata->cursor, 
		pgdata->config.selectAreaLen );
	return 0;
}

int ChoicePrevAvail( ChewingData *pgdata )
{
	if ( ++( pgdata->availInfo.currentAvail ) >= pgdata->availInfo.nAvail )
		pgdata->availInfo.currentAvail = 0;
	/* zonble */
	pgdata->PointStart = pgdata->cursor;
	pgdata->PointEnd = pgdata->availInfo.currentAvail + 1;
	SetChoiceInfo( 
		&( pgdata->choiceInfo ), 
		&( pgdata->availInfo ), 
		pgdata->phoneSeq, 
		pgdata->cursor,
		pgdata->config.selectAreaLen );
	return 0;
}

/** @brief Return the next phrase not longer than the previous phrase. */
int ChoiceNextAvail( ChewingData *pgdata ) 
{
	if ( --( pgdata->availInfo.currentAvail ) < 0 )
		pgdata->availInfo.currentAvail = pgdata->availInfo.nAvail - 1;
	/* zonble */
	pgdata->PointStart = pgdata->cursor;
	pgdata->PointEnd = pgdata->availInfo.currentAvail + 1;
	SetChoiceInfo(
		&( pgdata->choiceInfo ), 
		&( pgdata->availInfo ), 
		pgdata->phoneSeq,pgdata->cursor,
		pgdata->config.selectAreaLen );
	return 0;
}

int ChoiceEndChoice( ChewingData *pgdata )
{
	pgdata->bSelect = 0;
	pgdata->choiceInfo.nTotalChoice = 0;
	pgdata->choiceInfo.nPage = 0;

	if ( ! pgdata->choiceInfo.isSymbol ) {
		/* return to the old cursor & chiSymbolCursor position */
		pgdata->cursor = pgdata->choiceInfo.oldCursor;
		pgdata->chiSymbolCursor = pgdata->choiceInfo.oldChiSymbolCursor;
	} else {
		pgdata->choiceInfo.isSymbol = 0;
	}
	pgdata->PointStart = -1;
	pgdata->PointEnd = 0;
	return 0;
}

void ChangeUserData( ChewingData *pgdata, int selectNo )
{
	uint16 userPhoneSeq[ MAX_PHONE_SEQ_LEN ];
	int len;

	len = strlen( pgdata->choiceInfo.totalChoiceStr[ selectNo ] ) / 2;
	memcpy(
		userPhoneSeq, 
		&( pgdata->phoneSeq[ pgdata->cursor ] ), 
		len * sizeof( uint16 ) );
	userPhoneSeq[ len ] = 0;
	UserUpdatePhrase( userPhoneSeq, pgdata->choiceInfo.totalChoiceStr[ selectNo ] );
}

/** @brief commit the selected phrase. */
int ChoiceSelect( ChewingData *pgdata, int selectNo ) 
{
	ChoiceInfo *pci = &( pgdata->choiceInfo );
	AvailInfo *pai = &( pgdata->availInfo );

	ChangeUserData( pgdata, selectNo );
	ChangeSelectIntervalAndBreakpoint(
			pgdata,
			pgdata->cursor,
			pgdata->cursor + pai->avail[ pai->currentAvail ].len,
			pci->totalChoiceStr[ selectNo ] );
	ChoiceEndChoice( pgdata );
	return 0;
}

