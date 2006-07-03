/**
 * zuin.c
 *
 * Copyright (c) 1999, 2000, 2001
 *      Lu-chuan Kung and Kang-pen Chen.
 *      All rights reserved.
 *
 * Copyright (c) 2004
 *      libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

/**
 * @file zuin.c 
 *
 * control keyboard mapping
 * include the definition of ZuinData structure
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "zuin.h"
#include "char.h"
#include "hanyupinying.h"
#include "private.h"

int IsDvorakHsuPhoEndKey( int pho_inx[], int key )
{
	switch( key ) {
		case 'd':
		case 'h':
		case 't':
		case 'n':
		case ' ':
			return ( pho_inx[ 0 ] || pho_inx[ 1 ] || pho_inx[ 2 ] );
		default:
			return 0;
	}
}

/*
 * process a key input
 * return value:
 *	ZUIN_ABSORB
 *	ZUIN_COMMIT
 *	ZUIN_KEY_ERROR
 *	ZUIN_ERROR
 */
int IsHsuPhoEndKey( int pho_inx[], int key )
{
	switch( key ) {
		case 's':
		case 'd':
		case 'f':
		case 'j':
		case ' ':
			return ( pho_inx[ 0 ] || pho_inx[ 1 ] || pho_inx[ 2 ] );
		default:
			return 0;
	}
}

/* copy the idea from HSU keyboard */
int IsET26PhoEndKey( int pho_inx[], int key )
{
	switch( key ) {
		case 'd':
		case 'f':
		case 'j':
		case 'k':
		case ' ':
			return ( pho_inx[ 0 ] || pho_inx[ 1 ] || pho_inx[ 2 ] );
		default:
			return 0;
	}
}

int IsDefPhoEndKey( int key, int kbtype )
{
	if ( Key2PhoneInx( key, 3, kbtype, 1 )  )
		return 1;
	
	if ( key == ' ' )
		return 1;
	return 0;
}

int EndKeyProcess( ZuinData *pZuin, int key, int searchTimes )
{
	uint16 u16Pho;
	Word tempword;

	if ( 
		pZuin->pho_inx[ 0 ] == 0 && 
		pZuin->pho_inx[ 1 ] == 0 && 
		pZuin->pho_inx[ 2 ] == 0 ) {
		/*
		 * Special handle for space key (Indeed very special one).
		 * Un-break the situation that OnKeySpace() is not called,
		 * hence the Candidate window doesn't show up, because
		 * ZUIN_NO_WORD is returned.
		 */
		return (key == ' ') ? ZUIN_KEY_ERROR : ZUIN_NO_WORD;
	}

	pZuin->pho_inx[ 3 ] = Key2PhoneInx( key, 3, pZuin->kbtype, searchTimes );
	u16Pho = PhoneInx2Uint( pZuin->pho_inx );
	if ( GetCharFirst( &tempword, u16Pho ) == 0 ) {
		ZuinRemoveAll( pZuin );
		return ZUIN_NO_WORD;
	}
	
	pZuin->phone = u16Pho;
	memset( pZuin->pho_inx, 0, sizeof( pZuin->pho_inx ) );
	return ZUIN_COMMIT;
}

int DefPhoInput( ZuinData *pZuin, int key )
{
	int type=0, inx = 0;

	if ( IsDefPhoEndKey( key, pZuin->kbtype ) ) {
		return EndKeyProcess( pZuin, key, 1 );
	}
		
	/* decide if the key is a phone */
	for ( type = 0; type < 3; type++ ) {
		inx = Key2PhoneInx( key, type, pZuin->kbtype, 1 );
		if ( inx )
			break;
	}
	
	/* the key is NOT a phone */
	if ( type == 3 ) {
		return ZUIN_KEY_ERROR;
	}
	
	/* fill the key into the phone buffer */
	pZuin->pho_inx[ type ] = inx;
	return ZUIN_ABSORB;
}

