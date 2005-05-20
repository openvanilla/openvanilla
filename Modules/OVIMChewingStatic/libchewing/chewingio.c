/**
 * chewingio.c
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

/**
 * @file chewingio.c
 * @brief Implement some routines for chewing
 */

#include <string.h>
#include <ctype.h>

#include "chewingio.h"
#include "global.h"
#include "zuin.h"
#include "chewingutil.h"
#include "userphrase.h"
#include "private.h"

#ifdef ENABLE_DEBUG
#include <stdio.h>
#include <assert.h>
#define FAILSAFE_OUTPUT "/tmp/chewing-debug.out"
FILE *fp_g;
#endif

void (*TerminateServices[ TerminateServicesNUM ])() = {
	NULL
};
static int countTerminateService = 0;
static int bTerminateCompleted = 0;

static char *kb_type_str[] = {
	"KB_DEFAULT",
	"KB_HSU",
	"KB_IBM",
	"KB_GIN_YIEH",
	"KB_ET",
	"KB_ET26",
	"KB_DVORAK",
	"KB_DVORAK_HSU",
	"KB_HANYU_PINYING"
};

int KBStr2Num( char str[] )
{
	int i;

	for ( i = 0; i < KB_TYPE_NUM; i++) {
		if ( ! strcmp( str, kb_type_str[ i ] ) )
			return i;
	}
	return KB_DEFAULT;
}

void SetKBType( ZuinData *pZuin, int kbtype )
{
	pZuin->kbtype = kbtype;
}

#ifdef ENABLE_DEBUG     
static void TerminateDebug()
{
	DEBUG_OUT( "DEBUG: logging service is about to terminate.\n" );
	if ( fp_g ) {
		fclose( fp_g );
	}
}               
#endif

int addTerminateService( void (*callback)() )
{       
	if ( callback ) {
		TerminateServices[ countTerminateService++ ] = callback;
		return 0;
	}
	return 1;
}

int InitChewing( void *iccf, ChewingConf *cf )
{
	ChewingData *pgdata = (ChewingData *) iccf;

#ifdef ENABLE_DEBUG
        char *dbg_path;
	int failsafe = 1;
	dbg_path = getenv( "CHEWING_DEBUG" );
	if ( dbg_path ) {
		fp_g = fopen( dbg_path, "w+" );
		if ( fp_g )
			failsafe = 0;
	}
	if ( failsafe == 1 ) {
		dbg_path = FAILSAFE_OUTPUT;
	        fp_g = fopen( dbg_path, "w+" );
		if ( ! fp_g ) {
			fprintf( stderr, 
				"Failed to record debug message in file.\n"
				"--> Output to stderr\n" );
		}
	}
	if ( fp_g )
		addTerminateService( TerminateDebug );
#endif

	/* zuinData */
	memset( &( pgdata->zuinData ), 0, sizeof( ZuinData ) );

	SetKBType( &( pgdata->zuinData ), cf->kb_type );

	/* choiceInfo */
	memset( &( pgdata->choiceInfo ), 0, sizeof( ChoiceInfo ) );

	pgdata->chiSymbolCursor = 0;
	pgdata->chiSymbolBufLen = 0;
	pgdata->nPhoneSeq = 0;
	pgdata->cursor = 0;
	memset( pgdata->bUserArrCnnct, 0, sizeof( int ) * ( MAX_PHONE_SEQ_LEN + 1 ) );
	memset( pgdata->bUserArrBrkpt, 0, sizeof( int ) * ( MAX_PHONE_SEQ_LEN + 1 ) );
	pgdata->bChiSym = CHINESE_MODE;
	pgdata->bSelect = 0;
	pgdata->nSelect = 0;
	pgdata->PointStart = -1;
	pgdata->PointEnd = 0;
	/* XXX: make options for callee to decide if use atexit or not. */
	atexit( TerminateChewing );
	return 0;
}

void TerminateChewing()
{
	int i;

	/* No terminating services are registered. */
	if ( bTerminateCompleted || countTerminateService == 0 )
		return;

	for ( i = 0; i < countTerminateService; i++ ) {
		if ( TerminateServices[ i ] ) {
#ifdef ENABLE_DEBUG
			/* Can't output to debug file because it's about to close */
			fprintf( stderr, 
				EMPHASIZE( "Terminating service #%d / %d" ) ".\n",
				i, countTerminateService );
#endif
			(*TerminateServices[ i ])();
		}
	}
	
	/* XXX: should check if the services are really completed. */
	bTerminateCompleted = 1;
	return;
}

