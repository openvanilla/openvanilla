/**
 * sort_dic.c
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
 * @file  sort_dic.c
 * @brief Sort and Index dictionary.\n
 *        Generate \b ph_index.dat (dictionary index) and \b dict.dat (content of dictionary) from
 *	  \b tsi.src (dictionary file in libtabe standard).
 *
 *	  Read dictionary format :
 *  	  phrase   frequency   zuin1 zuin2 zuin3 ... \n
 *  	  Output format : ( Sorted by zuin's uint16 number )
 *  	  phrase   frequency   zuin1 zuin2 zuin3 ... \n
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

#define MAXLEN		149
#define MAXZUIN		9
#define MAX_FILE_NAME	(256)

#define IN_FILE		"phoneid.dic"

typedef struct {
	char str[ MAXLEN ];
	int freq;
	uint16 num[ MAXZUIN ];
} RECORD;

RECORD data[ 420000L ];
long nData;

const char user_msg[] = 
	"sort_dic -- read chinese phrase input and generate data file for chewing\n" \
	"usage: \n" \
		"\tsort_dic <tsi file name> or \n" \
		"\tsort_dic (default name is tsi.src) \n" \
		"This program creates three new files. \n" \
		"1." DICT_FILE " \t-- main dictionary file \n" \
		"2." PH_INDEX_FILE " \t-- index file of phrase \n" \
		"3." IN_FILE " \t-- intermediate file for make_tree \n";

extern const char *ph_pho[];
/*extern uint16 PhoneBg2Uint( const char *phone );*/

void DataSetNum( long index )
{
	char buf[ MAXLEN ], *p;
	int i = 0;

	strcpy( buf, data[ index ].str );
	strtok( buf, " \n\t" );
	data[ index ].freq = atoi( strtok( NULL, " \n\t" ) );
	for ( p = strtok( NULL, " \n\t" ); p; p = strtok( NULL, " \n\t" ) ) 
		data[ index ].num[ i++ ] = UintFromPhone( p );
}

void SetNewline2Zero( long index )
{
	char *p;

	p = strchr( data[ index ].str, '\n' );
	if ( p )
		*p = '\0';
}

void DataStripSpace( long index )
{
	long i, k = 0;
	char old[ MAXLEN ], last = '\0';

	strcpy( old, data[ index ].str );
	for ( i = 0; old[ i ]; i++ ) {
		if ( old[ i ] == ' ' && last == ' ' )
			continue;
		data[ index ].str[ k++ ] = old[ i ];
		last = old[ i ];
	}
	data[ index ].str[ k ] = '\0';
}

int CompRecord( const void *a, const void *b )
{
	long i;
	int cmp;

	for ( i = 0; i < MAXZUIN; i++ ) {
		cmp = ((RECORD *) a)->num[ i ] - ((RECORD *) b)->num[ i ];
		if ( cmp )
			return cmp;
	}
	return ( ((RECORD *) b)->freq - ((RECORD *) a)->freq );
}

int CompUint( long a, long b )
{
	long i;

	for ( i = 0; i < MAXZUIN; i++ ) {
		if ( data[ a ].num[ i ] != data[ b ].num[ i ] )
			return 1;
	}
	return 0;
}

#ifdef USED_IN_DAT2BIN
int sort_dic()
#else
int main( int argc, char *argv[] )
#endif
{
	FILE *infile;
	FILE *dictfile, *treedata, *ph_index;
	char in_file[ MAX_FILE_NAME ] = "tsi.src";
	long i, k;

#ifndef USED_IN_DAT2BIN
	if ( argc < 2 ) 
		printf( user_msg );
	else 
		strcpy( in_file, argv[ 1 ] );
#endif

	infile = fopen( in_file, "r" );

	if ( !infile ) {
		fprintf ( stderr, "Error opening %s for reading!\n", in_file );
		exit( -1 );
	}

	dictfile = fopen( DICT_FILE, "w" );
	treedata = fopen( IN_FILE, "w" );
	ph_index = fopen( PH_INDEX_FILE, "w" );

	if ( !dictfile || !treedata || !ph_index ) {
		fprintf( stderr, "Error opening output file!\n" );
		exit( -1 );
	}

	while ( fgets( data[ nData ].str, MAXLEN, infile ) ) {
		DataStripSpace( nData );
		DataSetNum( nData );
		SetNewline2Zero( nData );
		nData++;
	}
	qsort( data, nData, sizeof( RECORD ), CompRecord );

	for ( i = 0; i < nData - 1; i++ ) {
		if ( ( i == 0 ) || ( CompUint( i, i - 1 ) != 0 ) ) 
			fprintf( ph_index, "%ld\n", ftell( dictfile ) );
		fprintf( dictfile, "%s\t", data[ i ].str );
	}
	fprintf( ph_index, "%ld\n", ftell( dictfile ) ); 
	fprintf( dictfile, "%s", data[ nData - 1 ].str );
	fprintf( ph_index, "%ld\n", ftell( dictfile ) );

	for ( i = 0; i < nData; i++ ) {
		if ( ( i > 0 ) && ( CompUint( i, i - 1 ) == 0 ) )
			continue;

		for ( k = 0; data[ i ].num[ k ]; k++ )
			fprintf (treedata, "%hu ", data[ i ].num[ k ] );
		fprintf( treedata, "0\n" );

	} 
	fclose( ph_index );
	fclose( dictfile );
	fclose( treedata );
	return 0;
}

