/**
 * chewingutil.c
 *
 * Copyright (c) 1999, 2000, 2001
 *	Lu-chuan Kung and Kang-pen Chen.
 *	All rights reserved.
 *
 * Copyright (c) 2004, 2005, 2006
 *	libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "chewing-utf8-util.h"
#include "global.h"
#include "chewingutil.h"
#include "zuin.h"
#include "userphrase.h"
#include "private.h"

extern const char *zhuin_tab[]; 
static void MakePreferInterval( ChewingData *pgdata );
static void ShiftInterval( ChewingOutput *pgo, ChewingData *pgdata );

void SetUpdatePhraseMsg( ChewingData *pgdata, char *addWordSeq, int len, int state )
{
	char *insert = "加入－", *modify = "已有－";
	int begin = 3, i;

	pgdata->showMsgLen = begin + len;
	if ( state == USER_UPDATE_INSERT ) {
		ueStrNCpy( (char *) pgdata->showMsg[ 0 ].s, insert, 1, 1);
		ueStrNCpy( (char *) pgdata->showMsg[ 1 ].s,
		           ueStrSeek( insert, 1 ), 
		           1, 1 );
		ueStrNCpy( (char *) pgdata->showMsg[ 2 ].s,
		           ueStrSeek( insert, 2 ), 
		           1, 1 );
	}
	else {
		ueStrNCpy( (char *) pgdata->showMsg[ 0 ].s, modify, 1, 1);
		ueStrNCpy( (char *) pgdata->showMsg[ 1 ].s,
		           ueStrSeek( modify, 1 ),
			   1, 1 );
		ueStrNCpy( (char *) pgdata->showMsg[ 2 ].s,
		           ueStrSeek( modify, 2 ),
			   1, 1 );
	}
	for ( i = 0; i < len; i++ ) {
		ueStrNCpy( (char *) pgdata->showMsg[ begin + i ].s,
		           ueStrSeek(addWordSeq, i),
			   1, 1);
	}
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

int HaninSymbolInput(
		ChoiceInfo *pci, 
		AvailInfo *pai, 
		const uint16 phoneSeq[],
		int selectAreaLen)
{
	static char *chibuf[] = { 
		"，","、","。","．","；","：",
		"？","！","︰","?","…","‥","｜","—", "︴","﹏",
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
		"α","β","γ","δ","ε","ζ","η","θ",
		"ι","κ","λ","μ","ν","ξ","ο","π",
		"ρ","σ","τ","υ","φ","χ","ψ","ω"};
	int i, all = 216, len;

	len = pai->avail[ pai->currentAvail ].len;
	pci->nTotalChoice = 0;
	for ( i = 0; i< all; i++ ) {
		ueStrNCpy( pci->totalChoiceStr[ pci->nTotalChoice ],
				chibuf[i], 1, 1);
		pci->nTotalChoice++; 
	}  
	pai->avail[ 0 ].len = 1;
	pai->avail[ 0 ].id = -1;  
	pai->nAvail = 1;
	pai->currentAvail = 0;
	//pci->nChoicePerPage = (selectAreaLen - 5) / ( 2 + 3) ;
	pci->nChoicePerPage = selectAreaLen / (len + 1);
	if ( pci->nChoicePerPage > MAX_SELKEY ) {
		pci->nChoicePerPage = MAX_SELKEY;
	}
	pci->nPage = CEIL_DIV( pci->nTotalChoice, pci->nChoicePerPage );
	pci->pageNo = 0;
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
			ueStrNCpy( (char *) pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ].s,
			           chibuf[ i ],
				   1, 1);
			/* Save Symbol Key */
			memmove( &( pgdata->symbolKeyBuf[ pgdata->chiSymbolCursor + 1 ] ),
				&( pgdata->symbolKeyBuf[ pgdata->chiSymbolCursor ] ),
				sizeof( pgdata->symbolKeyBuf[0] ) * 
				( pgdata->chiSymbolBufLen - pgdata->chiSymbolCursor ) );
			pgdata->symbolKeyBuf[ pgdata->chiSymbolCursor ] = key;

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