int SetConfig( void *iccf, ConfigData *pcd )
{
	ChewingData *pgdata = (ChewingData *) iccf;

	pgdata->config.selectAreaLen = pcd->selectAreaLen;
	pgdata->config.maxChiSymbolLen = pcd->maxChiSymbolLen;
	memcpy( 
		pgdata->config.selKey,
		pcd->selKey, 
		sizeof( pcd->selKey[ 0 ] ) * MAX_SELKEY );
	pgdata->config.bAddPhraseForward = pcd->bAddPhraseForward;
	pgdata->config.bSpaceAsSelection = pcd->bSpaceAsSelection;
	
	/* Failback to default value */
	if ( (pgdata->config.bAddPhraseForward != 0) && (pgdata->config.bAddPhraseForward != 1) )
		pgdata->config.bAddPhraseForward = 0;
	if ( (pgdata->config.bSpaceAsSelection != 0) && (pgdata->config.bSpaceAsSelection != 1) )
		pgdata->config.bSpaceAsSelection = 1;
	return 0;
}

void SetChiEngMode( void *iccf, int mode )
{
	ChewingData *pgdata = (ChewingData *) iccf;

	if ( pgdata->bFirstKey == 0 ) {
		pgdata->bChiSym = mode;
		pgdata->bCaseChange = ( mode == CHINESE_MODE ? 0 : 1 );
		pgdata->bFirstKey = 1;
	}
}

int GetChiEngMode( void *iccf ) 
{
	return ( (ChewingData *) iccf )->bChiSym;
}

static void CheckAndResetRange( ChewingData *pgdata )
{
	if ( pgdata->PointStart > -1 ) {
		pgdata->PointStart = -1;
		pgdata->PointEnd = 0;
	}
}

static int DoSelect( ChewingData *pgdata, int num )
{
	if ( num >= 0 ) {
		num += pgdata->choiceInfo.pageNo * pgdata->choiceInfo.nChoicePerPage;
		/* Note: if num is larger than the total, there will be big troubles. */
		if ( num < pgdata->choiceInfo.nTotalChoice ) {
			if ( pgdata->choiceInfo.isSymbol == 1 ) {
				SymbolChoice( pgdata, num );
			} else { 
				/* change the select interval & selectStr & nSelect */
				AddSelect( pgdata, num );
				/* second, call choice module */
				ChoiceSelect( pgdata, num );
			}
			return 1;
		}
	}
	return 0;
}

