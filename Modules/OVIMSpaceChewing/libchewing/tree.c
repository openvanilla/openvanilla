/**
 * tree.c
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
 *	@file tree.c
 *	@brief API for accessing the phrase tree.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userphrase.h"
#include "global.h"
#include "dict.h"
#include "char.h"
#include "private.h"

#define INTERVAL_SIZE ( ( MAX_PHONE_SEQ_LEN + 1 ) * MAX_PHONE_SEQ_LEN / 2 )

typedef struct tagRecordNode {
	int *arrIndex;		/* the index array of the things in "interval" */
	int nInter ,freq;
	struct tagRecordNode *next;
	int nMatchCnnct;	/* match how many Cnnct. */
} RecordNode;

typedef struct {
	int leftmost[ MAX_PHONE_SEQ_LEN + 1 ] ;
	char graph[ MAX_PHONE_SEQ_LEN + 1 ][ MAX_PHONE_SEQ_LEN + 1 ];
	PhraseIntervalType interval[ MAX_INTERVAL ];
	int nInterval;
	RecordNode *phList;  
} TreeDataType;

extern TreeType tree[ TREE_SIZE ];

int IsContain( IntervalType in1, IntervalType in2 )
{
	return ( in1.from <= in2.from && in1.to >= in2.to );
}

int IsIntersect( IntervalType in1, IntervalType in2 )
{
	return ( max( in1.from, in2.from ) < min( in1.to, in2.to ) );
}

int PhraseIntervalContain(PhraseIntervalType in1, PhraseIntervalType in2)
{
	return ( in1.from <= in2.from && in1.to >= in2.to );
}

int PhraseIntervalIntersect(PhraseIntervalType in1, PhraseIntervalType in2)
{
	return ( max( in1.from, in2.from ) < min( in1.to, in2.to ) );
}

/** @brief check for intersection of two intervals and return it */
int GetIntersection( IntervalType in1, IntervalType in2, IntervalType *in3 )
{
	in3->from = max( in1.from, in2.from );
	in3->to = min( in1.to, in2.to );
	if ( in3->from < in3->to )
		return 1;
	return 0;
}

void ReadTree( const char *prefix )
{
	int i;
	FILE *infile;
	char filename[ 100 ];

	sprintf( filename, "%s/%s", prefix, PHONE_TREE_FILE );
	infile = fopen( filename, "r" );
	assert( infile );
	for ( i = 0; i < TREE_SIZE; i++ ) {
		if ( fscanf( infile, "%hu%d%d%d",
			&tree[ i ].phone_id,
			&tree[ i ].phrase_id,
			&tree[ i ].child_begin,
			&tree[ i ].child_end ) != 4 )
			break;
	}
	fclose( infile );
}

int CheckBreakpoint( int from, int to, int bArrBrkpt[] )
{
	int i;
	for ( i = from + 1; i < to; i++ )
		if ( bArrBrkpt[ i ] )
			return 0;
	return 1;
}

int CheckUserChoose( 
		uint16 *new_phoneSeq, int from , int to,
		Phrase **pp_phr, 
		char selectStr[][ MAX_PHONE_SEQ_LEN * 2 + 1 ], 
		IntervalType selectInterval[], int nSelect )
{
	IntervalType inte, c;
	int chno, len;
	int user_alloc;
	UserPhraseData *pUserPhraseData;
	Phrase *p_phr = ALC( Phrase, 1 );

	assert( p_phr );
	inte.from = from;
	inte.to = to;
	*pp_phr = NULL;

	/* pass 1
	 * if these exist one selected interval which is not contained by inte
	 * but has intersection with inte, then inte is an unacceptable interval
	 */
	for ( chno = 0; chno < nSelect; chno++ ) {
		c = selectInterval[ chno ];
		if ( IsIntersect( inte, c ) && ! IsContain( inte, c ) ) {
			free( p_phr );
			return 0;
		}
	}

	/* pass 2
	 * if there exist one phrase satisfied all selectStr then return 1, else return 0.
	 * also store the phrase with highest freq
	 */
	pUserPhraseData = UserGetPhraseFirst( new_phoneSeq );
	p_phr->freq = -1;
	do {
		for ( chno = 0; chno < nSelect; chno++ ) {
			c = selectInterval[ chno ];

			if ( IsContain( inte, c ) ) {
				/* 
				 * find a phrase of ph_id where the text contains 
				 * 'selectStr[chno]' test if not ok then return 0, 
				 * if ok then continue to test. */
				len = c.to - c.from;
				if ( memcmp( 
					&pUserPhraseData->wordSeq[ ( c.from - from ) * 2 ], 
					selectStr[ chno ], 
					len * 2 ) )
					break;
			}

		}
		if ( chno == nSelect ) {
			/* save phrase data to "pp_phr" */
			if ( pUserPhraseData->userfreq > p_phr->freq ) {
				if ( ( user_alloc = ( to - from ) ) > 0 ) {
					memcpy( 
						p_phr->phrase, 
						pUserPhraseData->wordSeq, 
						user_alloc *  2 * sizeof( char ) );
				}
				p_phr->phrase[ user_alloc * 2 ] = '\0';
				p_phr->freq = pUserPhraseData->userfreq;
				*pp_phr = p_phr;
			}
		}
	} while ( ( pUserPhraseData = UserGetPhraseNext( new_phoneSeq ) ) != NULL );

	if ( p_phr->freq != -1 ) 
		return 1;
		
	free( p_phr );
	return 0;
}