int FullShapeSymbolInput( int key, ChewingData *pgdata )
{
	int rtn;
	static char keybuf[] = {
		'0','1','2','3','4','5','6','7','8','9',
		'a','b','c','d','e','f','g','h','i','j',
		'k','l','m','n','o','p','q','r','s','t',
		'u','v','w','x','y','z','A','B','C','D',
		'E','F','G','H','I','J','K','L','M','N',
		'O','P','Q','R','S','T','U','V','W','X',
		'Y','Z',' ','\"','\'','/','<','>','`','[',
		']','{','}','+','-'
	};
	static char* chibuf[] = {
		"０","１","２","３","４","５","６","７","８","９",
		"ａ","ｂ","ｃ","ｄ","ｅ","ｆ","ｇ","ｈ","ｉ","ｊ",
		"ｋ","ｌ","ｍ","ｎ","ｏ","ｐ","ｑ","ｒ","ｓ","ｔ",
		"ｕ","ｖ","ｗ","ｘ","ｙ","ｚ","Ａ","Ｂ","Ｃ","Ｄ",
		"Ｅ","Ｆ","Ｇ","Ｈ","Ｉ","Ｊ","Ｋ","Ｌ","Ｍ","Ｎ",
		"Ｏ","Ｐ","Ｑ","Ｒ","Ｓ","Ｔ","Ｕ","Ｖ","Ｗ","Ｘ",
		"Ｙ","Ｚ","　","”","’","／","＜","＞","‵","〔",
		"〕","｛","｝","＋","－"
	};
	static int nSpecial = sizeof(keybuf) / sizeof(char);
	rtn = InternalSpecialSymbol( key, pgdata, nSpecial, keybuf, chibuf );
	if ( rtn == ZUIN_IGNORE )
		rtn = SpecialSymbolInput( key, pgdata );
	return (rtn == ZUIN_IGNORE ? SYMBOL_KEY_ERROR : SYMBOL_KEY_OK);
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

int SymbolChoice( ChewingData *pgdata, int sel_i )
{
        int kbtype;
	if ( pgdata->choiceInfo.isSymbol == 1 )
		pgdata->chiSymbolCursor --;
	memmove( &(pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ] ),
                 &(pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ] ) ,
                 sizeof(wch_t) *
                 (pgdata->chiSymbolBufLen - pgdata->chiSymbolCursor) );
	pgdata->chiSymbolBuf[pgdata->chiSymbolCursor].wch = (wchar_t) 0;
	ueStrNCpy( (char *) pgdata->chiSymbolBuf[pgdata->chiSymbolCursor].s,
	           pgdata->choiceInfo.totalChoiceStr[sel_i],
		   1, 1);
	pgdata->chiSymbolCursor ++ ; 
	pgdata->bUserArrCnnct[pgdata->cursor] = 0;
	ChoiceEndChoice(pgdata);
        /* Don't forget the kbtype */
        kbtype = pgdata->zuinData.kbtype;
        memset( &( pgdata->zuinData ), 0, sizeof( ZuinData ) );
        pgdata->zuinData.kbtype = kbtype;
	return ZUIN_ABSORB;
}

int SemiSymbolInput(ChewingData *pgdata)
{
    static char keybuf[] = {'1'} ;
    static char *chibuf[] = {"符"} ;
    static int nSpecial = 1 ;
    return InternalSpecialSymbol( '1', pgdata, nSpecial, keybuf, chibuf );
}

int SymbolInput( int key, ChewingData *pgdata )
{
	if ( isprint( (char) key ) && /* other character was ignored */
	     (pgdata->chiSymbolBufLen < MAX_PHONE_SEQ_LEN) ) { /* protect the buffer */
		memmove(
			&( pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor + 1 ] ),
			&( pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ] ),
			sizeof( wch_t ) * ( pgdata->chiSymbolBufLen - pgdata->chiSymbolCursor ) );

		pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ].wch = (wchar_t) 0;
		pgdata->chiSymbolBuf[ pgdata->chiSymbolCursor ].s[ 0 ] = (char) key;

		/* Save Symbol Key */
		memmove( &( pgdata->symbolKeyBuf[ pgdata->chiSymbolCursor + 1 ] ),
			&( pgdata->symbolKeyBuf[ pgdata->chiSymbolCursor ] ),
			sizeof( pgdata->symbolKeyBuf[0] ) * 
			( pgdata->chiSymbolBufLen - pgdata->chiSymbolCursor ) );
			pgdata->symbolKeyBuf[ pgdata->chiSymbolCursor ] = toupper( key );
	
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

