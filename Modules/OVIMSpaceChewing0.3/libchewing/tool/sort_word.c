/**
 * sort_word.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "zuin.h"

#define PHONE_CIN_FILE	"phone.cin"

#define CHARDEF_BEGIN	"%chardef  begin"
#define CHARDEF_END	"%chardef  end"
#define DO_WORD_ERROR (1)
#define MAX_WORD	(60000)
#define MAX_NUMBER	(12000)
#define MAX_BUF_LEN	(4096)

typedef struct {
	uint16 num[2];
	char word[ 8 ];
} WORD_DATA;

WORD_DATA word_data[ MAX_WORD ];
int nWord;
int phone_num;

int SortWord( const WORD_DATA *a, const WORD_DATA *b )
{
	return ( a->num[0] - b->num[0] );
}

int DoWord( char *buf )
{
	char keyBuf[ 128 ], phoneBuf[ 128 ];
	int phoneInx[ ZUIN_SIZE ];

	memset( phoneInx, 0, sizeof( phoneInx ) );
	sscanf( buf, "%s %s", keyBuf, word_data[ nWord ].word );
	if ( strlen( keyBuf ) > ZUIN_SIZE )
		return DO_WORD_ERROR;

	PhoneFromKey( phoneBuf, keyBuf, KB_DEFAULT, 1 );
	word_data[ nWord ].num[0] = UintFromPhone( phoneBuf );
	nWord++ ;
	return 0;
}

void Output()
{
	FILE *indexfile, *datafile, *configfile;
	int i;
	uint16 previous;

	indexfile = fopen( CHAR_INDEX_FILE, "w" );
	datafile = fopen( CHAR_FILE, "w" );
	configfile = fopen( CHEWING_DEFINITION_FILE, "aw" );
	if ( ! indexfile || ! datafile || ! configfile ) {
		fprintf( stderr, "File Write Error\n" );
		exit( 1 );
	}

	previous = 0 ;
	phone_num = 0;
	for ( i = 0; i < nWord; i++ ) {
		if ( word_data[ i ].num[0] != previous ) {
			previous = word_data[ i ].num[0];
			fprintf( indexfile, "%hu %ld\n", previous, ftell( datafile ) );
			phone_num++;
		}
		fprintf( datafile, "%hu %s\t", word_data[ i ].num[0], word_data[ i ].word );
	}
	fprintf( indexfile, "0 %ld\n", ftell( datafile ) );
	fprintf( configfile, "#define PHONE_NUM (%d)\n", phone_num );
	fclose( indexfile );
	fclose( datafile );
	fclose( configfile );
}

void CountSort()
{
	int number[ MAX_NUMBER ], i, place;
	WORD_DATA oldData[ MAX_WORD ];

	memset( number, 0, sizeof( number ) );
	for ( i = 0; i < nWord; i++ )
		number[ word_data[ i ].num[0] ]++;
	memmove( &number[ 1 ], number, sizeof( int ) * ( MAX_NUMBER - 1 ) );
	for ( i = 2; i < MAX_NUMBER; i++)
		number[ i ] += number[ i - 1 ];

	memcpy( oldData, word_data, sizeof( WORD_DATA ) * nWord );
	for ( i = 0; i < nWord; i++ ) {
		place = number[ oldData[ i ].num[0] ]++;
		memcpy( &word_data[ place ], &oldData[ i ], sizeof( WORD_DATA ) );
	}
}

#ifdef USED_IN_DAT2BIN
int sort_word()
#else
int main()
#endif
{
	FILE *cinfile;
	char buf[ MAX_BUF_LEN ];

	cinfile = fopen( PHONE_CIN_FILE, "r" );
	if ( ! cinfile ) {
		fprintf( stderr, "Error opening the file " PHONE_CIN_FILE "\n" );
		return 1;
	}

	do {
		fgets( buf, MAX_BUF_LEN, cinfile );
	} while ( strncmp( buf, CHARDEF_BEGIN, strlen( CHARDEF_BEGIN ) ) );

	for ( ; ; ) {
		fgets( buf, MAX_BUF_LEN, cinfile );
		if ( buf[ 0 ] == '%' )
			break;
		if ( DoWord( buf ) == DO_WORD_ERROR ) {
			fprintf( stderr, "The file " PHONE_CIN_FILE " is corrupted!\n" );
			return 1;
		}
	}
	fclose( cinfile );

	if ( strncmp( buf, CHARDEF_END, strlen( CHARDEF_END ) ) ) {
		fprintf( stderr, "The end of the file " PHONE_CIN_FILE " is error!\n" );
		return 1;
	}

	CountSort();
	Output();

	return 0;
}