/*
 * phrase is said to satisfy a choose interval if 
 * their intersections are the same */
int CheckChoose(
		int ph_id, int from, int to, Phrase **pp_phr, 
		char selectStr[][ MAX_PHONE_SEQ_LEN * 2 + 1 ], 
		IntervalType selectInterval[], int nSelect )
{
	IntervalType inte, c;
	int chno, len;
	Phrase *phrase = ALC( Phrase, 1 );

	assert( phrase );
	inte.from = from;
	inte.to = to;
	*pp_phr = NULL;

	/* if there exist one phrase satisfied all selectStr then return 1, else return 0. */
	GetPhraseFirst( phrase, ph_id );
	do {
		for ( chno = 0; chno < nSelect; chno++ ) {
			c = selectInterval[ chno ];

			if ( IsContain( inte, c ) ) {
				/* find a phrase of ph_id where the text contains 
				 * 'selectStr[chno]' test if not ok then return 0, if ok 
				 * then continue to test
				 */
				len = c.to - c.from;
				if ( memcmp( 
					&( phrase->phrase[ ( c.from - from ) *  2 ] ), 
					selectStr[ chno ], len * 2 ) )
					break;
			}
			else if ( IsIntersect( inte, selectInterval[ chno ] ) ) {
				free( phrase );
				return 0;
			} 
		}
		if ( chno == nSelect ) {
			*pp_phr = phrase;
			return 1;
		}
	} while ( GetPhraseNext( phrase ) );
	free( phrase );
	return 0;
}

/** @brief search for the phrases have the same pronunciation.*/
/* if phoneSeq[a] ~ phoneSeq[b] is a phrase, then add an interval
 * from (a) to (b+1) */
int TreeFindPhrase( int begin, int end, const uint16 *phoneSeq )
{
	int child, tree_p, i;

	tree_p = 0;
	for ( i = begin; i <= end; i++ ) {
		for ( 
			child = tree[ tree_p ].child_begin;
			child <= tree[ tree_p ].child_end;
			child++ ) {
			if ( tree[ child ].phone_id == phoneSeq[ i ] )
				break;
		}
		/* if not found any word then fail. */
		if ( child > tree[ tree_p ].child_end )
			return -1;
		else
			tree_p = child;
	}
	return tree[ tree_p ].phrase_id;
}

void AddInterval(
		TreeDataType *ptd, int begin , int end, 
		int p_id, Phrase *p_phrase, int dict_or_user )
{
	ptd->interval[ ptd->nInterval ].from = begin;
	ptd->interval[ ptd->nInterval ].to = end + 1;
	ptd->interval[ ptd->nInterval ].pho_id = p_id;
	ptd->interval[ ptd->nInterval ].p_phr = p_phrase;
	ptd->interval[ ptd->nInterval ].source = dict_or_user;
	ptd->nInterval++;
}

