/**
 * chewingutil.c
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

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "global.h"
#include "chewingutil.h"
#include "zuin.h"
#include "userphrase.h"
#include "private.h"

extern const char *ph_pho[]; 
static void MakePreferInterval( ChewingData *pgdata );
static void ShiftInterval( ChewingOutput *pgo, ChewingData *pgdata );

void SetUpdatePhraseMsg( ChewingData *pgdata, char *addWordSeq, int len, int state )
{
	char *insert = "加入－", *modify = "已有－";
	int begin = 3, i;

	pgdata->showMsgLen = begin + len;
	if ( state == USER_UPDATE_INSERT ) {
		pgdata->showMsg[ 0 ].s[ 0 ] = insert[ 0 ];
		pgdata->showMsg[ 0 ].s[ 1 ] = insert[ 1 ];
		pgdata->showMsg[ 0 ].s[ 2 ] = 0;
		pgdata->showMsg[ 1 ].s[ 0 ] = insert[ 2 ];
		pgdata->showMsg[ 1 ].s[ 1 ] = insert[ 3 ];
		pgdata->showMsg[ 1 ].s[ 2 ] = 0;
		pgdata->showMsg[ 2 ].s[ 0 ] = insert[ 4 ];
		pgdata->showMsg[ 2 ].s[ 1 ] = insert[ 5 ];
		pgdata->showMsg[ 2 ].s[ 2 ] = 0 ;
	}
	else {
		pgdata->showMsg[ 0 ].s[ 0 ] = modify[ 0 ];
		pgdata->showMsg[ 0 ].s[ 1 ] = modify[ 1 ];
		pgdata->showMsg[ 0 ].s[ 2 ] = 0;
		pgdata->showMsg[ 1 ].s[ 0 ] = modify[ 2 ];
		pgdata->showMsg[ 1 ].s[ 1 ] = modify[ 3 ];
		pgdata->showMsg[ 1 ].s[ 2 ] = 0;
		pgdata->showMsg[ 2 ].s[ 0 ] = modify[ 4 ];
		pgdata->showMsg[ 2 ].s[ 1 ] = modify[ 5 ];
		pgdata->showMsg[ 2 ].s[ 2 ] = 0;
	}
	for ( i = 0; i < len; i++ ) {
		pgdata->showMsg[ begin + i ].s[ 0 ] = addWordSeq[ i * 2 ];
		pgdata->showMsg[ begin + i ].s[ 1 ] = addWordSeq[ i * 2 + 1 ];
		pgdata->showMsg[ begin + i ].s[ 2 ] = 0 ;
	}
	pgdata->showMsg[ begin + i].s[ 0 ] = 0;
	pgdata->showMsg[ begin + i].s[ 1 ] = 0;
	pgdata->showMsg[ begin + i].s[ 2 ] = 0;
}

int NoSymbolBetween( ChewingData *pgdata, int begin, int end )
{
	int i, nChi, k;

	/* find the beginning index in the chiSymbolBuf */
	for ( nChi = i = 0; i < pgdata->chiSymbolBufLen && nChi < begin; i++ )
		/* it is Chinese word */
		if ( pgdata->chiSymbolBuf[ i ].wch == (wchar_t) 0 )
			nChi++;

	for ( k = i + 1; k < pgdata->chiSymbolBufLen && k <= end; k++ )
		/*  not a Chinese word */
		if ( pgdata->chiSymbolBuf[ i ].wch != (wchar_t) 0 )
			return 0;

	return 1;
}

int ChewingIsEntering( ChewingData *pgdata )
{
	if ( pgdata->choiceInfo.isSymbol )
		return 1;
	return (
		pgdata->chiSymbolBufLen != 0 || 
		ZuinIsEntering( &( pgdata->zuinData ) ) );
}

#define CEIL_DIV(a,b) ((a+b-1)/b)

