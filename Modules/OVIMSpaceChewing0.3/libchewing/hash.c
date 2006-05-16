/**
 * hash.c
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

#include <string.h>
#include <sys/stat.h>

#include "chewing-utf8-util.h"
#include "hash.h"
#include "private.h"
#include "global.h"

int chewing_lifetime;

static HASH_ITEM *hashtable[ HASH_TABLE_SIZE ];
static char formatstring[ 30 ];
static char hashfilename[ 200 ];

int AlcUserPhraseSeq( UserPhraseData *pData, int phonelen, int wordlen )
{
	pData->phoneSeq = ALC( uint16, phonelen + 1 );
	pData->wordSeq = ALC( char, wordlen + 1 );
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
	len = ueStrLen( pData->wordSeq );
	if ( ! AlcUserPhraseSeq( &( pItem->data ), len, strlen( pData->wordSeq ) ) )
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
	len = ueStrLen( pItem->data.wordSeq );
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

/* 
 * capacity of 'str' MUST bigger then FIELD_SIZE !
 */
static void HashItem2Binary( char *str, HASH_ITEM *pItem )
{
	int i, phraselen;
	uint16 *pshort;
	unsigned char buf[ FIELD_SIZE ], *puc;

	memset(str, 0, FIELD_SIZE);
	if ( sizeof(int) * 4 + ueStrLen( pItem->data.wordSeq ) * 2 +
	     strlen( pItem->data.wordSeq ) >= FIELD_SIZE ) {
		/* exceed buffer size */
		return;
	}

	/* freq info */
	*(int*) &str[ 0 ] = pItem->data.userfreq;
	*(int*) &str[ 4 ] = pItem->data.recentTime;
	*(int*) &str[ 8 ] = pItem->data.maxfreq;
	*(int*) &str[ 12 ] = pItem->data.origfreq;

	/* phone seq*/
	phraselen = ueStrLen( pItem->data.wordSeq );
	str[ 16 ] = phraselen;
	pshort = (uint16 *) &str[ 17 ];
	for ( i = 0; i < phraselen; i++ ) {
		*pshort = pItem->data.phoneSeq[ i ];
		pshort++;
	}

	/* phrase */
	puc = (unsigned char *) pshort;
	*puc = strlen( pItem->data.wordSeq );
	strcpy( (char *) (puc + 1), pItem->data.wordSeq );
	pItem->data.wordSeq[ (int) *puc ] = '\0';
}

void HashModify( HASH_ITEM *pItem )
{
	FILE *outfile;
	char str[ FIELD_SIZE + 1 ];

	outfile = fopen( hashfilename, "r+b" );

	/* update "lifetime" */
	fseek( outfile, strlen( BIN_HASH_SIG ), SEEK_SET );
	fwrite( &chewing_lifetime, 1, 4, outfile );
#ifdef ENABLE_DEBUG
	sprintf( str, "%d", chewing_lifetime );
	DEBUG_OUT( 
		"HashModify-1: formatstring='%s',printing '%s'\n", 
		formatstring, str );
	DEBUG_FLUSH;
#endif

	/* update record */
	if ( pItem->item_index < 0 ) {
		fseek( outfile, 0, SEEK_END );
		pItem->item_index =
			( ftell( outfile ) - 4 - strlen( BIN_HASH_SIG ) ) / FIELD_SIZE;
	}
	else {
		fseek( outfile,
		       pItem->item_index * FIELD_SIZE + 4 + strlen( BIN_HASH_SIG ),
		       SEEK_SET );
	}
#ifdef ENABLE_DEBUG
	HashItem2String( str, pItem );
	DEBUG_OUT( 
		"HashModify-2: formatstring='%s',printing '%s'\n",
		formatstring, str );
	DEBUG_FLUSH;
#endif
	HashItem2Binary( str, pItem );
	fwrite( str, 1, FIELD_SIZE, outfile );
	fflush( outfile );
	fclose( outfile );
}

static int isValidChineseString( char *str )
{
	if ( strlen( str ) == 0 ) {
		return  0;
	}
	while ( *str != '\0' )  {
		int len = ueBytesFromChar( (unsigned char) *str );
		if ( len <= 1 ) {
			return 0;
		}
		str += len;
	};
	return 1;
}

/**
 * @return 1, 0 or -1
 * retval 0	end of file
 * retval 1	continue
 * retval -1	ignore this record
 */