void FindInterval(
		uint16 *phoneSeq, int nPhoneSeq, 
		char selectStr[][ MAX_PHONE_SEQ_LEN * 2 + 1 ], 
		IntervalType selectInterval[], int nSelect, 
		int bArrBrkpt[], TreeDataType *ptd )
{
	int end, begin, pho_id;
	Phrase *p_phrase, *puserphrase, *pdictphrase;
	uint16 new_phoneSeq[ MAX_PHONE_SEQ_LEN ];

	for ( begin = 0; begin < nPhoneSeq; begin++ ) {
		for ( end = begin; end < nPhoneSeq; end++ ) {
			if ( ! CheckBreakpoint( begin, end + 1, bArrBrkpt ) )
				continue;

			/* set new_phoneSeq */
			memcpy( 
				new_phoneSeq, 
				&phoneSeq[ begin ], 
				sizeof( uint16 ) * ( end - begin + 1 ) );
			new_phoneSeq[ end - begin + 1 ] = 0;
			puserphrase = pdictphrase = NULL;

			/* check user phrase */
			if ( UserGetPhraseFirst( new_phoneSeq ) &&
					CheckUserChoose( new_phoneSeq, begin, end + 1, 
					&p_phrase, selectStr, selectInterval, nSelect ) ) {
				puserphrase = p_phrase;
			}

			/* check dict phrase */
			pho_id = TreeFindPhrase( begin, end, phoneSeq );
			if ( 
				( pho_id != -1 ) && 
				CheckChoose( 
					pho_id, begin, end + 1, 
					&p_phrase, selectStr, 
					selectInterval, nSelect ) ) {
				pdictphrase = p_phrase;
			}

			/* add only one interval, which has the largest freqency
			 * but when the phrase is the same, the user phrase overrides 
			 * static dict
			 */
			if ( puserphrase != NULL && pdictphrase == NULL ) 
				AddInterval( 
					ptd, 
					begin, 
					end, 
					-1, 
					puserphrase, 
					IS_USER_PHRASE );
			else if ( puserphrase == NULL && pdictphrase != NULL )
				AddInterval( 
					ptd, 
					begin, 
					end, 
					pho_id, 
					pdictphrase, 
					IS_DICT_PHRASE );
			else if( puserphrase != NULL && pdictphrase != NULL ) {
				/* the same phrase, userphrase overrides */
				if ( ! memcmp( 
					puserphrase->phrase, 
					pdictphrase, 
					( end - begin + 1 ) * 2 * sizeof( char ) ) ) 
					AddInterval( 
						ptd, 
						begin, 
						end, 
						-1, 
						puserphrase, 
						IS_USER_PHRASE );
				else {
					if ( puserphrase->freq > pdictphrase->freq ) 
						AddInterval( 
							ptd, 
							begin, 
							end, 
							-1, 
							puserphrase, 
							IS_USER_PHRASE );
					else
						AddInterval( 
							ptd, 
							begin, 
							end, 
							pho_id, 
							pdictphrase, 
							IS_DICT_PHRASE );
				}
			}
		}
	}
}

void SetInfo( int len, TreeDataType *ptd )
{
	int i, a;

	for ( i = 0; i <= len; i++ )
		ptd->leftmost[ i ] = i;
	for ( i = 0; i < ptd->nInterval; i++ ) {
		ptd->graph[ ptd->interval[ i ].from ][ ptd->interval[ i ].to ] = 1;
		ptd->graph[ ptd->interval[ i ].to ][ ptd->interval[ i ].from ] = 1;
	}

	/* set leftmost */
	for ( a = 0; a <= len; a++ ) {
		for ( i = 0; i <= len; i++ ) {
			if ( ! ( ptd->graph[ a ][ i ] ) )
				continue;
			if ( ptd->leftmost[ i ] < ptd->leftmost[ a ] )
				ptd->leftmost[ a ] = ptd->leftmost[ i ];
		}
	}
}

int CompLen( IntervalType *pa, IntervalType *pb )
{
	return ( ( pa->to - pa->from ) - ( pb->to - pb->from ) );
}

int CompLenDescend( IntervalType *pa, IntervalType *pb )
{
	return ( ( pb->to - pb->from ) - ( pa->to - pa->from ) );
}

int CompFrom( IntervalType *pa, IntervalType *pb )
{
	int cmp = pa->from - pb->from;
	if ( cmp )
		return cmp;
	return ( pa->to - pb->to );
}

/* 
 * First we compare the 'nMatchCnnct'.
 * If the values are the same, we will compare the 'freq'
 */
int CompRecord( const RecordNode **pa, const RecordNode **pb )
{
	int diff = (*pb)->nMatchCnnct - (*pa)->nMatchCnnct;

	if ( diff )
		return diff;
	return ( (*pb)->freq - (*pa)->freq );
}