int HaninSymbolInput(ChoiceInfo *pci, AvailInfo *pai, const uint16 phoneSeq[],	int selectAreaLen) {
    static char *chibuf[] = { "，","、","。","．","；","：",
                              "？","！","︰","?","…","‥","｜","—", "︴","﹏\",
		"（","）","︵","︶","《","》","︽","︾",
		"〈","〉","︿","﹀","【","】","︻","︼",
		"｛","｝","︷","︸","〔","〕","︹","︺",
		"「","」","﹁","﹂","『","』","﹃","﹄",
		"＃","＆","＊","※","§","〃","○","●",
		"◎","㊣","△","▽","▲","▼","∴","∵",
		"☆","★","◇","◆","□","■","♀","♂",
		"→","←","↑","↓","↗","↖","↘","↙",
		"㏕","㎜","㎝","㎞","㏎","㎡","㎎","㎏",
		"㏄","℃","℉","°","±","×","÷","≒",
		"≠","≦","≧","～","∠","⊥","∟","≡",
		"⊿","∞","√","?","┌","┬","┐","├",
		"┼","┤","└","┴","┘","─","│","═",
		"╞","╪","╡","╱","╲","╳","╭","╮",
		"╰","╯","▁","▂","▃","▄","▅","▆",
		"▇","█","▏","▎","▍","▌","▋","▊",
		"▉","▔","〡","〢","〣","〤","〥","〦",
		"〧","〨","〩","十","卄","卅","Ⅰ","Ⅱ",
		"Ⅲ","Ⅳ","Ⅴ","Ⅵ","Ⅶ","Ⅷ","Ⅸ","Ⅹ",
		"Α","Β","Γ","Δ","Ε","Ζ","Η","Θ",
		"Ι","Κ","Λ","Μ","Ν","Ξ","Ο","Π",
		"Ρ","Σ","Τ","Υ","Φ","Χ","Ψ","Ω",
		"α\","β","γ","δ","ε","ζ","η","θ",
                              "ι","κ","λ","μ","ν","ξ","ο","π",
                              "ρ","σ","τ","υ","φ","χ","ψ","ω"};
    int i, all = 216;

    pci->nTotalChoice = 0;
    for(i = 0; i< all; i++){
        memcpy( pci->totalChoiceStr[ pci->nTotalChoice],
                chibuf[i], sizeof(char)*2) ;
        pci->totalChoiceStr[ pci->nTotalChoice][2] = '\0' ;
        pci->nTotalChoice++; 
    }  
    pai->avail[1].len = 1;
    pai->avail[1].id = -1;  
    pai->nAvail = all;
    pai->currentAvail = 1;
    pci->nChoicePerPage = (selectAreaLen - 5) / ( 2 + 3) ;
    if(pci->nChoicePerPage > MAX_SELKEY) pci->nChoicePerPage = MAX_SELKEY ;
    pci->nPage = CEIL_DIV(pci->nTotalChoice, pci->nChoicePerPage) ;
    pci->pageNo = 0 ;
    pci->isSymbol = 1;
    return ZUIN_ABSORB;
}

static int InternalSpecialSymbol(
		int key, ChewingData *pgdata,
		int nSpecial, char keybuf[], char *chibuf[] )
{
	int i, rtn = ZUIN_IGNORE; /* very strange , and very difficult to understand */
        int kbtype;

	for ( i = 0; i < nSpecial; i++ ) {
		if ( key == keybuf[ i ] ) {
			rtn = ZUIN_ABSORB;
			memmove( 
				&( pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor + 1 ] ),
				&( pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ] ),
				sizeof( wch_t ) * 
				( pgdata->chiSymbolBufLen - pgdata->chiSymbolCursor ) );

			pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ].wch = (wchar_t) 0;
			pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ].s[ 0 ] = 
				chibuf[ i ][ 0 ];
			pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ].s[ 1 ] = 
				chibuf[ i ][ 1 ];
			pgdata->chiSymbolCursor++;
			pgdata->chiSymbolBufLen++;
			pgdata->bUserArrCnnct[ pgdata->cursor ] = 0;
			/* reset Zuin data */
                        /* Don't forget the kbtype */
                        kbtype = pgdata->zuinData.kbtype;
			memset( &( pgdata->zuinData ), 0, sizeof( ZuinData ) );
                        pgdata->zuinData.kbtype = kbtype;
			break;
		}
	}
	return rtn;
}

int SpecialSymbolInput( int key, ChewingData *pgdata )
{
	static char keybuf[] = {
		'[', ']', '{', '}', '\'','<', ':', '\"', '>',
		'~', '!', '@', '#', '$', '%', '^', '&', '*',
		'(', ')', '_', '+', '=','\\', '|', '?',
		',', '.', ';'
	};

	static char *chibuf[] = {
		"「", "」", "『", "』", "、", "，", "：", "；", "。",
		"～", "！", "＠", "＃", "＄", "％", "︿", "＆", "＊",
		"（", "）", "﹍", "＋", "＝", "＼", "｜", "？",
		"，", "。", "；"
	};
	static int nSpecial = 29;
	return InternalSpecialSymbol( key, pgdata, nSpecial, keybuf, chibuf );
}