int ReadHashItem_bin( const char *srcbuf, HASH_ITEM *pItem, int item_index )
{
       int len, i, word_len, ptr;
       uint16 *pshort;
       char wordbuf[ 64 ];
       unsigned char recbuf[ FIELD_SIZE ], *puc;

       memcpy( recbuf, srcbuf, FIELD_SIZE );
       memset( pItem, 0, sizeof(HASH_ITEM) );
       
       /* freq info */
       pItem->data.userfreq = *(int *) &recbuf[ 0 ];
       pItem->data.recentTime = *(int *) &recbuf[ 4 ];
       pItem->data.maxfreq = *(int *) &recbuf[ 8 ];
       pItem->data.origfreq = *(int *) &recbuf[ 12 ];

       /* phone seq, length in num of chi words */
       len = (int) recbuf[ 16 ];
       pItem->data.phoneSeq = ALC( uint16, len + 1 );
       pshort = (uint16 *) &recbuf[ 17 ];
       for ( i = 0; i < len; i++ ) {
               pItem->data.phoneSeq[ i ] = *pshort;
               ++pshort;
       }
       pItem->data.phoneSeq[ i ] = 0;
       
       /* phrase, length in num of bytes */
       puc = (unsigned char *) pshort;
       pItem->data.wordSeq = ALC( char, (*puc) + 1 );
       strcpy( pItem->data.wordSeq, (char *) (puc + 1) );
       pItem->data.wordSeq[ (int) *puc ] = '\0';

       /* Invalid UTF-8 Chinese characters found */
       if ( ! isValidChineseString( pItem->data.wordSeq ) ) {
               goto ignore_corrupted_record;
       }

       /* set item_index */
       pItem->item_index = item_index;

       return 1; /* continue */

ignore_corrupted_record:
       if ( pItem->data.phoneSeq != NULL ) {
               free( pItem->data.phoneSeq );
               pItem->data.phoneSeq = NULL;
       }
       if ( pItem->data.wordSeq != NULL ) {
               free( pItem->data.wordSeq );
               pItem->data.wordSeq = NULL;
       }
       return -1; /* ignore */
}

/**
 * @return 1, 0 or -1
 * retval -1 Ignore bad data item
 */
int ReadHashItem_txt( FILE *infile, HASH_ITEM *pItem, int item_index )
{
	int len, i, word_len;
	char wordbuf[ 64 ];

	/* read wordSeq */
	if ( fscanf( infile, "%s", wordbuf ) != 1 )
		return 0;

	/* Invalid UTF-8 Chinese characters found */
	if ( ! isValidChineseString( wordbuf ) ) {
		fseek( infile, FIELD_SIZE - strlen( wordbuf ) - 1, SEEK_CUR );
		return -1;
	}

	word_len = strlen( wordbuf );
	pItem->data.wordSeq = ALC( char, word_len + 1 );
	strcpy( pItem->data.wordSeq, wordbuf );

	/* read phoneSeq */
	len = ueStrLen( pItem->data.wordSeq );
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

static FILE *open_file_get_length(
		const char *filename, 
		const char *otype, int *size)
{
	FILE *tf = fopen( filename, otype );
	if ( tf == NULL ) {
		return NULL;
	}
	if ( size != NULL ) {
		fseek( tf, 0, SEEK_END );
		*size = ftell( tf );
		fseek( tf, 0, SEEK_SET );
	}
	return  tf;
}

static char *_load_hash_file( const char *filename, int *size )
{
	int flen;
	char *pd = NULL;
	FILE *tf;

	tf = open_file_get_length( filename, "rb", &flen );
	if ( tf == NULL ) {
		goto err_load_file;
	}
	pd = (char *) malloc( flen );
	if ( pd == NULL ) {
		goto err_load_file;
	}
	if ( fread( pd, flen, 1, tf ) != 1 ) {
		goto err_load_file;
	}
	fclose( tf );
	if ( size != NULL )
		*size = flen;
	return pd;

err_load_file:
	if ( pd != NULL )
		free( pd );
	if ( tf != NULL )
		fclose( tf );
	return NULL;
}

static int migrate_hash_to_bin( const char *ofilename )
{
	FILE *txtfile;
	char oldname[ 256 ], *dump, *seekdump;
	HASH_ITEM item, *pItem;
	int item_index, hashvalue, iret, tflen;

	/* allocate dump buffer */
	txtfile = open_file_get_length( ofilename, "r", &tflen );
	if ( txtfile == NULL ) {
		return 0;
	}
	dump = (char *) malloc( tflen * 2 );
	if ( dump == NULL ) {
		fclose( txtfile );
		return 0;
	}
	fscanf( txtfile, "%d", &chewing_lifetime );

	/* prepare the bin file */
	seekdump = dump;
	memcpy( seekdump, BIN_HASH_SIG, strlen( BIN_HASH_SIG ) );
	memcpy( seekdump + strlen( BIN_HASH_SIG ),
	        &chewing_lifetime, 
		sizeof(chewing_lifetime) );
	seekdump += strlen( BIN_HASH_SIG ) + sizeof(chewing_lifetime);

	/* migrate */
	item_index = 0;
	while ( 1 ) {
		iret = ReadHashItem_txt( txtfile, &item, ++item_index );

		if ( iret == -1 ) {
			--item_index;
			continue;
		}
		else if ( iret==0 )
			break;

		HashItem2Binary( seekdump, &item );
		seekdump += FIELD_SIZE;
		free( item.data.phoneSeq );
		free( item.data.wordSeq );
	};
	fclose( txtfile );

	/* backup as *.old */
	strncpy( oldname, ofilename, sizeof(oldname) );
	strncat( oldname, ".old", sizeof(oldname) );
	oldname[ sizeof(oldname) - 1 ] = '\0';
	PLAT_UNLINK( oldname );
	PLAT_RENAME( ofilename, oldname );

	/* dump new file */
	PLAT_UNLINK( ofilename );
	txtfile = fopen( ofilename, "w+b" );
	fwrite( dump, seekdump - dump, 1, txtfile );
	fflush( txtfile );
	fclose( txtfile );

	return  1;
}

/**
 * Attempt to re-compute lifetime
 */
static int ComputeChewingLifeTime()
{
       HASH_ITEM *item;
       int i, min;
       
       i = 0;

       chewing_lifetime++;
       min = chewing_lifetime;

       while ( hashtable[ i ] ) {
               item = hashtable[ i ];
               while ( item ) {
                       if ( item->data.recentTime < min )
                               min = item->data.recentTime;
                       item = item->next;
               }
               i++;
       }

       chewing_lifetime -= min;
       i = 0;

       while ( hashtable[ i ] ) {
               item = hashtable[ i ];
               while ( item ) {
                       item->data.recentTime -= min;
                       HashModify( item );
                       item = item->next;
               }
               i++;
       }
       return 0;
}

int ReadHash( const char *path )
{
	HASH_ITEM item, *pItem;
	int item_index, hashvalue, iret, fsize, hdrlen;
	char *dump, *seekdump;

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
				PLAT_TMPDIR, CHEWING_HASH_PATH );
		}
		PLAT_MKDIR( hashfilename );
		strcat( hashfilename, PLAT_SEPARATOR );
		strcat( hashfilename, HASH_FILE );
	} else {
		sprintf( hashfilename, "%s" PLAT_SEPARATOR "%s", path, HASH_FILE );
	}
	memset( hashtable, 0, HASH_TABLE_SIZE );
	sprintf( formatstring, "%%-%ds", FIELD_SIZE );