void Discard1( TreeDataType *ptd )
{
	int a, b;
	char failflag[ INTERVAL_SIZE ];
	int nInterval2;

	memset( failflag, 0, sizeof( failflag ) );
	for ( a = 0; a < ptd->nInterval; a++ ) {
		if ( failflag[ a ] ) 
			continue;
		for ( b = 0; b < ptd->nInterval; b++ ) {
			if ( a == b || failflag[ b ] )
				continue ;
			if ( ptd->interval[ b ].from >= ptd->interval[ a ].from && 
				ptd->interval[ b ].to <= ptd->interval[ a ].to )
				continue;
			if ( ptd->interval[ b ].from <= ptd->interval[ a ].from && 
				ptd->interval[ b ].to <= ptd->interval[ a ].from )
				continue;
			if ( ptd->interval[ b ].from >= ptd->interval[ a ].to && 
				ptd->interval[ b ].to >= ptd->interval[ a ].to )
				continue;
			break;
		}
		/* if any other interval b is inside or leftside or rightside the 
		 * interval a */
		if ( b >= ptd->nInterval ) {
			/* then kill all the intervals inside the interval a */
			int i;
			for ( i = 0; i < ptd->nInterval; i++ )  {
				if ( 
					! failflag[ i ] && i != a &&
					ptd->interval[ i ].from >= 
						ptd->interval[ a ].from && 
					ptd->interval[ i ].to <= ptd->interval[ a ].to ) {
					failflag[ i ] = 1;
				}
			}
		}
	}
	/* discard all the intervals whose failflag[a] = 1 */
	nInterval2 = 0;
	for ( a = 0; a < ptd->nInterval; a++ )
		if ( ! failflag[ a ] )
			ptd->interval[ nInterval2++ ] = ptd->interval[ a ];
	ptd->nInterval = nInterval2;
}

void Discard2( TreeDataType *ptd )
{
	int i, j;
	char overwrite[ MAX_PHONE_SEQ_LEN ], failflag[ MAX_PHONE_SEQ_LEN ];
	int nInterval2;

	memset( failflag, 0, sizeof( failflag ) );
	for ( i = 0; i < ptd->nInterval; i++ ) {
		if ( ptd->leftmost[ ptd->interval[ i ].from ] == 0 )
			continue;
		/* test if interval i is overwrited by other intervals */
		memset( overwrite, 0, sizeof( overwrite ) );
		for ( j = 0; j < ptd->nInterval; j++ ) {
			if ( j == i )
				continue;
			memset( 
				&overwrite[ ptd->interval[ j ].from ],
				1, 
				ptd->interval[ j ].to - ptd->interval[ j ].from );
		}
		if ( memchr( 
			&overwrite[ ptd->interval[ i ].from ],
			1,	
			ptd->interval[ i ].to - ptd->interval[ i ].from ) )
			failflag[ i ] = 1;
	}
	/* discard all the intervals whose failflag[a] = 1 */
	nInterval2 = 0;
	for ( i = 0; i < ptd->nInterval; i++ )
		if ( ! failflag[ i ] )
			ptd->interval[ nInterval2++ ] = ptd->interval[ i ];
	ptd->nInterval = nInterval2;
}

void LoadChar( char *buf, uint16 phoneSeq[], int nPhoneSeq )
{
	int i;
	Word word;

	for ( i = 0; i < nPhoneSeq; i++ ) {
		GetCharFirst( &word, phoneSeq[ i ] );
		memcpy( buf + i * 2, word.word, 2 );
	}
	buf[ nPhoneSeq * 2 ] = '\0';
}

/* kpchen said, record is the index array of interval */
void OutputRecordStr(
		char *out_buf, int *record, int nRecord, 
		uint16 phoneSeq[], int nPhoneSeq, 
		char selectStr[][ MAX_PHONE_SEQ_LEN * 2 + 1 ], 
		IntervalType selectInterval[],
		int nSelect, TreeDataType *ptd )
{
	PhraseIntervalType inter;
	int i;

	LoadChar( out_buf, phoneSeq, nPhoneSeq );
	out_buf[ nPhoneSeq * 2 ] = '\0' ;
	for ( i = 0; i < nRecord; i++ ) {
		inter = ptd->interval[ record[ i ] ];
		memcpy( 
			out_buf + inter.from * 2, 
			( inter.p_phr )->phrase, 
			( inter.to - inter.from ) * 2 );
	}
	for ( i = 0; i < nSelect; i++ ) {
		inter.from = selectInterval[ i ].from;
		inter.to = selectInterval[ i ].to ;
		memcpy( 
			out_buf+inter.from * 2, 
			selectStr[ i ], 
			( inter.to - inter.from ) * 2);
	}
}