int SpecialEtenSymbolInput( int key, ChewingData *pgdata )
{
	static char keybuf[] = {
		17, 23, 5, 18, 20, 25, 21, 9, 15, 16,
		1, 19, 4, 6, 7, 8, 10, 11, 12, 59, 39,
		26, 24, 3, 22, 2, 14, 13, 44, 46, 47
	};

	static char *chibuf[] = {
		"┌","┬","┐","□","〈","〉", "…","、","。","※",
		"├","┼","┤","【","】","◇","○", "—", "│", "；","：",
		"└","┴","┘", "ˇ", "《", "》" ,"─", "，","．","？"
	};
	static int nSpecial = 31;
	return InternalSpecialSymbol( key, pgdata, nSpecial, keybuf, chibuf );
}

int SymbolChoice(ChewingData *pgdata, int sel_i){
        int kbtype;
	pgdata->chiSymbolCursor -- ;
	memmove( &(pgdata->chiSymbolBuf[pgdata->chiSymbolCursor]),
                 &(pgdata->chiSymbolBuf[pgdata->chiSymbolCursor] ) ,
                 sizeof(wch_t)*
                 (pgdata->chiSymbolBufLen - pgdata->chiSymbolCursor) ) ;
	pgdata->chiSymbolBuf[pgdata->chiSymbolCursor].wch = (wchar_t) 0 ;
	pgdata->chiSymbolBuf[pgdata->chiSymbolCursor].s[0] = pgdata->choiceInfo.totalChoiceStr[sel_i][0];
	pgdata->chiSymbolBuf[pgdata->chiSymbolCursor].s[1] = pgdata->choiceInfo.totalChoiceStr[sel_i][1];
	pgdata->chiSymbolCursor ++ ; 
	pgdata->bUserArrCnnct[pgdata->cursor] = 0;
	ChoiceEndChoice(pgdata);
        /* Don't forget the kbtype */
        kbtype = pgdata->zuinData.kbtype;
        memset( &( pgdata->zuinData ), 0, sizeof( ZuinData ) );
        pgdata->zuinData.kbtype = kbtype;
	return ZUIN_ABSORB;
}

int SemiSymbolInput(int key, ChewingData *pgdata)
{
    static char keybuf[] = {' ', '1'} ;
    static char *chibuf[] = {"　", "符"} ;
    static int nSpecial = 2 ;
    return InternalSpecialSymbol( key, pgdata, nSpecial, keybuf, chibuf );
}

int SymbolInput( int key, ChewingData *pgdata )
{
	if ( isprint( (char) key ) ) { /* other character was ignored */
		memmove(
			&( pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor + 1 ] ),
			&( pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ] ),
			sizeof( wch_t ) * ( pgdata->chiSymbolBufLen - pgdata->chiSymbolCursor ) );

		pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ].wch = (wchar_t) 0;
		pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ].s[ 0 ] = (char) key;
		pgdata->chiSymbolCursor++;
		pgdata->chiSymbolBufLen++;
		pgdata->bUserArrCnnct[ pgdata->cursor ] = 0;
		return SYMBOL_KEY_OK;
	}
	return SYMBOL_KEY_ERROR;
}

static int CompInterval( const IntervalType *a, const IntervalType *b )
{
	int cmp = a->from - b->from;
	if ( cmp )
		return cmp;
	return ( a->to - b->to );
}

int FindIntervalFrom( int from, IntervalType inte[], int nInte )
{
	int i;

	for ( i = 0; i < nInte; i++ )
		if ( inte[ i ].from == from )
			return i;
	return -1;
}

int WriteChiSymbolToBuf( wch_t csBuf[], int csBufLen, ChewingData *pgdata )
{
	int i, phoneseq_i = 0;

	for ( i = 0 ; i < csBufLen; i++ ) {
		if ( ChewingIsChiAt( i, pgdata ) ) {
			/*
			 * Workaround to avoid different initialization behavior 
			 * among Win32 and Unix-like OSs.
			 */
			memset( &( csBuf[ i ].s ), 0, WCH_SIZE );
			memcpy( csBuf[ i ].s, &( pgdata->phrOut.chiBuf[ phoneseq_i ] ), 2 );
			phoneseq_i += 2;
		}
		else 
			csBuf[ i ].wch = pgdata->chiSymbolBuf[ i ].wch;
	}
	return 0;
}