int HsuPhoInput( ZuinData *pZuin, int key )
{
	int type = 0, searchTimes = 0, inx = 0;

	if ( 
		( IsHsuPhoEndKey( pZuin->pho_inx, key ) && 
			( pZuin->kbtype == KB_HSU ) ) ||
		( IsDvorakHsuPhoEndKey( pZuin->pho_inx, key ) && 
			( pZuin->kbtype == KB_DVORAK_HSU ) ) ) {
		if ( pZuin->pho_inx[ 1 ] == 0 && pZuin->pho_inx[ 2 ] == 0 ) {
			/* convert "£¡£¢££" to "£¤£¥£¦" */
			if ( 12 <= pZuin->pho_inx[ 0 ] && pZuin->pho_inx[ 0 ] <= 14 ) {
				pZuin->pho_inx[ 0 ] += 3 ;
			}
			/* convert "£~" to "£¬" */
			else if ( pZuin->pho_inx[ 0 ] == 11 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 2;
			}
			/* convert "£|" to "£­" */
			else if ( pZuin->pho_inx[ 0 ] == 9 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 3;
			}
			/* convert "£v" to "£³" */
			else if ( pZuin->pho_inx[ 0 ] == 3 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 9;
			}
			/* convert "£z" to "£´" */
			else if ( pZuin->pho_inx[ 0 ] == 7 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 10;
			}
			/* convert "£}" to "£µ" */
			else if ( pZuin->pho_inx[ 0 ] == 10 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 11;
			}
			/* convert "£{" to "£·" */
			else if ( pZuin->pho_inx[ 0 ] == 8 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 13;
			}
		}

		if (
			( pZuin->pho_inx[ 0 ] == 9 ) && 
			( ( pZuin->pho_inx[ 1 ] == 1 ) || 
				( pZuin->pho_inx[ 1 ] == 3 ) ) ) {
			pZuin->pho_inx[ 0 ] = 12;
		}

		if ( pZuin->kbtype == KB_HSU )
			searchTimes = ( key == 'j' ) ? 3 : 2;
		else if ( pZuin->kbtype == KB_DVORAK_HSU )
			searchTimes = ( key == 'h' || key == 'n' ) ? 3 : 2 ;

		return EndKeyProcess( pZuin, key, searchTimes );
	}
	else {
		/* decide if the key is a phone */
		for ( type = 0, searchTimes = 1; type < 3; type++ ) {
			inx = Key2PhoneInx( key, type, pZuin->kbtype, searchTimes );
			if ( ! inx )
				continue; /* if inx == 0, next type */
			else if ( type == 0 ) {
				if ( pZuin->pho_inx[ 0 ] || pZuin->pho_inx[ 1 ] ) {
					/* if inx !=0 */
					searchTimes = 2 ; /* possible infinite loop here */
				}
				else 
					break;
			}
			else if ( type == 1 && inx == 1 ) { /* handle i and e*/
				if ( pZuin->pho_inx[ 1 ] ) {
					searchTimes = 2;
				}
				else 
					break;
			}
			else 
				break;
		}
		/* processing very special cases "j v c" */
		if ( 
			type == 1 && 
			inx == 2 && 
			12 <= pZuin->pho_inx[ 0 ] && 
			pZuin->pho_inx[ 0 ] <= 14 ) {
			pZuin->pho_inx[ 0 ] += 3;
		}

		/* Fuzzy "g e" to "j e" */
		if (
			( pZuin->pho_inx[ 0 ] == 9 ) && 
			( ( pZuin->pho_inx[ 1 ] == 1 ) || ( pZuin->pho_inx[ 1 ] == 3 ) ) ) {
			pZuin->pho_inx[ 0 ] = 12;
		}

		/* £¡£¢££ must follow £¸£º */
		if (
			type == 2 && 
			pZuin->pho_inx[ 1 ] == 0 && 
			12 <= pZuin->pho_inx[ 0 ] && 
			pZuin->pho_inx[ 0 ] <= 14 ) {
			pZuin->pho_inx[ 0 ] += 3;
		}

		if ( type == 3 ) { /* the key is NOT a phone */
			if ( isalpha( key ) )
				return ZUIN_NO_WORD;
			return ZUIN_KEY_ERROR;
		}
		/* fill the key into the phone buffer */
		pZuin->pho_inx[ type ] = inx;
		return ZUIN_ABSORB;
	}
}

/* copy the idea from hsu */
int ET26PhoInput( ZuinData *pZuin, int key ) 
{
	int type = 0, searchTimes = 0, inx = 0;

	if ( IsET26PhoEndKey( pZuin->pho_inx, key ) ) {
		if ( pZuin->pho_inx[ 1 ] == 0 && pZuin->pho_inx[ 2 ] == 0 ) {
			/* convert "£¡££" to "£¤£¦" */
			if ( pZuin->pho_inx[ 0 ] == 12 || pZuin->pho_inx[ 0 ] == 14 ) {
				pZuin->pho_inx[ 0 ] += 3;
			}
			/* convert "£u" to "£²" */
			else if ( pZuin->pho_inx[ 0 ] == 2 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 8;
			}
			/* convert "£v" to "£³" */
			else if ( pZuin->pho_inx[ 0 ] == 3 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 9;
			}
			/* convert "£z" to "£´" */
			else if ( pZuin->pho_inx[ 0 ] == 7) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 10;
			}
			/* convert "£y" to "£µ" */
			else if ( pZuin->pho_inx[ 0 ] == 6 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 11;
			}
			/* convert "£{" to "£¶" */
			else if ( pZuin->pho_inx[ 0 ] == 8 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 12;
			}
			/* convert "£~" to "£·" */
			else if ( pZuin->pho_inx[ 0 ] == 11 ) {
				pZuin->pho_inx[ 0 ] = 0;
				pZuin->pho_inx[ 2 ] = 13;
			}
		}
		searchTimes = 2;
		return EndKeyProcess( pZuin, key, searchTimes );
	}
	else {
		/* decide if the key is a phone */
		for ( type = 0, searchTimes = 1; type < 3; type++ ) {
			inx = Key2PhoneInx( key, type, pZuin->kbtype, searchTimes );
			if ( ! inx ) 
				continue; /* if inx == 0, next type */
			else if ( type == 0 ) {
				if ( pZuin->pho_inx[ 0 ] || pZuin->pho_inx[ 1 ] ) {
					/* if inx !=0 */
					searchTimes = 2 ; /* possible infinite loop here */
				}
				else
					break;
			}
			else
				break;	
		}
		/* convert "£¡££" to "£¤£¦" */
		if ( type == 1 ) {
			if ( inx == 2 ) {
				if ( 
					pZuin->pho_inx[ 0 ] == 12 || 
					pZuin->pho_inx[ 0 ] == 14 ) {
					pZuin->pho_inx[ 0 ] += 3;
				}
			}
			else {
				/* convert "£|" to "£¢" */
				if ( pZuin->pho_inx[ 0 ] == 9 ) {
					pZuin->pho_inx[ 0 ] = 13;	
				}
			}
		}

		if ( 
			type == 2 && 
			pZuin->pho_inx[ 1 ] == 0 && 
			(pZuin->pho_inx[ 0 ] == 12 || pZuin->pho_inx[ 0 ] == 14 ) ) {
			pZuin->pho_inx[ 0 ] += 3;
		}

		if ( type == 3 ) { /* the key is NOT a phone */
			if ( isalpha( key ) )
				return ZUIN_NO_WORD;
			return ZUIN_KEY_ERROR;
		}
		/* fill the key into the phone buffer */
		pZuin->pho_inx[ type ] = inx;
		return ZUIN_ABSORB;
	}
}