int LoadPhraseAndCountFreq( int *record,int nRecord, TreeDataType *ptd )
{
	int i, total_freq = 0;
	PhraseIntervalType inter;

	for ( i = 0; i < nRecord; i++ ) {
		inter = ptd->interval[ record[ i ] ];
		assert( inter.p_phr );
		
		/* We adjust the 'freq' of One-word Phrase */
		total_freq += ( inter.to - inter.from == 1 ) ?
			( inter.p_phr->freq / 512 ) :
			inter.p_phr->freq;
	}
	return total_freq;
}

int IsRecContain( int *intA, int nA, int *intB, int nB, TreeDataType *ptd )
{
	int big, sml;

	for ( big = 0, sml = 0; sml < nB; sml++ ) {
		while ( 
			( big < nA ) && 
			ptd->interval[ intA[ big ] ].from < 
				ptd->interval[ intB[ sml ] ].to ) {
			if ( PhraseIntervalContain(
				ptd->interval[ intA[ big ] ], 
				ptd->interval[ intB[ sml ] ] ) )
				break;
			big++;
		}
		if ( 
			( big >= nA ) || 
			ptd->interval[ intA[ big ] ].from >= 
				ptd->interval[ intB[ sml ] ].to )
			return 0;
	}
	return 1;
}

void SortListByFreq( TreeDataType *ptd )
{
	int i, listLen;
	RecordNode *p, **arr;

	for ( 
		listLen = 0, p = ptd->phList; 
		p; 
		listLen++, p = p->next )
		;

	arr = ALC( RecordNode *, listLen );
	assert( arr );

	for ( 
		i = 0, p=ptd->phList;
		i < listLen;
		p = p->next, i++ ) {
		arr[ i ] = p;
		p->freq = LoadPhraseAndCountFreq( 
			p->arrIndex, 
			p->nInter, 
			ptd );
	}

	qsort( arr, listLen, sizeof( RecordNode * ), (CompFuncType) CompRecord );

	ptd->phList = arr[ 0 ];
	for ( i = 1; i < listLen; i++ ) {
		arr[ i - 1 ]->next = arr[ i ];
	}
	arr[ listLen - 1 ]->next = NULL;

	free( arr );
}

/* when record==NULL then output the "link list" */
void SaveRecord( int *record, int nInter, TreeDataType *ptd )
{
	RecordNode *now, *p, *pre;

	pre = NULL;
	for ( p = ptd->phList; p; ) {
		/* if  'p' contains 'record', then discard 'record'. */
		if ( IsRecContain( p->arrIndex, p->nInter, record, nInter, ptd ) ) 
			return;

		/* if 'record' contains 'p', then discard 'p' 
		 * -- We must deal with the linked list. */
		if ( IsRecContain( record, nInter, p->arrIndex, p->nInter, ptd ) ) {
			RecordNode *tp = p;

			if ( pre ) 
				pre->next = p->next;
			else
				ptd->phList = ptd->phList->next;
			p = p->next;
			free( tp->arrIndex );			
			free( tp );
		}
		else 
			pre = p, p = p->next;
	}
	now = ALC( RecordNode, 1 );
	assert( now );
	now->next = ptd->phList;
	now->arrIndex = ALC( int, nInter );
	assert( now->arrIndex );
	now->nInter = nInter;
	memcpy( now->arrIndex, record, nInter * sizeof( int ) );	
	ptd->phList = now;
}

void RecursiveSave( int depth, int to, int *record, TreeDataType *ptd )
{
	int first, i;
	/* to find first interval */
	for ( 
		first = record[ depth - 1 ] + 1; 
		ptd->interval[ first ].from < to && first < ptd->nInterval; 
		first++ )
		;

	if ( first == ptd->nInterval ) {
		SaveRecord( record + 1, depth - 1, ptd );
		return;
	}
	record[ depth ] = first;
	RecursiveSave( depth + 1, ptd->interval[ first ].to, record, ptd );
	/* for each interval which intersects first */
	for ( 
		i= first + 1; 
		PhraseIntervalIntersect(
			ptd->interval[ first ], 
			ptd->interval[ i ] ) && i < ptd->nInterval;
			i++ ) {
		record[ depth ] = i;
		RecursiveSave( depth + 1, ptd->interval[ i ].to,record, ptd );
	}
}