int OnKeySpace( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;
	int toSelect = 0;
	int rtn;

	/* check if Old Chewing style */
	if (0) {						// lukhnos: ALWAYS USE SPACE
		return OnKeyDefault( pgdata, ' ', pgo );
	}

	CheckAndResetRange( pgdata );

	if ( pgdata->bSelect ) {
		if ( pgdata->choiceInfo.pageNo < ( pgdata->choiceInfo.nPage - 1 ) ) {
			return OnKeyRight( iccf, pgo );
		}
	}

	if ( ! ChewingIsEntering( pgdata ) ) {
		rtn = SymbolInput( ' ', pgdata );
		pgo->commitStr[ 0 ].wch = pgdata->chiSymbolBuf[ 0 ].wch;
		pgo->nCommitStr = 1;
		pgdata->chiSymbolBufLen = 0;
		pgdata->chiSymbolCursor = 0;
		keystrokeRtn = KEYSTROKE_COMMIT;
	} else if ( pgdata->bChiSym != CHINESE_MODE ) {
		rtn = SymbolInput( ' ', pgdata );
		keystrokeRtn = KEYSTROKE_ABSORB;
	} else {
		rtn = ZuinPhoInput( &( pgdata->zuinData ), ' ' );
		switch ( rtn ) {
			case ZUIN_ABSORB:
				keystrokeRtn = KEYSTROKE_ABSORB;
				break;
			case ZUIN_COMMIT:
				AddChi( pgdata->zuinData.phone, pgdata );
				CallPhrasing( pgdata );
				break;
			case ZUIN_NO_WORD:
				keystrokeRtn = KEYSTROKE_BELL | KEYSTROKE_ABSORB;
				break;
			case ZUIN_KEY_ERROR:
			case ZUIN_IGNORE:
				if ( pgdata->chiSymbolCursor == pgdata->chiSymbolBufLen ) {
					if (
						ChewingIsChiAt(
							pgdata->chiSymbolCursor - 1,
							pgdata ) ) {
						toSelect = 1;
					}
				} else {
					if (
						ChewingIsChiAt(
							pgdata->chiSymbolCursor,
							pgdata ) ) {
						toSelect = 1;
					}
				}

				if ( toSelect ) {
					if ( ! pgdata->bSelect )
						ChoiceFirstAvail( pgdata );
					else
						ChoiceNextAvail( pgdata );
				}
				break;
		}
	}

	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyEsc( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	CheckAndResetRange( pgdata );

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	} else if ( pgdata->bSelect ) {
		if ( pgdata->choiceInfo.isSymbol == 1 ) {
			ChewingKillChar(
				pgdata, 
				pgdata->cursor - 1, 
				pgdata->chiSymbolCursor - 1,
				DECREASE_CURSOR );
		}
		ChoiceEndChoice( pgdata );
	} else if ( ZuinIsEntering( &( pgdata->zuinData ) ) ) {
		ZuinRemoveAll( &( pgdata->zuinData ) );
	}

	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyEnter( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int nCommitStr = pgdata->chiSymbolBufLen;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	} else if ( pgdata->bSelect ) {
		keystrokeRtn = KEYSTROKE_ABSORB | KEYSTROKE_BELL;
	} else if ( pgdata->PointStart > -1) {
		int buf = pgdata->cursor;
		int key;
		if ( pgdata->PointEnd > 0 ) {
			if ( !pgdata->config.bAddPhraseForward ) {
				pgdata->cursor = pgdata->PointStart;
				key = '0' + pgdata->PointEnd;
			} else {
				pgdata->cursor = pgdata->PointStart + pgdata->PointEnd;
				key = '0' + pgdata->PointEnd;
			}
			OnKeyCtrlNum( (void *) pgdata, key, pgo );
			pgdata->cursor = buf;
		} else if ( pgdata->PointEnd < 0 ) {
			if ( pgdata->config.bAddPhraseForward )
				pgdata->cursor = buf - pgdata->PointEnd;
			key = '0' - pgdata->PointEnd;
			OnKeyCtrlNum( (void *) pgdata, key, pgo );
			pgdata->cursor = buf;
		}
		pgdata->PointStart = -1;
		pgdata->PointEnd = 0;
	} else {
		keystrokeRtn = KEYSTROKE_COMMIT;
		WriteChiSymbolToBuf( pgo->commitStr, nCommitStr, pgdata );
		AutoLearnPhrase( pgdata );
		CleanAllBuf( pgdata );  
		CallPhrasing( pgdata );
		pgo->nCommitStr = nCommitStr;
	}

	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyDel( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	CheckAndResetRange( pgdata );

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	}

	if ( ! pgdata->bSelect ) {
		if ( 
			! ZuinIsEntering( &( pgdata->zuinData ) ) && 
			pgdata->chiSymbolCursor < pgdata->chiSymbolBufLen ) {
			ChewingKillChar(
				pgdata, 
				pgdata->cursor, 
				pgdata->chiSymbolCursor, 
				NONDECREASE_CURSOR );
		}
		CallPhrasing( pgdata );
	}
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyBackspace( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	CheckAndResetRange( pgdata );

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	}

	if ( ! pgdata->bSelect ) {
		if ( ZuinIsEntering( &( pgdata->zuinData ) ) ) {
			ZuinRemoveLast( &( pgdata->zuinData ) );
		}
		else if ( pgdata->chiSymbolCursor > 0 ) {
			ChewingKillChar(
				pgdata, 
				pgdata->cursor - 1, 
				pgdata->chiSymbolCursor - 1,
				DECREASE_CURSOR );
		}
		CallPhrasing( pgdata );
	}
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );

	return 0;
}

int OnKeyUp( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	CheckAndResetRange( pgdata );

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	}

	if ( pgdata->bSelect )
		ChoicePrevAvail( pgdata );

	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyDown( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int toSelect = 0;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	CheckAndResetRange( pgdata );

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	}

	/* see if to select */
	if ( pgdata->chiSymbolCursor == pgdata->chiSymbolBufLen ) {
		if ( ChewingIsChiAt( pgdata->chiSymbolCursor - 1, pgdata ) )
			toSelect = 1;
	} else {
		if ( ChewingIsChiAt( pgdata->chiSymbolCursor, pgdata ) ) 
			toSelect = 1;
	}

	if ( toSelect ) {
		if( ! pgdata->bSelect ) {
			ChoiceFirstAvail( pgdata );
		} else {
			ChoiceNextAvail( pgdata );
		}
	}

	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

