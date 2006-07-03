/**
 * userphrase.c
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

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "global.h"
#include "hash.h"
#include "dict.h"
#include "private.h"

extern int chewing_lifetime;
static HASH_ITEM *pItemLast;

int DeltaFreq( int recentTime )
{
	int diff;

	diff = ( chewing_lifetime - recentTime );

	if ( diff < 1000 )
		return ( 1500 - diff ); /* 1500 ~ 500 */
	if ( diff < 2000 )
		return ( 500 );       /* 500 ~ 500 */
	if ( diff < 3000 )
		return ( 2500 - diff ); /* 500 ~ -500 */
	return ( -500 );    /* -500 forever */
}

/* load the orginal frequency from the static dict */
int LoadOriginalFreq( const uint16 phoneSeq[], const char wordSeq[], int len )
{
	int pho_id;
	int retval;
	Phrase *phrase = ALC( Phrase, 1 );

	pho_id = TreeFindPhrase( 0, len - 1, phoneSeq );
	if ( pho_id != -1 ) {
		GetPhraseFirst( phrase, pho_id );
		do {
			/* find the same phrase */
			if ( ! memcmp( 
				phrase->phrase, 
				wordSeq, 
				len * 2 * sizeof( char ) ) ) { 
				retval = phrase->freq;	
				free( phrase );
				return retval;
			}
		} while ( GetPhraseNext( phrase ) );
	}

	return FREQ_INIT_VALUE;
}

/* find the maximum frequency of the same phrase */
int LoadMaxFreq( const uint16 phoneSeq[], int len )
{
	int pho_id;
	Phrase *phrase = ALC( Phrase, 1 );
	int maxFreq = FREQ_INIT_VALUE;
	UserPhraseData *uphrase;

	pho_id = TreeFindPhrase( 0, len - 1, phoneSeq );
	if ( pho_id != -1 ) {
		GetPhraseFirst( phrase, pho_id );
		do {
			if ( phrase->freq > maxFreq )
				maxFreq = phrase->freq;
		} while( GetPhraseNext( phrase ) );
	}
	free( phrase );

	uphrase = UserGetPhraseFirst( phoneSeq );
	while ( uphrase ) {
		if ( uphrase->userfreq > maxFreq )
			maxFreq = uphrase->userfreq;
		uphrase = UserGetPhraseNext( phoneSeq );
	}	  

	return maxFreq;
}

/* compute the new updated freqency */
int UpdateFreq( int freq, int maxfreq, int origfreq, int deltatime )
{
	int delta;

	/* Short interval */
	if ( deltatime < 4000 ) {
		delta = ( freq >= maxfreq ) ?
			min( 
				( maxfreq - origfreq ) / 5 + 1, 
				SHORT_INCREASE_FREQ ) :
			max( 
				( maxfreq - origfreq ) / 5 + 1, 
				SHORT_INCREASE_FREQ );
		return min( freq + delta, MAX_ALLOW_FREQ );
	}
	/* Medium interval */
	else if ( deltatime < 50000 ) {
		delta = ( freq >= maxfreq ) ?
			min( 
				( maxfreq - origfreq ) / 10 + 1, 
				MEDIUM_INCREASE_FREQ ) :
			max( 
				( maxfreq - origfreq ) / 10 + 1, 
				MEDIUM_INCREASE_FREQ );
		return min( freq + delta, MAX_ALLOW_FREQ );
	}
	/* long interval */
	else {
		delta = max( ( freq - origfreq ) / 5, LONG_DECREASE_FREQ );
		return max( freq - delta, origfreq );
	}
}

int UserUpdatePhrase( const uint16 phoneSeq[], const char wordSeq[] )
{
	HASH_ITEM *pItem;
	UserPhraseData data;
	int len;

	len = strlen( wordSeq ) / 2;
	pItem = HashFindEntry( phoneSeq, wordSeq );
	if ( ! pItem ) {
		if ( ! AlcUserPhraseSeq( &data, len ) ) {
			return USER_UPDATE_FAIL;
		}

		memcpy( data.phoneSeq, phoneSeq, len * sizeof( phoneSeq[ 0 ] ) );
		data.phoneSeq[ len ] = 0;
		strcpy( data.wordSeq, wordSeq );

		/* load initial freq */
		data.origfreq = LoadOriginalFreq( phoneSeq, wordSeq, len );
		data.maxfreq = LoadMaxFreq( phoneSeq, len );

		data.userfreq = data.origfreq;
		data.recentTime = chewing_lifetime;
		pItem = HashInsert( &data );
		HashModify( pItem );
		return USER_UPDATE_INSERT;
	}
	else {
		pItem->data.maxfreq = LoadMaxFreq( phoneSeq, len );
		pItem->data.userfreq = UpdateFreq( 
			pItem->data.userfreq, 
			pItem->data.maxfreq, 
			pItem->data.origfreq, 
			chewing_lifetime - pItem->data.recentTime );
		pItem->data.recentTime = chewing_lifetime;
		HashModify( pItem );
		return USER_UPDATE_MODIFY;
	}
}

UserPhraseData *UserGetPhraseFirst( const uint16 phoneSeq[] )
{
	pItemLast = HashFindPhonePhrase( phoneSeq, NULL );
	if ( ! pItemLast ) 
		return NULL;
	return &( pItemLast->data );
}

UserPhraseData *UserGetPhraseNext( const uint16 phoneSeq[] ) 
{
	pItemLast = HashFindPhonePhrase( phoneSeq, pItemLast );
	if ( ! pItemLast )
		return NULL;
	return &( pItemLast->data );
}