static int FindIntervalFrom( int from, IntervalType inte[], int nInte )
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
			memset( &( csBuf[ i ].s ), 0, MAX_UTF8_SIZE + 1 );
			ueStrNCpy( (char *) csBuf[ i ].s,
			           &( pgdata->phrOut.chiBuf[ phoneseq_i ] ), 
				   1, 1);
			phoneseq_i += ueBytesFromChar( pgdata->phrOut.chiBuf[ phoneseq_i ] );
		}
		else 
			csBuf[ i ] = pgdata->chiSymbolBuf[ i ];
	}
	return 0;
}

static int CountReleaseNum( ChewingData *pgdata )
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

static void KillFromLeft( ChewingData *pgdata, int nKill )
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

	pgdata->phrOut.nNumCut = 0;

	memset( pgdata->symbolKeyBuf, 0, sizeof( pgdata->symbolKeyBuf ) );
}

int ReleaseChiSymbolBuf( ChewingData *pgdata, ChewingOutput *pgo )
{
	int throwEnd;
	uint16 bufPhoneSeq[ MAX_PHONE_SEQ_LEN + 1 ];
	char bufWordSeq[ MAX_PHONE_SEQ_LEN * 3 + 1 ];

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
		ueStrNCpy( bufWordSeq, pgdata->phrOut.chiBuf, throwEnd, 1 );
		UserUpdatePhrase( bufPhoneSeq, bufWordSeq );

		KillFromLeft( pgdata, throwEnd );
	}
	return throwEnd;
}

