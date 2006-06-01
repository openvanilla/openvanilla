/**
 * key2pho.c
 *
 * Copyright (c) 2004, 2005, 2006
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
#include "chewing-utf8-util.h"
#include "global.h"

const char *zhuin_tab[] = {					/* number of bits */
	  "  ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙ",	/* 5 */
	  "  ㄧㄨㄩ",						/* 2 */
	  "  ㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ",			/* 4 */
	  "  ˙ˊˇˋ"						/* 3 */
};

static const int zhuin_tab_num[] = { 22, 4, 14, 5 };
static const int shift[] = { 9, 7, 3, 0 };
const static int sb[] = { 31, 3, 15, 7 };

static const char *ph_str =
	"ㄅㄆㄇㄈ"
	"ㄉㄊㄋㄌ"
	"ㄍㄎㄏㄐ"
	"ㄑㄒㄓㄔ"
	"ㄕㄖㄗㄘ"
	"ㄙㄧㄨㄩ"
	"ㄚㄛㄜㄝ"
	"ㄞㄟㄠㄡ"
	"ㄢㄣㄤㄥㄦ"
	"˙ˊˇˋ" ;
static char *key_str[ MAX_KBTYPE ] = {
	 "1qaz2wsxedcrfv5tgbyhnujm8ik,9ol.0p;/-7634",		/* standard kb */
	 "bpmfdtnlgkhjvcjvcrzasexuyhgeiawomnkllsdfj",		/* hsu */
	 "1234567890-qwertyuiopasdfghjkl;zxcvbn/m,.",		/* IBM */
	 "2wsx3edcrfvtgb6yhnujm8ik,9ol.0p;/-['=1qaz",		/* Gin-yieh */
	 "bpmfdtnlvkhg7c,./j;'sexuaorwiqzy890-=1234",		/* ET  */
	 "bpmfdtnlvkhgvcgycjqwsexuaorwiqzpmntlhdfjk",		/* ET26 */
	 "1'a;2,oq.ejpuk5yixfdbghm8ctw9rnv0lsz[7634",		/* Dvorak */
	 "bpmfdtnlgkhjvcjvcrzasexuyhgeiawomnklldhtn",		/* Dvorak Hsu */
	 "1qaz2wsxedcrfv5tgbyhnujm8ik,9ol.0p;/-7634",		/* pinyin */
} ;

/* 
 * Read one zhuin string,
 *
 * return the number it means
 */
uint16 UintFromPhone( const char *zhuin )
{
	char *iter, *pos;
	char buf[ 7 ];
	int len, result = 0;
	int i;

	iter = (char*) zhuin;
	/* Here the constant 4 is the number
	   of zhuin_tab and zhuin_tab_num */
	for ( i = 0; i < 4; i++ ) {
		/* Should be less than 4, how do we handle this? */
		len = ueBytesFromChar( iter[ 0 ] );
		strncpy( buf, iter, len );
		buf[len] = '\0';
		if (! buf[0])
			continue;
		pos = strstr( zhuin_tab[ i ], buf );
		if ( pos ) {
			result |= (zhuin_tab_num[ i ] - ueStrLen( pos )) << shift[ i ];
			iter += len;
		}
	}
	return result;
}

int PhoneFromKey( char *pho, const char *inputkey, int kbtype, int searchTimes )
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
		ueStrNCpy( ueStrSeek( pho, i ),
		           ueStrSeek( (char *) ph_str, index ), 
			   1, 0);
	}
	pTarget = ueStrSeek( pho, len );
	pTarget[0] = '\0';
	return 1;
}

#if 0
int PhoneFromUint( char *phone, long seq )
{
    int i, j, k;
    char *pos;
    char buffer[7];
    for ( i = 0, j = 0; i < 4; i++) {
        k = ((seq >> shift[ i ]) & sb[ i ] ) - 1;
        if ( k >= 0 && (pos = ueStrSeek( (char*)zhuin_tab[ i ], k )) )
        {
			ueStrNCpy(buffer, pos, 1, 1);
            strcat(phone, buffer);
            j++;
        }
    }
    return j;
}
#endif

int PhoneInxFromKey( int key, int type, int kbtype, int searchTimes )
{
	char keyStr[ 2 ], rtStr[ 10 ], *p;

	keyStr[ 0 ] = key;
	keyStr[ 1 ] = '\0';
	if ( ! PhoneFromKey( rtStr, keyStr, kbtype, searchTimes ) )
		return 0;
	p = strstr( zhuin_tab[ type ], rtStr );
	if ( ! p )
		return 0;
	return zhuin_tab_num[type] - ueStrLen(p);
}

uint16 UintFromPhoneInx( const int ph_inx[] )
{
	int i;
	uint16 result = 0;

	for ( i = 0; i < ZUIN_SIZE; i++ )
		result |= ph_inx[ i ] << shift[ i ];
	return result;
}