int IsPinYingEndKey( ZuinData *pZuin, int key )
{
	if ( (key == ' ') || (key == '1') || (key == '2') ||
			(key == '3') || (key == '4') || (key == '5') ) {
		return 1;
	}
	return 0;
}

int PinYingInput( ZuinData *pZuin, int key )
{
	int err = 0, status, i;
	char zuinKeySeq[ 5 ], buf[ 2 ];

	DEBUG_CHECKPOINT();

	if ( IsPinYingEndKey( pZuin, key ) ) {
		err = HanyuPinYingToZuin( pZuin->pinYingData.keySeq, zuinKeySeq );
		if (err)
			return ZUIN_KEY_ERROR;

		DEBUG_OUT( "zuinKeySeq: %s\n", zuinKeySeq );
		for ( i = 0; i < strlen( zuinKeySeq ); i++ ) {
			status = DefPhoInput( pZuin, zuinKeySeq[ i ] );
			if ( status != ZUIN_ABSORB )
				return ZUIN_KEY_ERROR;
		}
		switch ( key ) {
			case '1':
				key = ' ';
				break;
			case '2':
				key = '6';
				break;
			case '5': 
				key = '7';
		}
		pZuin->pinYingData.keySeq[ 0 ] = '\0';
		return EndKeyProcess( pZuin, key, 1 );
	}
	buf[ 0 ] = key; buf[ 1 ] = '\0';
	strcat( pZuin->pinYingData.keySeq, buf );
	
	DEBUG_OUT( "PinYing Seq: %s\n", pZuin->pinYingData.keySeq );

	return ZUIN_ABSORB;
}

/* key: ascii code of input, including space */
int ZuinPhoInput(ZuinData *pZuin,int key )
{
	switch ( pZuin->kbtype ) {
		case KB_HSU:
		case KB_DVORAK_HSU:
			return HsuPhoInput( pZuin,key );
			break ;
		case KB_ET26:
			return ET26PhoInput( pZuin, key );
			break;
        	case KB_HANYU_PINYING:
                	return PinYingInput( pZuin, key );
                        break;
		default:
			return DefPhoInput( pZuin, key );		
	}	
	return ZUIN_ERROR;
}

/* remove the latest key */
int ZuinRemoveLast( ZuinData *pZuin )
{
	int i;
	if(pZuin->kbtype >= KB_HANYU_PINYING) {
		i = strlen(pZuin->pinYingData.keySeq);
		pZuin->pinYingData.keySeq[i-1]='\0';
	} else {
		for ( i = 3; i >= 0; i-- ) {
			if ( pZuin->pho_inx[ i ] ) {
				pZuin->pho_inx[ i ] = 0;
				return 0;
			}
		}
	}
	return 0;
}

/* remove all the key entered */
int ZuinRemoveAll( ZuinData *pZuin )
{
	memset( pZuin->pho_inx, 0, sizeof( pZuin->pho_inx ) );
	memset( pZuin->pinYingData.keySeq,0,sizeof(pZuin->pinYingData.keySeq));
	return 0;
}

int ZuinIsEntering( ZuinData *pZuin )
{
	int i;
        if(pZuin->kbtype >= KB_HANYU_PINYING) {
	    if(pZuin->pinYingData.keySeq[0])
		return 1;
        } else {
	    for ( i = 0; i < ZUIN_SIZE; i++ )
		if ( pZuin->pho_inx[ i ] )
		    return 1;
        }
	return 0;
}


/* Local Variables: */
/* c-indentation-style: linux */
/* c-basic-offset: 8 */
/* indent-tabs-mode: t */
/* End: */