/* Add phrase in Hanin Style */
int OnKeyShiftLeft( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *)iccf ;
	int keystrokeRtn = KEYSTROKE_ABSORB ;

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE ;
	} 
	if(!pgdata->bSelect ) {
		/*  PointEnd locates (-9, +9) */
		if ( 
			! ZuinIsEntering( &( pgdata->zuinData ) ) && 
			pgdata->chiSymbolCursor > 0
			&& pgdata->PointEnd > -9 ) {
			pgdata->chiSymbolCursor--;
			if ( pgdata->PointStart == -1 )
				pgdata->PointStart = pgdata->cursor;
			if (
				pgdata->cursor > 0 && 
				ChewingIsChiAt( pgdata->chiSymbolCursor, pgdata ) ) {
				pgdata->cursor--;
				pgdata->PointEnd--;
			}
			if ( pgdata->PointEnd == 0 )
				pgdata->PointStart = -1; 
		}
	}

	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyLeft( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	}

	if ( pgdata->bSelect ) {
		if ( pgdata->choiceInfo.pageNo > 0 )
			pgdata->choiceInfo.pageNo--;
		else
			pgdata->choiceInfo.pageNo = pgdata->choiceInfo.nPage - 1;
	}
	else {
		if ( 
			! ZuinIsEntering( &( pgdata->zuinData ) ) && 
			pgdata->chiSymbolCursor > 0 ) {
			CheckAndResetRange( pgdata );
			pgdata->chiSymbolCursor--;
			if ( 
				pgdata->cursor > 0 && 
				ChewingIsChiAt( pgdata->chiSymbolCursor, pgdata ) )
				pgdata->cursor--;
		}
	}
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

/* Add phrase in Hanin Style */
int OnKeyShiftRight(void *iccf, ChewingOutput *pgo)
{
	ChewingData *pgdata = (ChewingData *)iccf ;
	int keystrokeRtn = KEYSTROKE_ABSORB ;

	if( ! ChewingIsEntering(pgdata)) {
		keystrokeRtn = KEYSTROKE_IGNORE ;
	} 

	if( ! pgdata->bSelect) {
		/* PointEnd locates (-9, +9) */
		if ( 
			! ZuinIsEntering( &( pgdata->zuinData ) ) && 
			pgdata->chiSymbolCursor < pgdata->chiSymbolBufLen && 
			pgdata->PointEnd < 9 ) {
			if ( pgdata->PointStart == -1 )
				pgdata->PointStart = pgdata->cursor;
			if (
				pgdata->cursor < pgdata->nPhoneSeq && 
				ChewingIsChiAt( pgdata->chiSymbolCursor, pgdata ) ) {
				pgdata->cursor++;
				pgdata->PointEnd++;
			}
			pgdata->chiSymbolCursor++;
			if ( pgdata->PointEnd == 0 )
				pgdata->PointStart = -1; 
		}
	}

	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyRight( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	}

	if ( pgdata->bSelect ) {
		if ( pgdata->choiceInfo.pageNo < pgdata->choiceInfo.nPage - 1)
			pgdata->choiceInfo.pageNo++;
		else
			pgdata->choiceInfo.pageNo = 0;
	}
	else {
		if ( 
			! ZuinIsEntering( &( pgdata->zuinData ) ) &&			
			pgdata->chiSymbolCursor < pgdata->chiSymbolBufLen ) {			
			CheckAndResetRange( pgdata );
			if ( 
				pgdata->cursor < pgdata->nPhoneSeq && 
				ChewingIsChiAt( pgdata->chiSymbolCursor, pgdata ) )
				pgdata->cursor++;
			pgdata->chiSymbolCursor++;
		}
	}

	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyTab( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	CheckAndResetRange( pgdata );

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	}


	if ( ! pgdata->bSelect ) {
		if ( ChewingIsChiAt( pgdata->chiSymbolCursor - 1, pgdata ) ) {
			if ( IsPreferIntervalConnted( pgdata->cursor, pgdata) ) {
				pgdata->bUserArrBrkpt[ pgdata->cursor ] = 1;
				pgdata->bUserArrCnnct[ pgdata->cursor ] = 0;
			}
			else {
				pgdata->bUserArrBrkpt[ pgdata->cursor ] = 0;
				pgdata->bUserArrCnnct[ pgdata->cursor ] = 1;
			}
		}
		CallPhrasing( pgdata );
	}
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyDblTab( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	CheckAndResetRange( pgdata );

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
	}

	if ( ! pgdata->bSelect ) {
		pgdata->bUserArrBrkpt[ pgdata->cursor ] = 0;
		pgdata->bUserArrCnnct[ pgdata->cursor ] = 0;
	}
	CallPhrasing( pgdata );

	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}