int CountReleaseNum( ChewingData *pgdata )
{
	int remain, i;

	/* reserve ZUIN_SIZE positions for Zuin */
	remain = pgdata->chiSymbolBufLen + ZUIN_SIZE - pgdata->config.maxChiSymbolLen;
	if ( remain <= 0 )
		return 0;

	qsort(
		pgdata->preferInterval, 
		pgdata->nPrefer, 
		sizeof( IntervalType ),
		(CompFuncType) CompInterval ); 

	if ( ! ChewingIsChiAt( 0, pgdata ) )
		return 1;
	
	i = FindIntervalFrom( 0, pgdata->preferInterval, pgdata->nPrefer );
	if ( i >= 0 ) {
		return ( pgdata->preferInterval[ i ].to - pgdata->preferInterval[ i ].from ); 
	}

	return 1;
}

void KillFromLeft( ChewingData *pgdata, int nKill )
{
	int i;

	for ( i = 0; i < nKill; i++ )
		ChewingKillChar( pgdata, 0, 0, DECREASE_CURSOR );
}

void CleanAllBuf( ChewingData *pgdata )
{
	/* 1 */
	pgdata->nPhoneSeq = 0 ;
	memset( pgdata->phoneSeq, 0, sizeof( pgdata->phoneSeq ) );
	/* 2 */
	pgdata->chiSymbolBufLen = 0;
	memset( pgdata->chiSymbolBuf, 0, sizeof( pgdata->chiSymbolBuf ) );
	/* 3 */
	memset( pgdata->bUserArrBrkpt, 0, sizeof( pgdata->bUserArrBrkpt ) );
	/* 4 */
	pgdata->nSelect = 0;
	/* 5 */
	pgdata->cursor = 0;
	/* 6 */
	pgdata->chiSymbolCursor = 0;
	/* 7 */
	memset( pgdata->bUserArrCnnct, 0, sizeof( pgdata->bUserArrCnnct ) );
}

int ReleaseChiSymbolBuf( ChewingData *pgdata, ChewingOutput *pgo )
{
	int throwEnd;
	uint16 bufPhoneSeq[ MAX_PHONE_SEQ_LEN + 1 ];
	char bufWordSeq[ MAX_PHONE_SEQ_LEN * 2 + 1 ];

	throwEnd = CountReleaseNum( pgdata );

	pgo->nCommitStr = throwEnd;
	if ( throwEnd ) {
		/*
		 * count how many chinese words in "chiSymbolBuf[ 0 .. (throwEnd - 1)]"
		 * And release from "chiSymbolBuf" && "phoneSeq"
		 */
		WriteChiSymbolToBuf( pgo->commitStr, throwEnd, pgdata );

		/* Add to userphrase */
		memcpy( bufPhoneSeq, pgdata->phoneSeq, sizeof( uint16 ) * throwEnd );
		bufPhoneSeq[ throwEnd ] = (uint16) 0;
		memcpy( bufWordSeq, pgdata->phrOut.chiBuf, sizeof( char ) * throwEnd * 2 );
		bufWordSeq[ throwEnd * 2 ] = '\0';
		UserUpdatePhrase( bufPhoneSeq, bufWordSeq );

		KillFromLeft( pgdata, throwEnd );
	}
	return throwEnd;
}

void AutoLearnPhrase( ChewingData *pgdata )
{
	uint16 bufPhoneSeq[ MAX_PHONE_SEQ_LEN + 1 ];
	char bufWordSeq[ MAX_PHONE_SEQ_LEN * 2 + 1 ];
	int i, from, len;

	for ( i = 0; i < pgdata->nPrefer; i++ ) {
		from = pgdata->preferInterval[ i ].from;
		len = pgdata->preferInterval[i].to - from;
		memcpy( bufPhoneSeq, &pgdata->phoneSeq[ from ], sizeof( uint16 ) * len );
		bufPhoneSeq[ len ] = (uint16) 0;
		memcpy( 
			bufWordSeq, 
			&pgdata->phrOut.chiBuf[ from * 2 ], 
			sizeof( char ) * len * 2 );
		bufWordSeq[ len * 2 ] = '\0';
		UserUpdatePhrase( bufPhoneSeq, bufWordSeq );
	}
}