open_hash_file:
	dump = _load_hash_file( hashfilename, &fsize );
	hdrlen = strlen( BIN_HASH_SIG ) + sizeof(chewing_lifetime);
	item_index = 0;
	if ( dump == NULL || fsize < hdrlen ) {
		FILE *outfile;
		outfile = fopen( hashfilename, "w+b" );
		if ( ! outfile ) {
			if ( dump ) {
				free( dump );
			}
			return 0;
		}
		chewing_lifetime = 0;
		fwrite( BIN_HASH_SIG, 1, strlen( BIN_HASH_SIG ), outfile );
		fwrite( &chewing_lifetime, 1, sizeof(chewing_lifetime), outfile );
		fclose( outfile );
	}
	else {
		char header[ 5 ];
		if ( memcmp(dump, BIN_HASH_SIG, strlen(BIN_HASH_SIG)) != 0 ) {
			/* perform migrate from text-based to binary form */
			free( dump );
			if ( ! migrate_hash_to_bin( hashfilename ) ) {
				return  0;
			}
			goto open_hash_file;
		}

		chewing_lifetime = *(int *) (dump + strlen( BIN_HASH_SIG ));
		seekdump = dump + hdrlen;
		fsize -= hdrlen;

		while ( fsize >= FIELD_SIZE ) {
			iret = ReadHashItem_bin( seekdump, &item, item_index++ );
			/* Ignore illegal data */
			if ( iret == -1 ) {
				seekdump += FIELD_SIZE;
				fsize -= FIELD_SIZE;
				--item_index;
				continue;
			}
			else if ( iret == 0 )
				break;

			hashvalue = HashFunc( item.data.phoneSeq );
			pItem = ALC( HASH_ITEM, 1 );
			memcpy( pItem, &item, sizeof( HASH_ITEM ) );
			pItem->next = hashtable[ hashvalue ];
			hashtable[ hashvalue ] = pItem;
			seekdump += FIELD_SIZE;
			fsize -= FIELD_SIZE;
		}
		free( dump );
		ComputeChewingLifeTime();
	}
	return 1;
}

