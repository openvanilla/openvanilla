/**
 * hash.c
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

#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "hash.h"
#include "private.h"
#include "global.h"

int chewing_lifetime;

static HASH_ITEM *hashtable[ HASH_TABLE_SIZE ];
static char formatstring[ 30 ];
static char hashfilename[ 200 ];

int AlcUserPhraseSeq( UserPhraseData *pData, int len )
{
	pData->phoneSeq = ALC( uint16, len + 1 );
	pData->wordSeq = ALC( char, len * 2 + 1 );
	return ( pData->phoneSeq && pData->wordSeq );
}

static int PhoneSeqTheSame( const uint16 p1[], const uint16 p2[] )
{
	int i;

	for ( i = 0; ( p1[ i ] != 0 && p2[ i ] != 0 ); i++ ) {
		if ( p1[ i ] != p2[ i ] )
			return 0;
	}
	if ( p1[ i ] != p2[ i ] )
		return 0;
	return 1;
}

static unsigned int HashFunc( const uint16 phoneSeq[] )
{
	int i, value = 0;

	for ( i = 0; phoneSeq[ i ] != 0; i++ )
		value ^= phoneSeq[ i ];
	return ( value & ( HASH_TABLE_SIZE - 1 ) );
}

HASH_ITEM *HashFindPhonePhrase( const uint16 phoneSeq[], HASH_ITEM *pItemLast )
{
	HASH_ITEM *pNow = ( ! pItemLast ) ?
		hashtable[ HashFunc( phoneSeq ) ] :
		pItemLast->next;
	
	for ( ; pNow; pNow = pNow->next ) 
		if ( PhoneSeqTheSame( pNow->data.phoneSeq, phoneSeq ) )
			return pNow;
	return NULL;
}

HASH_ITEM *HashFindEntry( const uint16 phoneSeq[], const char wordSeq[] )
{
	HASH_ITEM *pItem;
	int hashvalue;

	hashvalue = HashFunc( phoneSeq );

	for ( pItem = hashtable[ hashvalue ]; pItem ; pItem = pItem->next ) {
		if ( 
			! strcmp( pItem->data.wordSeq, wordSeq ) && 
			PhoneSeqTheSame( pItem->data.phoneSeq, phoneSeq ) ) {
			return pItem;
		}
	}
	return NULL;
}

HASH_ITEM *HashInsert( UserPhraseData *pData )
{
	int hashvalue, len;
	HASH_ITEM *pItem;

	pItem = HashFindEntry( pData->phoneSeq, pData->wordSeq );
	if ( pItem != NULL )
		return pItem;

	pItem = ALC( HASH_ITEM, 1 );
	if ( ! pItem )
		return NULL;  /* Error occurs */
	len = strlen( pData->wordSeq ) / 2;
	if ( ! AlcUserPhraseSeq( &( pItem->data ), len ) )
		return NULL; /* Error occurs */

	hashvalue = HashFunc( pData->phoneSeq );
	/* set the new element */
	pItem->next = hashtable[ hashvalue ];

	memcpy( &( pItem->data ), pData, sizeof( pItem->data ) );
	pItem->item_index = -1;

	/* set link to the new element */
	hashtable[ hashvalue ] = pItem;

	return pItem;
}

static void HashItem2String( char *str, HASH_ITEM *pItem )
{
	int i, len;
	char buf[ FIELD_SIZE ];

	sprintf( str, "%s ", pItem->data.wordSeq );
	len = strlen( pItem->data.wordSeq ) / 2;
	for ( i = 0; i < len; i++ ) {
		sprintf( buf, "%hu ", pItem->data.phoneSeq[ i ] );
		strcat( str, buf );
	}
	sprintf( 
		buf, "%d %d %d %d",
		pItem->data.userfreq, pItem->data.recentTime, 
		pItem->data.maxfreq, pItem->data.origfreq );
	strcat( str, buf );
}