int AddChi( uint16 phone, ChewingData *pgdata )
{
	int i;

	/* shift the selectInterval */
	for ( i = 0; i < pgdata->nSelect; i++ ) {
		if ( pgdata->selectInterval[ i ].from >= pgdata->cursor ) {
			pgdata->selectInterval[ i ].from++;
			pgdata->selectInterval[ i ].to++;
		}
	}

	/* shift the Brkpt */
	memmove( 
		&( pgdata->bUserArrBrkpt[ pgdata->cursor + 2 ] ),
		&( pgdata->bUserArrBrkpt[ pgdata->cursor + 1 ] ),
		sizeof( int ) * ( pgdata->nPhoneSeq - pgdata->cursor ) );
	memmove(
		&( pgdata->bUserArrCnnct[ pgdata->cursor + 2 ] ),
		&( pgdata->bUserArrCnnct[pgdata->cursor + 1 ] ),
		sizeof( int ) * ( pgdata->nPhoneSeq - pgdata->cursor ) );

	/* add to phoneSeq */
	memmove(
		&( pgdata->phoneSeq[ pgdata->cursor + 1 ] ),
		&( pgdata->phoneSeq[ pgdata->cursor ] ) ,
		sizeof( uint16 ) * ( pgdata->nPhoneSeq - pgdata->cursor ) );
	pgdata->phoneSeq[ pgdata->cursor ] = phone;
	pgdata->nPhoneSeq ++;
	pgdata->cursor ++;

	/* add to chiSymbolBuf */
	memmove(
		&( pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor + 1 ] ),
		&( pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ] ) ,
		sizeof( wch_t ) * ( pgdata->chiSymbolBufLen - pgdata->chiSymbolCursor ) );
	/* "0" means Chinese word */
	pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ].wch = (wchar_t) 0;
	pgdata->chiSymbolBufLen++;
	pgdata->chiSymbolCursor++;

	return 0;
}

#ifdef ENABLE_DEBUG
void ShowChewingData( ChewingData *pgdata )
{
	int i ;

	DEBUG_OUT(
		"nPhoneSeq : %d\n"
		"phoneSeq : ", 
		pgdata->nPhoneSeq );
	for ( i = 0; i < pgdata->nPhoneSeq; i++ )
		DEBUG_OUT( "%hu ", pgdata->phoneSeq[ i ] );
	DEBUG_OUT(
		"cursor : %d\n"
		"nSelect : %d\n"
		"selectStr       selectInterval\n", 
		pgdata->cursor, 
		pgdata->nSelect );
	for ( i = 0; i < pgdata->nSelect; i++ ) {
		DEBUG_OUT(
			"  %14s%4d%4d\n",
			pgdata->selectStr[ i ], 
			pgdata->selectInterval[ i ].from,
			pgdata->selectInterval[ i ].to );
	}
	
	DEBUG_OUT( "bUserArrCnnct : " );
	for ( i = 0; i <= pgdata->nPhoneSeq; i++ )
		DEBUG_OUT( "%d ", pgdata->bUserArrCnnct[ i ] );
	DEBUG_OUT( "\n" );

	DEBUG_OUT( "bUserArrBrkpt : " );
	for ( i = 0; i <= pgdata->nPhoneSeq; i++ )
		DEBUG_OUT( "%d ", pgdata->bUserArrBrkpt[ i ] );
	DEBUG_OUT( "\n" );

	DEBUG_OUT( "bArrBrkpt : " );
	for ( i = 0; i <= pgdata->nPhoneSeq; i++ )
		DEBUG_OUT( "%d ", pgdata->bArrBrkpt[ i ] );
	DEBUG_OUT( "\n" );

	DEBUG_OUT( 
		"bChiSym : %d , bSelect : %d , bCaseChange : %d\n",
		pgdata->bChiSym, 
		pgdata->bSelect, 
		pgdata->bCaseChange );
}

#endif