int OnKeyCapslock( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;

	pgdata->bChiSym = 1 - pgdata->bChiSym;
	pgdata->bCaseChange = ( pgdata->bChiSym == CHINESE_MODE ? 0 : 1 );
	MakeOutputWithRtn( pgo, pgdata, KEYSTROKE_ABSORB );
	return 0;
}

int OnKeyHome( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	CheckAndResetRange( pgdata );

	if ( ! ChewingIsEntering( pgdata ) ) { 
		keystrokeRtn = KEYSTROKE_IGNORE;
	} 
	else if ( ! pgdata->bSelect ) {
		pgdata->chiSymbolCursor = 0; 
		pgdata->cursor = 0;
	}
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyEnd( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	CheckAndResetRange( pgdata );

	if ( ! ChewingIsEntering( pgdata ) ) {
		keystrokeRtn = KEYSTROKE_IGNORE; 
	} 
	else if ( ! pgdata->bSelect ) {
		pgdata->chiSymbolCursor = pgdata->chiSymbolBufLen; 
		pgdata->cursor = pgdata->nPhoneSeq; 
	} 
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );       
	return 0;
}

/* Dvorak <-> Qwerty keyboard layout converter */
static int dvorak_convert( int key )
{
	char dkey[] = {
		'\'','\"',',','<','.','>','p','P','y','Y','f','F','g','G',
		'c','C','r','R','l','L','/','?','=','+','\\','|',
		'a','A','o','O','e','E','u','U','i','I','d','D','h','H',
		't','T','n','N','s','S','-','_',
		';',':','q','Q','j','J','k','K','x','X','b','B','m','M',
		'w','W','v','V','z','Z'};
	char qkey[] = {
		'Q','q','w','W','e','E','r','R','t','T','y','Y','u','U',
		'i','I','o','O','p','P','[','{',']','}','\\','|',
		'a','A','s','S','d','D','f','F','g','G','h','H','j','J',
		'k','K','l','L',';',':','\'','\"',
		'z','Z','x','X','c','C','v','V','b','B','n','N','m','M',
		',','<','.','>','/','?'};
	int i = 0, Total = 67;

	for ( i = 0; i < Total; i++ ) {
		if ( key == qkey[ i ] ) {
			key = dkey[ i ];
			return key;
		}
	}
	return key;
}

