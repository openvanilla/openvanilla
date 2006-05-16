/**
 * userphrase.h
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

#ifndef USERPHRASE_H
#define USERPHRASE_H

#define FREQ_INIT_VALUE (1)
#define SHORT_INCREASE_FREQ (10)
#define MEDIUM_INCREASE_FREQ (5)
#define LONG_DECREASE_FREQ (10)
#define MAX_ALLOW_FREQ (99999999)

#define USER_UPDATE_FAIL (4)
#define USER_UPDATE_INSERT (1)
#define USER_UPDATE_MODIFY (2)
#define USER_UPDATE_IGNORE (8)

#include "global.h"

typedef struct {
	uint16 *phoneSeq;
	char *wordSeq;
	int userfreq;
	int recentTime;
	int origfreq;	/* the initial frequency of this phrase */
	int maxfreq;	/* the maximum frequency of the phrase of the same pid */
} UserPhraseData ;

/**
 * @brief Update or add a new UserPhrase.
 *
 * @param phoneSeq[] Phone sequence
 * @param wordSeq[] Phrase against the phone sequence
 *
 * @return
 * @retval USER_UPDATE_FAIL Update fail.
 * @retval USER_UPDATE_INSERT Sequence is new, add new entry.
 * @retval USER_UPDATE_MODIFY Sequence is existing, update it's data.
 */
int UserUpdatePhrase( const uint16 phoneSeq[], const char wordSeq[] );

/**
 * @brief Read the first phrase of the phone in user phrase database.
 *
 * @param phoneSeq[] Phone sequence
 * 
 * @return UserPhraseData, if it's not existing then return NULL.
 */
UserPhraseData *UserGetPhraseFirst( const uint16 phoneSeq[] );

/**
 * @brief Read the next phrase of the phone in user phrase database.
 * 
 * @param phoneSeq[] Phone sequence
 *
 * @return UserPhraseData, if it's not existing then return NULL.
 */
UserPhraseData *UserGetPhraseNext( const uint16 phoneSeq[] );

#endif