int CallPhrasing( ChewingData *pgdata )
{
	/* set "bSymbolArrBrkpt" && "bArrBrkpt" */
	int i, ch_count = 0;

	memcpy(
		pgdata->bArrBrkpt, 
		pgdata->bUserArrBrkpt, 
		(MAX_PHONE_SEQ_LEN + 1) * sizeof( int ) );
	memset(
		pgdata->bSymbolArrBrkpt, 0, 
		(MAX_PHONE_SEQ_LEN + 1) * sizeof( int ) );

	for ( i = 0; i < pgdata->chiSymbolBufLen; i++ ) {
		if ( ChewingIsChiAt( i, pgdata ) )
			ch_count++;
		else {
			pgdata->bArrBrkpt[ ch_count ] = 1;
			pgdata->bSymbolArrBrkpt[ ch_count ] = 1;
		}
	}

	/* kill select interval */
	for ( i = 0; i < pgdata->nPhoneSeq; i++ ) {
		if ( pgdata->bArrBrkpt[ i ] ) {
			ChewingKillSelectIntervalAcross( i, pgdata );
		}
	}

	/* then phrasing */
	Phrasing( 
		&( pgdata->phrOut ), pgdata->phoneSeq, pgdata->nPhoneSeq,
		pgdata->selectStr, pgdata->selectInterval, pgdata->nSelect, 
		pgdata->bArrBrkpt, pgdata->bUserArrCnnct );

	/* and then make prefer interval */
	MakePreferInterval( pgdata );

	return 0;
}


static void Union( int set1,int set2, int parent[] )
{
	if ( set1 != set2 )
		parent[ max( set1, set2 ) ] = min( set1, set2 );
}

static int SameSet( int set1,int set2, int parent[] )
{
	while ( parent[ set1 ] != 0 ) {
		set1 = parent[ set1 ];
	}
	while ( parent[ set2 ] != 0 ) {
		set2 = parent[ set2 ];
	}
	return ( set1 == set2 );
}

/* make prefer interval from phrOut->dispInterval */
static void MakePreferInterval( ChewingData *pgdata )
{
	int i, j, set_no;
	int belong_set[ MAX_PHONE_SEQ_LEN + 1 ];
	int parent[ MAX_PHONE_SEQ_LEN + 1 ];

	memset( belong_set, 0 , sizeof( int ) * ( MAX_PHONE_SEQ_LEN + 1 ) );
	memset( parent, 0, sizeof( int ) * ( MAX_PHONE_SEQ_LEN + 1 ) );

	/* for each interval */
	for ( i = 0; i < pgdata->phrOut.nDispInterval; i++ ) {
		for (
			j = pgdata->phrOut.dispInterval[ i ].from;
			j < pgdata->phrOut.dispInterval[ i ].to;
			j++ ) {
			belong_set[ j ] = i + 1;
		}
	}
	set_no = i;
	for ( i = 0; i < pgdata->nPhoneSeq; i++ )
		if ( belong_set[i] == 0 ) 
			belong_set[ i ] = set_no++;

	/* for each connect point */
	for ( i = 1; i < pgdata->nPhoneSeq; i++ ) {
		if ( pgdata->bUserArrCnnct[ i ] ) {
			Union( belong_set[ i - 1 ], belong_set[ i ], parent );
		}
	}

	/* generate new intervals */
	pgdata->nPrefer = 0;
	i = 0;
	while ( i < pgdata->nPhoneSeq ) {
		for ( j = i + 1; j < pgdata->nPhoneSeq; j++ )
			if ( ! SameSet( belong_set[ i ], belong_set[ j ], parent ) )
				break;

		pgdata->preferInterval[ pgdata->nPrefer ].from = i;
		pgdata->preferInterval[ pgdata->nPrefer ].to = j;
		pgdata->nPrefer++;
		i = j;
	}
}

/* for MakeOutput */
static void ShiftInterval( ChewingOutput *pgo, ChewingData *pgdata )
{
	int i, arrPos[ MAX_PHONE_SEQ_LEN ], k = 0, from, len;

	for ( i = 0; i < pgdata->chiSymbolBufLen; i++ ) {
		if ( ChewingIsChiAt( i, pgdata ) ) {
			arrPos[ k++ ] = i;
		}
	}
	arrPos[ k ] = i;

	pgo->nDispInterval = pgdata->nPrefer;
	for ( i = 0; i < pgdata->nPrefer; i++ ) {
		from = pgdata->preferInterval[ i ].from;
		len = pgdata->preferInterval[ i ].to - from;
		pgo->dispInterval[ i ].from = arrPos[ from ];
		pgo->dispInterval[ i ].to = arrPos[ from ] + len;
	}
}