void SaveList( TreeDataType *ptd )
{
	int record[ MAX_PHONE_SEQ_LEN + 1 ] = { -1 };

	RecursiveSave( 1, 0, record, ptd );	
}

void InitPhrasing( TreeDataType *ptd ) 
{
	memset( ptd, 0, sizeof( TreeDataType ) );
}

void SaveDispInterval( PhrasingOutput *ppo, TreeDataType *ptd )
{
	int i;

	for ( i = 0; i < ptd->phList->nInter; i++ ) {
		ppo->dispInterval[ i ].from = 
			ptd->interval[ ptd->phList->arrIndex[ i ] ].from;
		ppo->dispInterval[ i ].to = 
			ptd->interval[ ptd->phList->arrIndex[ i ] ].to;
	}
	ppo->nDispInterval = ptd->phList->nInter;
}

void CleanUpMem( TreeDataType *ptd )
{
	int i;

	for ( i = 0; i < ptd->nInterval; i++ ) {
		if ( ptd->interval[ i ].p_phr ) {
			free( ptd->interval[ i ].p_phr );
			ptd->interval[ i ].p_phr = NULL;
		}
	}
}

void CountMatchCnnct( TreeDataType *ptd, int *bUserArrCnnct, int nPhoneSeq )
{
	RecordNode *p;
	int i, k, sum;

	for ( p = ptd->phList; p; p = p->next ) {
		/* for each record, count its 'nMatchCnnct' */
		for ( sum = 0, i = 1; i < nPhoneSeq; i++ ) {
			if ( !bUserArrCnnct[ i ] ) 
				continue;
			/* check if matching 'cnnct' */
			for ( k = 0; k < p->nInter; k++ ) {
				if ( 
					ptd->interval[ p->arrIndex[ k ] ].from < i &&
					ptd->interval[ p->arrIndex[ k ] ].to > i ) {
					sum++;
					break;
				}
			}
		}
		p->nMatchCnnct = sum;
	}
}

#ifdef ENABLE_DEBUG
void ShowList( TreeDataType *ptd )
{
	RecordNode *p;
	int i;

	DEBUG_OUT( "After SaveList :\n" );
	for ( p = ptd->phList; p; p = p->next ) {
		DEBUG_OUT( "  interval : " );
		for ( i = 0; i < p->nInter; i++ ) {
			DEBUG_OUT(
				"[%d %d] ", 
				ptd->interval[ p->arrIndex[ i ] ].from,
				ptd->interval[ p->arrIndex[ i ] ].to );
		}
		DEBUG_OUT(
			"\n   freq : %d , nMatchCnnct : %d\n", 
			p->freq, 
			p->nMatchCnnct );
	}
	DEBUG_OUT( "\n" );
}

#endif

int Phrasing(
		PhrasingOutput *ppo, uint16 phoneSeq[], int nPhoneSeq, 
		char selectStr[][ MAX_PHONE_SEQ_LEN * 2 + 1 ], 
		IntervalType selectInterval[], int nSelect, 
		int bArrBrkpt[], int bUserArrCnnct[] ) 
{
	TreeDataType treeData;

	InitPhrasing( &treeData );

	FindInterval( 
		phoneSeq, nPhoneSeq, selectStr, selectInterval, nSelect, 
		bArrBrkpt, &treeData );
	SetInfo( nPhoneSeq, &treeData );
	Discard1( &treeData );
	Discard2( &treeData );
	SaveList( &treeData );
	CountMatchCnnct( &treeData, bUserArrCnnct, nPhoneSeq );
	SortListByFreq( &treeData );

#ifdef ENABLE_DEBUG
	ShowList( &treeData );
	DEBUG_FLUSH;
#endif

	/* set phrasing output */
	OutputRecordStr(
		ppo->chiBuf, 
		( treeData.phList )->arrIndex, 
		( treeData.phList )->nInter, 
		phoneSeq, 
		nPhoneSeq, 
		selectStr, selectInterval, nSelect, &treeData );
	SaveDispInterval( ppo, &treeData );

	/* free "phrase" */
	CleanUpMem( &treeData );
	return 0;
}