void AutoLearnPhrase( ChewingData *pgdata )
{
	uint16 bufPhoneSeq[ MAX_PHONE_SEQ_LEN + 1 ];
	char bufWordSeq[ MAX_PHONE_SEQ_LEN * 3 + 1 ];
	int i, from, len;

	for ( i = 0; i < pgdata->nPrefer; i++ ) {
		from = pgdata->preferInterval[ i ].from;
		len = pgdata->preferInterval[i].to - from;
		memcpy( bufPhoneSeq, &pgdata->phoneSeq[ from ], sizeof( uint16 ) * len );
		bufPhoneSeq[ len ] = (uint16) 0;
		ueStrNCpy( bufWordSeq,
		           ueStrSeek( (char *) &pgdata->phrOut.chiBuf, from ),
		           len, 1);
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
static void ShowChewingData( ChewingData *pgdata )
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

static int MakeOutput( ChewingOutput *pgo, ChewingData *pgdata )
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
			ueStrNCpy( (char *) pgo->chiSymbolBuf[ chiSymbol_i ].s,
			           &( pgdata->phrOut.chiBuf[ chi_i ] ),
			           1, 1 );
			chi_i += ueBytesFromChar( pgo->chiSymbolBuf[ chiSymbol_i ].s[0] );
		}
		else {
			/* is Symbol */
			pgo->chiSymbolBuf[ chiSymbol_i ] = pgdata->chiSymbolBuf[ chiSymbol_i ];
		}
	}

	/* fill point */
	pgo->PointStart = pgdata->PointStart;
	pgo->PointEnd = pgdata->PointEnd;

	/* fill other fields */
	pgo->chiSymbolBufLen = pgdata->chiSymbolBufLen;
	pgo->chiSymbolCursor = pgdata->chiSymbolCursor;
	
	/*  fill zuinBuf */
        if ( pgdata->zuinData.kbtype >= KB_HANYU_PINYIN ) {
		char *p = pgdata->zuinData.pinYinData.keySeq;
		/* 
		 * Copy from old content in zuinBuf
		 * NOTE: No Unicode transformation here.
		 */
		for ( i = 0; i< ZUIN_SIZE; i++) {
			int j;
			for ( j = 0; j < 2; j++ ) {
				if ( p[0] ) {
					pgo->zuinBuf[ i ].s[ j ] = p[ 0 ];
					p++;
				} 
				else {
					pgo->zuinBuf[ i ].s[ j ] = '\0';
				}
			}
			pgo->zuinBuf[ i ].s[ 2 ] = '\0';
		}
	} else {
		for ( i = 0; i < ZUIN_SIZE; i++ ) { 
			if ( pgdata->zuinData.pho_inx[ i ] != 0 ) {
				/* Here we should use (zhuin_tab[i] + 2) to
				 * skip the 2 space characters at 
				 * zhuin_tab[0] and zhuin_tab[1]. */
				ueStrNCpy( (char *) pgo->zuinBuf[ i ].s,
				           ueStrSeek( (char *) (zhuin_tab[ i ] + 2),
						      pgdata->zuinData.pho_inx[ i ] - 1 ),
				           1, 1);
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
	ueStrNCpy( pgdata->selectStr[ nSelect ],
			pgdata->choiceInfo.totalChoiceStr[ sel_i ],
			length, 1 );
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

static int ChewingKillSelectIntervalAcross( int cursor, ChewingData *pgdata )
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

static int KillCharInSelectIntervalAndBrkpt( ChewingData *pgdata, int cursorToKill )
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
	pgdata->symbolKeyBuf[ chiSymbolCursorToKill ] = 0;
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

int OpenSymbolChoice( ChewingData *pgdata )
{
	static char *symbol_buf[][50] = {
		{ "0","ø", 0 },
		{ "[", "「", "『", "《", "〈", "【", "〔", 0 },
		{ "]", "」", "』", "》", "〉", "】", "〕", 0 },
		{ "{", "｛", 0 },
		{ "}", "｝", 0 },
		{ "<", "，", "＜", "←", 0 },
		{ ">", "。", "＞", "→", "．", "…", 0 },
		{ "?", "？","¿", 0 },
		{ "!", "！", "１", "①", "➀", "Ⅰ","¡", 0 },
		{ "@", "＠", "２", "②", "➁", "Ⅱ", "⊕", "⊙", "㊣", "﹫", 0 },
		{ "#", "＃", "３", "③", "➂", "Ⅲ", "﹟", 0 },
		{ "$", "＄", "４", "④", "➃", "Ⅳ", "€", "﹩", "￠", "∮","￡", "￥", 0 },
		{ "%", "％", "５", "⑤", "➄", "Ⅴ", 0 },
		{ "^", "︿", "６", "⑥", "➅", "Ⅵ", "﹀", "︽", "︾", 0 },
		{ "&", "＆", "７", "⑦", "➆", "Ⅶ", "﹠", 0 },
		{ "*", "＊", "８", "⑧", "➇", "Ⅷ", "×", "※", "╳", "﹡", "☯","☆", "★", 0 },
		{ "(", "（", "９", "⑨", "➈", "Ⅸ", 0 },
		{ ")", "）", "０", "⑩", "➉", "Ⅹ", 0 },
		{ "_", "＿", "…", "‥", "←", "→", "﹍", "﹉", "ˍ", "￣", "–", "—", "¯", "﹊", "﹎", "﹏", "﹣", "－", 0 },
		{ "+", "＋", "±", "﹢", "✙", "✚", "✛", "✜", "✝", "✞", "✟", 0 },
		{ "=", "＝", "≒", "≠", "≡", "≦", "≧", "﹦", 0},
		{ "`", "』", "『", "′", "‵", 0 },
		{ "~", "～", 0 },
		{ ":", "：", "；", "︰", "﹕", 0 },
		{ "\"", "；", 0 },
		{ "\'", "、", "…", "‥", 0 },
		{ "\\", "＼", "↖", "↘", "﹨", 0 },
		{ "-", "－", "＿", "￣", "¯", "ˍ", "–", "—", "‥", "…",
			"←","→","╴","﹉","﹊","﹍","﹎","﹏","﹣", 0 },
		{ "/","／","÷","↗","↙","∕", 0 },
		{ "|","↑", "↓", "∣", "∥", "︱", "︳", "︴" ,0 },
		{ "A", "Ⓐ", "Å","Α", "α", "├", "╠", "╟", "╞", 0 },
		{ "B", "Ⓑ", "Β", "β","∵", 0 },
		{ "C", "Ⓒ", "Χ", "χ", "┘", "╯", "╝", "╜", "╛","㏄","℃","㎝","♣","♧","©" ,0 },
		{ "D", "Ⓓ", "Δ", "δ", "◇", "◆", "┤", "╣", "╢", "╡","♦", 0 },
		{ "E", "Ⓔ", "Ε", "ε", "┐", "╮", "╗", "╓", "╕", 0 },
		{ "F", "Ⓕ", "Φ", "ψ", "│", "║", "℉","♀", 0 },
		{ "G", "Ⓖ", "Γ", "γ", 0 },
		{ "H", "Ⓗ", "Η", "η","♥","♡", 0 },
		{ "I", "Ⓘ", "Ι", "ι", 0 },
		{ "J", "Ⓙ", "φ", 0 },
		{ "K", "Ⓚ", "Κ", "κ","㎞", "㏎", 0 },
		{ "L", "Ⓛ", "Λ", "λ","㏒", "㏑", 0 },
		{ "M", "Ⓜ", "Μ", "μ","♂","ℓ","㎎", "㏕", "㎜","㎡", 0 },
		{ "N", "Ⓝ", "Ν", "ν","№", 0 },
		{ "O", "Ⓞ", "Ο", "ο", 0 },
		{ "P", "Ⓟ", "Π", "π", 0 },
		{ "Q", "Ⓠ", "Θ", "θ","Д","┌", "╭", "╔", "╓", "╒","۞", 0 },
		{ "R", "Ⓡ", "Ρ", "ρ", "─", "═" ,"®" , 0 },
		{ "S", "Ⓢ", "Σ", "σ","∴","□","■","┼", "╬", "╪", "╫","∫","§","♠","♤", 0 },
		{ "T", "Ⓣ", "Τ", "τ","θ","△","▲","▽","▼","™","⊿", "™", 0 },
		{ "U", "Ⓤ", "Υ", "υ","μ","∪", "∩", 0 },
		{ "V", "Ⓥ", 0 },
		{ "W", "Ⓦ", "Ω", "ω", "┬", "╦", "╤", "╥", 0 },
		{ "X", "Ⓧ", "Ξ", "ξ", "┴", "╩", "╧", "╨", 0 },
		{ "Y", "Ⓨ", "Ψ", 0 },
		{ "Z", "Ⓩ", "Ζ", "ζ", "└", "╰", "╚", "╙", "╘", 0 },
	};
	int i, symbol_buf_len = 56;
	char **pBuf;
	ChoiceInfo *pci = &( pgdata->choiceInfo );
	pci->oldCursor = pgdata->cursor;
	pci->oldChiSymbolCursor = pgdata->chiSymbolCursor;

	/* see if there is some word in the cursor position */
	if ( pgdata->chiSymbolCursor == pgdata->chiSymbolBufLen )
		pgdata->chiSymbolCursor--;
	if ( pgdata->symbolKeyBuf[ pgdata->chiSymbolCursor ] == '1' ) {
		pgdata->bSelect = 1;
		HaninSymbolInput( pci, &( pgdata->availInfo ), 
				pgdata->phoneSeq, pgdata->config.selectAreaLen );
		pci->isSymbol = 2;
		return 0;
	}
	for ( i = 0; i < symbol_buf_len; i++ ) {
		if ( symbol_buf[i][0][0] == pgdata->symbolKeyBuf[ pgdata->chiSymbolCursor ] ) {
			pBuf = symbol_buf[i];
			break;
		}
	}
	if ( i == symbol_buf_len ) {
		ChoiceEndChoice( pgdata );
		return 0;
	}
	pci->nTotalChoice = 0;
	for ( i = 1; pBuf[i]; i++ ) {
		ueStrNCpy( pci->totalChoiceStr[pci->nTotalChoice], 
				pBuf[i], ueStrLen( pBuf[i] ), 1 );
		pci->nTotalChoice++; 
	}

	pci->nChoicePerPage = 8 ;
	if ( pci->nChoicePerPage > MAX_SELKEY )
		pci->nChoicePerPage = MAX_SELKEY ;
	pci->nPage = CEIL_DIV( pci->nTotalChoice, pci->nChoicePerPage );
	pci->pageNo = 0;
	pci->isSymbol = 2;

	pgdata->bSelect = 1;
	pgdata->availInfo.nAvail = 1;
	pgdata->availInfo.currentAvail = 0;
	pgdata->availInfo.avail[ 0 ].id = -1;
	pgdata->availInfo.avail[ 0 ].len = 1; 
	return 0;
}