int MakeOutput( ChewingOutput *pgo, ChewingData *pgdata )
{
	int chi_i, chiSymbol_i, i ;

	/* fill zero to chiSymbolBuf first */
	memset( pgo->chiSymbolBuf, 0, sizeof( wch_t ) * MAX_PHONE_SEQ_LEN );

	/* fill chiSymbolBuf */
	for ( 
		chi_i = chiSymbol_i = 0; 
		chiSymbol_i < pgdata->chiSymbolBufLen; 
		chiSymbol_i ++ ) {
		if ( pgdata->chiSymbolBuf[ chiSymbol_i ].wch == (wchar_t) 0 ) { 
			/* is Chinese, then copy from the PhrasingOutput "phrOut" */
			pgo->chiSymbolBuf[ chiSymbol_i ].wch = (wchar_t) 0;
			memcpy(
				pgo->chiSymbolBuf[ chiSymbol_i ].s,
				&( pgdata->phrOut.chiBuf[ chi_i ] ),
				2 );
			chi_i += 2;
		}
		else {
			/* is Symbol */
			pgo->chiSymbolBuf[ chiSymbol_i ].wch = 
				pgdata->chiSymbolBuf[ chiSymbol_i ].wch;
		}
	}

	/* fill point */
	pgo->PointStart = pgdata->PointStart;
	pgo->PointEnd = pgdata->PointEnd;

	/* fill other fields */
	pgo->chiSymbolBufLen = pgdata->chiSymbolBufLen;
	pgo->chiSymbolCursor = pgdata->chiSymbolCursor;
	
	/*  fill zuinBuf */
        if(pgdata->zuinData.kbtype >= KB_HANYU_PINYING) {
		char *p = pgdata->zuinData.pinYingData.keySeq;
		for ( i = 0; i< ZUIN_SIZE; i++) {
			int j;
			for(j = 0; j < 2; j++) {
				if(p[0]) {
					pgo->zuinBuf[i].s[j] = p[0];
					p++;
				} else {
					pgo->zuinBuf[i].s[j] = '\0';
				}
			}
			pgo->zuinBuf[i].s[2]='\0';
		}
	} else {
		for ( i = 0; i < ZUIN_SIZE; i++ ) { 
			if ( pgdata->zuinData.pho_inx[ i ] != 0 ) {
				memcpy( 
					pgo->zuinBuf[ i ].s, 
					& ph_pho[ i ][ pgdata->zuinData.pho_inx[ i ] * 2 ], 
					2 ); 
				pgo->zuinBuf[ i ].s[ 2 ] = '\0';
			}
			else
				pgo->zuinBuf[ i ].wch = (wchar_t) 0;
		}
        }

	ShiftInterval( pgo, pgdata );
	memcpy( 
		pgo->dispBrkpt, pgdata->bUserArrBrkpt, 
		sizeof( pgo->dispBrkpt[ 0 ] ) * ( MAX_PHONE_SEQ_LEN + 1 ) );
	pgo->pci = &( pgdata->choiceInfo );
	pgo->bChiSym = pgdata->bChiSym;
	memcpy( pgo->selKey, pgdata->config.selKey, sizeof( pgdata->config.selKey ) );
	pgo->bShowMsg = 0;
	return 0;
}

int MakeOutputWithRtn( ChewingOutput *pgo, ChewingData *pgdata, int keystrokeRtn )
{
	pgo->keystrokeRtn = keystrokeRtn;
	return MakeOutput( pgo, pgdata );
}

void MakeOutputAddMsgAndCleanInterval( ChewingOutput *pgo, ChewingData *pgdata )
{
	pgo->bShowMsg = 1;
	memcpy( pgo->showMsg, pgdata->showMsg, sizeof( wch_t ) * ( pgdata->showMsgLen ) );
	pgo->showMsgLen = pgdata->showMsgLen;
	pgo->nDispInterval = 0;
}

int AddSelect( ChewingData *pgdata, int sel_i )
{
	int length, nSelect;

	/* save the typing time */
	length = pgdata->availInfo.avail[ pgdata->availInfo.currentAvail ].len;
	nSelect = pgdata->nSelect;

	/* change "selectStr" , "selectInterval" , and "nSelect" of ChewingData */
	memcpy(
		pgdata->selectStr[ nSelect ],
		pgdata->choiceInfo.totalChoiceStr[ sel_i ],
		length * 2 ) ;
	pgdata->selectInterval[ nSelect ].from = pgdata->cursor;
	pgdata->selectInterval[ nSelect ].to = pgdata->cursor + length;
	pgdata->nSelect++;
	return 0;
}

