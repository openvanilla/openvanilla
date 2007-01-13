/**
 * key2pho.c
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
 * @file key2pho.c
 * @brief map zuins to uint16 type according to different kb_type
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "global.h"

const static int shift[] = { 9, 7, 3, 0 };
const char *ph_pho[] = {				/* number of bits */
	"  £t£u£v£w£x£y£z£{£|£}£~£¡£¢£££¤£¥£¦£§£¨£©£ª",	/* 5 */
	"  £¸£¹£º",					/* 2 */
	"  £«£¬£­£®£¯£°£±£²£³£´£µ£¶£·",			/* 4 */
	"  £»£½£¾£¿"					/* 3 */
};
const char *ph_str =
"£t£u£v£w£x£y£z£{£|£}£~£¡£¢£££¤£¥£¦£§£¨£©£ª£¸£¹£º£«£¬£­£®£¯£°£±£²£³£´£µ£¶£·£»£½£¾£¿" ;

char *key_str[ MAX_KBTYPE ] = {
	"1qaz2wsxedcrfv5tgbyhnujm8ik,9ol.0p;/-7634",		/* standard kb */
	"bpmfdtnlgkhjvcjvcrzasexuyhgeiawomnkllsdfj",		/* hsu */
	"1234567890-qwertyuiopasdfghjkl;zxcvbn/m,.",		/* IBM */
	"2wsx3edcrfvtgb6yhnujm8ik,9ol.0p;/-['=1qaz",		/* Gin-yieh */
	"bpmfdtnlvkhg7c,./j;'sexuaorwiqzy890-=1234",		/* ET  */
	"bpmfdtnlvkhgvcgycjqwsexuaorwiqzpmntlhdfjk",		/* ET26 */
	"1'a;2,oq.ejpuk5yixfdbghm8ctw9rnv0lsz[7634",		/* Dvorak */
	"bpmfdtnlgkhjvcjvcrzasexuyhgeiawomnklldhtn",		/* Dvorak Hsu */
	"1qaz2wsxedcrfv5tgbyhnujm8ik,9ol.0p;/-7634",		/* pinying */
} ;

int Key2PhoneInx( int key, int type, int kbtype, int searchTimes )
{
	char keyStr[ 5 ], bgStr[ 10 ], *p;

	keyStr[ 0 ] = key;
	keyStr[ 1 ] = '\0';
	Key2Pho( bgStr, keyStr, kbtype, searchTimes );
	p = strstr( ph_pho[ type ], bgStr );
	if ( ! p )
		return 0;
        /*
          This /2 here is because ph_pho[] string array uses
          big5. Every char is 2 bytes.
         */
	return ( p - ph_pho[ type ] ) / 2;
}

uint16 PhoneInx2Uint( const int ph_inx[] )
{
	int i;
	uint16 result = 0;

	for ( i = 0; i < ZUIN_SIZE; i++ )
		result |= ph_inx[ i ] << shift[ i ];
	return result;
}

/*
   Translate a single phone into a single uint
   assume phone is a big5 null-terminated string with no spaces
   */
uint16 PhoneBg2Uint( const char *phone )
{
	int i, j, len = strlen( phone );
	uint16 result = 0;
	char temp[ 3 ];
	char *pc;

        /*
          The constant 4 in this loop is the size of ph_pho[] ( j =
          0,1,2,3) , instead of the max length of a zuin-sequence. So
          it shouldn't be replaced with ZUIN_SIZE.
         */
	for ( i = 0, j = 0; i < len && j < 4; j++ ) {
		temp[ 0 ] = phone[ i ];
		temp[ 1 ] = phone[ i + 1 ];
		temp[ 2 ] = 0;
		pc = strstr( ph_pho[ j ], temp );
		if ( pc ) {
			result |= ( ( pc - ph_pho[ j ] ) >> 1 ) << shift[ j ];
			i += 2;
		}
	}
	assert( i == len );
	return result;
}

int Key2Pho( char *pho, const char *inputkey, int kbtype, int searchTimes )
{
	int len = strlen( inputkey ), i, s;
	char *pTarget;

	pho[ 0 ] = '\0';
	for ( i = 0; i < len; i++ ) {
		char *findptr = NULL;
		int index;

		for ( 
			s = 0, pTarget = key_str[ kbtype ]; 
			s < searchTimes; 
			s++, pTarget = findptr + 1 ) {
			findptr = strchr( pTarget, inputkey[ i ] );
			if ( ! findptr ) {
				return 0;
			}
		}
		index = findptr - key_str[ kbtype ];
		pho[ i * 2 ] = ph_str[ index * 2 ];
		pho[ i * 2 + 1 ] = ph_str[ index * 2 + 1 ];
	}
	pho[ len * 2 ] = '\0';
	return 1;
}