int OnKeyDefault( void *iccf, int key, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int rtn, num;
	int keystrokeRtn = KEYSTROKE_ABSORB;
	int bQuickCommit = 0;

	/* Skip the special key */
	if ( key & 0xFF00 ) {
		keystrokeRtn = KEYSTROKE_IGNORE;
		goto End_OnKeyDefault;
	}

	CheckAndResetRange( pgdata );

	DEBUG_CHECKPOINT();
	DEBUG_OUT( "   key=%d\n", key );

	/* Dvorak Hsu */
	if ( pgdata->zuinData.kbtype == KB_DVORAK_HSU ) {
		key = dvorak_convert( key );
	}

	/* selecting */
	if ( pgdata->bSelect ) {
		if ( key == ' ' )
			return OnKeyRight( iccf, pgo );
		/* num starts from 0 */
		num = CountSelKeyNum( key, pgdata );
		DoSelect( pgdata, num );
	}
	/* editing */
	else {
		if ( pgdata->bChiSym == CHINESE_MODE ) {
			rtn = ZuinPhoInput( &( pgdata->zuinData ), key );
			DEBUG_OUT(
				"\t\tchinese mode key, "
				"ZuinPhoInput return value = %d\n", 
				rtn );
			DEBUG_FLUSH;
			
			if ( rtn == ZUIN_KEY_ERROR )
				rtn = SpecialSymbolInput( key, pgdata );
			switch ( rtn ) {
				case ZUIN_ABSORB:
					keystrokeRtn = KEYSTROKE_ABSORB;
					break;
				case ZUIN_COMMIT:
					AddChi( pgdata->zuinData.phone, pgdata );
					break;
				case ZUIN_NO_WORD:
					keystrokeRtn = KEYSTROKE_BELL | KEYSTROKE_ABSORB;
					break;
				case ZUIN_KEY_ERROR:
				case ZUIN_IGNORE:
					DEBUG_OUT(
						"\t\tbefore isupper(key),key=%d\n", 
						key );
					/* change upper case into lower case */
					if ( isupper( key ) ) 
						key = tolower( key );

					DEBUG_OUT(
						"\t\tafter isupper(key),key=%d\n", 
						key );

					/* see if buffer contains nothing */
					if ( pgdata->chiSymbolBufLen == 0 ) {
						bQuickCommit = 1;
					}

					rtn = SymbolInput( key, pgdata );
					if ( rtn == SYMBOL_KEY_ERROR ) {
						keystrokeRtn = KEYSTROKE_IGNORE;
						/*
						 * If the key is not a printable symbol, 
						 * then it's wrongto commit it.
						 */
						bQuickCommit = 0;
					} else
						keystrokeRtn = KEYSTROKE_ABSORB;

					break;

			}
		}
		/* English mode */
		else {
			/* see if buffer contains nothing */
			if ( pgdata->chiSymbolBufLen == 0 ) {
				bQuickCommit = 1;
			}
			rtn = SymbolInput( key, pgdata );
			if ( rtn == SYMBOL_KEY_ERROR ) {
				keystrokeRtn = KEYSTROKE_IGNORE;
				bQuickCommit = 0;
			}
		}
		if ( ! bQuickCommit ) {
			CallPhrasing( pgdata );
			if( ReleaseChiSymbolBuf( pgdata, pgo ) != 0 )
				keystrokeRtn = KEYSTROKE_COMMIT;
		}
		/* Quick commit */
		else {
			DEBUG_OUT(
				"\t\tQuick commit buf[0]=%c\n", 
				pgdata->chiSymbolBuf[ 0 ].s[ 0 ] );
			pgo->commitStr[ 0 ].wch = pgdata->chiSymbolBuf[ 0 ].wch; 
			pgo->nCommitStr = 1;
			pgdata->chiSymbolBufLen = 0;
			pgdata->chiSymbolCursor = 0;
			keystrokeRtn = KEYSTROKE_COMMIT;
		}
	}

End_OnKeyDefault:
	CallPhrasing( pgdata );
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyCtrlNum( void *iccf, int key, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int keystrokeRtn = KEYSTROKE_ABSORB;
	int newPhraseLen;
	int i;
	uint16 addPhoneSeq[ MAX_PHONE_SEQ_LEN ];
	char addWordSeq[ MAX_PHONE_SEQ_LEN * 2 + 1 ];
	int phraseState;

	CheckAndResetRange( pgdata );

	CallPhrasing( pgdata );
	newPhraseLen = key - '0';

	if ( ( key == '0' || key == '1') && ! pgdata->bSelect ) {
		pgdata->bSelect = 1;  
		HaninSymbolInput(
			&( pgdata->choiceInfo ), 
			&( pgdata->availInfo ), 
			pgdata->phoneSeq, 
			pgdata->config.selectAreaLen ); 
		SemiSymbolInput('1', pgdata);
                CallPhrasing( pgdata );
                MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
                return 0;
	}

        if ( ! pgdata->config.bAddPhraseForward ) {
		if ( 
			newPhraseLen >= 1 && 
			pgdata->cursor + newPhraseLen - 1 <= pgdata->nPhoneSeq ) {
			if ( NoSymbolBetween( 
				pgdata, 
				pgdata->cursor, 
				pgdata->cursor + newPhraseLen - 1 ) ) {
				/* Manually add phrase to the user phrase database. */
				memcpy(
					addPhoneSeq,
					&pgdata->phoneSeq[ pgdata->cursor ],
					sizeof( uint16 ) * newPhraseLen );
				addPhoneSeq[ newPhraseLen ] = 0;
				memcpy(
					addWordSeq,
					&pgdata->phrOut.chiBuf[ pgdata->cursor * 2 ],
					sizeof( char ) * 2 * newPhraseLen );
				addWordSeq[ newPhraseLen * 2 ] = '\0';

				phraseState = UserUpdatePhrase( addPhoneSeq, addWordSeq );
				SetUpdatePhraseMsg( 
					pgdata, 
					addWordSeq, 
					newPhraseLen, 
					phraseState );

				/* Clear the breakpoint between the New Phrase */
				for ( i = 1; i < newPhraseLen; i++ )
					pgdata->bUserArrBrkpt[ pgdata->cursor + i ] = 0;
			}
		}
	}
	else {
		if ( 
			newPhraseLen >= 1 && 
			pgdata->cursor - newPhraseLen >= 0 ) {
			if ( NoSymbolBetween( 
				pgdata, 
				pgdata->cursor, 
				pgdata->cursor - newPhraseLen ) ) {
				/* Manually add phrase to the user phrase database. */
				memcpy(
					addPhoneSeq,
					&pgdata->phoneSeq[ pgdata->cursor - newPhraseLen ],
					sizeof( uint16 ) * newPhraseLen );
				addPhoneSeq[ newPhraseLen ] = 0;
				memcpy(
					addWordSeq,
					&pgdata->phrOut.chiBuf[ ( pgdata->cursor - newPhraseLen ) * 2 ],
					sizeof( char ) * 2 * newPhraseLen );
				addWordSeq[ newPhraseLen * 2 ] = '\0';

				phraseState = UserUpdatePhrase( addPhoneSeq, addWordSeq );
				SetUpdatePhraseMsg( 
					pgdata, 
					addWordSeq, 
					newPhraseLen, 
					phraseState );

				/* Clear the breakpoint between the New Phrase */
				for ( i = 1; i < newPhraseLen; i++ )
					pgdata->bUserArrBrkpt[ pgdata->cursor - newPhraseLen + i ] = 0;
			}
		}
	}
	CallPhrasing( pgdata );
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	MakeOutputAddMsgAndCleanInterval( pgo, pgdata );
	return 0;
}

int OnKeyCtrlOption( void *iccf, int key, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *)iccf;
	int rtn;
	int keystrokeRtn = KEYSTROKE_ABSORB;

	if ( ! pgdata->bSelect ) {
		CheckAndResetRange( pgdata );
		rtn = SpecialEtenSymbolInput( key, pgdata );
	}
	CallPhrasing( pgdata );
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyShiftSpace( void *iccf, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int rtn, key = ' ';
	int keystrokeRtn = KEYSTROKE_ABSORB;

	if ( ! pgdata->bSelect ) {
		CheckAndResetRange( pgdata );
		rtn = SemiSymbolInput( key, pgdata );
	}
	CallPhrasing( pgdata );
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

int OnKeyNumlock( void *iccf, int key, ChewingOutput *pgo )
{
	ChewingData *pgdata = (ChewingData *) iccf;
	int rtn, QuickCommit = 0;
	int keystrokeRtn = KEYSTROKE_ABSORB;
	
	if ( ! pgdata->bSelect ) {
		/* If we're not selecting words, we should send out numeric
		 * characters at once. 
		 */
		if ( pgdata->chiSymbolBufLen == 0 ) {
			QuickCommit = 1;
		}
		rtn = SymbolInput( key, pgdata );
		/* copied from OnKey Default */
		if ( rtn == SYMBOL_KEY_ERROR ) {
			keystrokeRtn = KEYSTROKE_IGNORE ;
		} else if ( QuickCommit ) {
			pgo->commitStr[ 0 ].wch = pgdata->chiSymbolBuf[ 0 ].wch; 
			pgo->nCommitStr = 1;
			pgdata->chiSymbolBufLen = 0;
			pgdata->chiSymbolCursor = 0;
			keystrokeRtn = KEYSTROKE_COMMIT;
		}
	} else {
		/* Otherwise, if we are selecting words, we use numeric keys
		 * as selkey
		 * and submit the words. 
		 */
		int num = -1;
		if ( key > '0' && key < '8' )
			num = key - '1';
		DoSelect( pgdata, num );
 	}
	CallPhrasing( pgdata );
	MakeOutputWithRtn( pgo, pgdata, keystrokeRtn );
	return 0;
}

/* Local Variables: */
/* c-indentation-style: linux */
/* indent-tabs-mode: t */
/* End: */