void HashModify( HASH_ITEM *pItem )
{
	FILE *outfile;
	char str[ FIELD_SIZE + 1 ];

	outfile = fopen( hashfilename, "r+" );

	/* update "lifetime" */
	fseek( outfile, 0, SEEK_SET );
	sprintf( str, "%d", chewing_lifetime );
	DEBUG_OUT( 
		"HashModify-1: formatstring='%s',printing '%s'\n", 
		formatstring,str );
	DEBUG_FLUSH;
	fprintf( outfile, formatstring, str );

	/* update record */
	if( pItem->item_index < 0 ) {
		fseek( outfile, 0, SEEK_END );
		pItem->item_index = ftell( outfile ) / FIELD_SIZE;
	}
	else {
		fseek( outfile, pItem->item_index * FIELD_SIZE, SEEK_SET );
	}
	HashItem2String( str, pItem );
	DEBUG_OUT( 
		"HashModify-2: formatstring='%s',printing '%s'\n",
		formatstring, str );
	DEBUG_FLUSH;
	fprintf( outfile, formatstring, str );
	fclose( outfile );
}

int ReadHashItem( FILE *infile, HASH_ITEM *pItem, int item_index )
{
	int len, i, word_len;
	char wordbuf[ 64 ];

	/* read wordSeq */
	if ( fscanf( infile, "%s", wordbuf ) != 1 )
		return 0;
	word_len = strlen( wordbuf );
	pItem->data.wordSeq = ALC( char, word_len + 1 );
	strcpy( pItem->data.wordSeq, wordbuf );

	/* read phoneSeq */
	len = word_len / 2;
	pItem->data.phoneSeq = ALC( uint16, len + 1 );
	for ( i = 0; i < len; i++ )
		if ( fscanf( infile, "%hu", &( pItem->data.phoneSeq[ i ] ) ) != 1 )
			return 0;
	pItem->data.phoneSeq[ len ] = 0;

	/* read userfreq & recentTime */
	if ( fscanf( infile, "%d %d %d %d", 
		&(pItem->data.userfreq), 
		&(pItem->data.recentTime),
		&(pItem->data.maxfreq), 
		&(pItem->data.origfreq) ) != 4 )
		return 0;

	/* set item_index */
	pItem->item_index = item_index;

	return 1;
}

int ReadHash( char *path )
{
	FILE *infile;
	HASH_ITEM item, *pItem;
	int item_index, hashvalue;

	/* make sure of write permission */
	if ( access( path, W_OK ) != 0) {
		if ( getenv( "HOME" ) ) {
			sprintf(
				hashfilename, "%s%s", 
				getenv( "HOME" ), CHEWING_HASH_PATH );
		}
		else {
			sprintf(
				hashfilename, "%s%s",
				"/tmp", CHEWING_HASH_PATH );
		}
		mkdir( hashfilename, S_IRWXU );
		strcat (hashfilename, "/");
		strcat (hashfilename, HASH_FILE);
	} else {
		sprintf( hashfilename, "%s/%s", path, HASH_FILE );
	}
	memset( hashtable, 0, HASH_TABLE_SIZE );
	sprintf( formatstring, "%%-%ds", FIELD_SIZE ); 
	infile = fopen( hashfilename, "r" );
	if ( ! infile ) {
		FILE *outfile;
		outfile = fopen( hashfilename, "w" );
		if ( ! outfile )
			return 0;

		fprintf( outfile, formatstring, "0" );
		chewing_lifetime = 0;
		fclose( outfile );
	}
	else {
		fscanf( infile, "%d", &chewing_lifetime );
		item_index = 0;
		while ( ReadHashItem( infile, &item, ++item_index ) ) {
			hashvalue = HashFunc( item.data.phoneSeq );
			pItem = ALC( HASH_ITEM, 1 );
			memcpy( pItem, &item, sizeof( HASH_ITEM ) );
			pItem->next = hashtable[ hashvalue ];
			hashtable[ hashvalue ] = pItem;
		}
		fclose( infile );
	}
	return 1;
}