int CountSelKeyNum( int key, ChewingData *pgdata )
	/* return value starts from 0.  If less than zero : error key */
{
	int i;

	for ( i = 0; i < MAX_SELKEY; i++ )
		if ( pgdata->config.selKey[ i ] == key )
			return i;
	return -1;
}

int ChewingIsChiAt( int chiSymbolCursor, ChewingData *pgdata )
{
	/* wch == 0 means Chinese */
	return (
		( chiSymbolCursor < pgdata->chiSymbolBufLen ) &&
		( 0 <= chiSymbolCursor ) &&
		(pgdata->chiSymbolBuf[ chiSymbolCursor ].wch == (wchar_t) 0 ) );
}

void RemoveSelectElement( int i, ChewingData *pgdata )
{
	if ( --pgdata->nSelect == i )
		return;
	pgdata->selectInterval[ i ] = pgdata->selectInterval[ pgdata->nSelect ];
	strcpy( pgdata->selectStr[ i ], pgdata->selectStr[ pgdata->nSelect ] );
}

int ChewingKillSelectIntervalAcross( int cursor, ChewingData *pgdata )
{
	int i;
	for ( i = 0; i < pgdata->nSelect; i++ ) {
		if ( pgdata->selectInterval[ i ].from < cursor && 
			pgdata->selectInterval[ i ].to > cursor ) {
			RemoveSelectElement( i, pgdata );
			i--;
		}
	}
	return 0;
}

int KillCharInSelectIntervalAndBrkpt( ChewingData *pgdata, int cursorToKill )
{
	int i; 

	for ( i = 0; i < pgdata->nSelect; i++ ) { 
		if ( pgdata->selectInterval[ i ].from <= cursorToKill && 
			pgdata->selectInterval[ i ].to > cursorToKill ) { 
			RemoveSelectElement( i, pgdata ); 
			i--;      /* the last one was swap to i, we need to recheck i */ 
		} 
		else if( pgdata->selectInterval[ i ].from > cursorToKill ) { 
			pgdata->selectInterval[ i ].from--; 
			pgdata->selectInterval[ i ].to--; 
		} 
	} 
	memmove( 
		&( pgdata->bUserArrBrkpt[ cursorToKill ] ),
		&( pgdata->bUserArrBrkpt[ cursorToKill + 1 ] ),
		sizeof( int ) * ( pgdata->nPhoneSeq - cursorToKill ) );
	memmove( 
		&( pgdata->bUserArrCnnct[ cursorToKill ] ),
		&( pgdata->bUserArrCnnct[ cursorToKill + 1 ] ),
		sizeof( int ) * ( pgdata->nPhoneSeq - cursorToKill ) );

	return 0;
}

int ChewingKillChar(
		ChewingData *pgdata, 
		int cursorToKill, 
		int chiSymbolCursorToKill, 
		int minus )
{
	if ( ChewingIsChiAt( chiSymbolCursorToKill, pgdata ) ) {
		KillCharInSelectIntervalAndBrkpt(pgdata, cursorToKill);
		memmove(
			&( pgdata->phoneSeq[ cursorToKill ] ), 
			&(pgdata->phoneSeq[ cursorToKill + 1 ] ),
			(pgdata->nPhoneSeq - cursorToKill - 1) * sizeof( uint16 ) );
		pgdata->nPhoneSeq--;
		pgdata->cursor -= minus;
	}
	memmove( 
		& pgdata->chiSymbolBuf[ chiSymbolCursorToKill ],
		& pgdata->chiSymbolBuf[ chiSymbolCursorToKill + 1 ], 
		(pgdata->chiSymbolBufLen - chiSymbolCursorToKill) * sizeof( wch_t ) );
	pgdata->chiSymbolBufLen--;
	pgdata->chiSymbolCursor -= minus;
	return 0;
}

int IsPreferIntervalConnted( int cursor, ChewingData *pgdata )
{
	int i;

	for ( i = 0; i < pgdata->nPrefer; i++ ) {
		if ( 
			pgdata->preferInterval[ i ].from < cursor &&
			pgdata->preferInterval[ i ].to > cursor ) 
			return 1;
	}
	return 0;
}

