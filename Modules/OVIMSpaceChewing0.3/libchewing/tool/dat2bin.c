/**
 * dat2bin.c
 *
 * Copyright (c) 2005
 *	libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

/* @(#)dat2bin.c
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "char.h"
#include "private.h"

/* Some wrapper definitions */
#ifdef WIN32
#include <direct.h>
#define CHDIR(x) _chdir(x)
#define GETCWD(x,y) _getcwd(x,y)
#else /* POSIX */
#define CHDIR(x) chdir(x)
#define GETCWD(x,y) getcwd(x,y)
#endif

/* Let's include other utilities */
#define USED_IN_DAT2BIN
#include "sort_word.c"
#include "sort_dic.c"
#include "maketree.c"
#undef USED_IN_DAT2BIN

int main( int argc, char* argv[] )
{
	char* prefix = argc < 2 ? "" : argv[1];
	char filename[ 100 ];
	char workdir[ 100 ];
	int i;

	if ( *prefix )
		CHDIR( prefix );
	else {
		GETCWD( workdir, 100 );
		prefix = workdir;
	}

	printf("sort_dic...\n");
	sort_dic();

	printf("maketree...\n");
	maketree();

	printf("sort_word...\n");
	sort_word();

	printf("convert dat files to binary format...\n");

	sprintf( filename, "%s" PLAT_SEPARATOR "%s", prefix, CHAR_INDEX_FILE );
	FILE* fi = fopen( filename, "r" );
	if ( !fi )
		return 1;

	strcat(filename, "_bin");
	FILE* fo = fopen( filename, "wb" );

	if( fo )
	{
		static uint16 *arrPhone;
		static int *begin;
		arrPhone = ALC( uint16, phone_num * 2 );
		begin = ALC( int, phone_num * 2 );
		for ( i = 0; i <= phone_num * 2; i++ )
		{
			if( fscanf( fi, "%hu %d", &arrPhone[i], &begin[i] ) < 2 )
			{
				fwrite( begin, sizeof(int), i, fo );
				fwrite( arrPhone, sizeof(uint16), i, fo );
				break;
			}
		}
		free( arrPhone );
		free( begin );
		fclose( fo );
	}
	fclose( fi );

	sprintf( filename, "%s" PLAT_SEPARATOR "%s", prefix, PHONE_TREE_FILE );
	fi = fopen( filename, "r" );
	if ( !fi )
		return 1;
	strcat(filename, "_bin");
	fo = fopen( filename, "wb" );

	if( fo )
	{
		TreeType tree = {0};
		for ( i = 0; i < tree_size * 2; i++ ) {
			if ( fscanf( fi, "%hu%d%d%d",
				&tree.phone_id,
				&tree.phrase_id,
				&tree.child_begin,
				&tree.child_end ) != 4 )
				break;
			fwrite( &tree, sizeof(TreeType), 1, fo );
		}
		fclose( fo );
	}
	fclose( fi );

	sprintf( filename, "%s" PLAT_SEPARATOR "%s", prefix, PH_INDEX_FILE );
	fi = fopen( filename, "r" );
	if( !fi )
		return 1;
	strcat( filename, "_bin" );
	fo = fopen( filename, "wb" );
	if( fo )
	{
		int i = 0;
		int begin;
		while ( !feof( fi ) )
		{
			fscanf( fi, "%d", &begin );
			fwrite( &begin, sizeof(int), 1, fo );
		}
		fclose(fo);
	}
	fclose( fi );
	return 0;
}

