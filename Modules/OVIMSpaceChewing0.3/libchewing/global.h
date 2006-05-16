/**
 * global.h
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

#ifndef _CHEWING_GLOBAL_H
#define _CHEWING_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>

#define PHONE_TREE_FILE		"fonetree.dat"
#define DICT_FILE		"dict.dat"
#define PH_INDEX_FILE		"ph_index.dat"
#define CHAR_FILE		"us_freq.dat"
#define CHAR_INDEX_FILE		"ch_index.dat"
#define CHEWING_DEFINITION_FILE "chewing-definition.h"

#define IS_USER_PHRASE 1
#define IS_DICT_PHRASE 0
#define CHINESE_MODE 1
#define SYMBOL_MODE 0
#define FULLSHAPE_MODE 1
#define HALFSHAPE_MODE 0

#define MAX_KBTYPE 10
#define MAX_SELKEY 10
#define WCH_SIZE 4
#define MAX_UTF8_SIZE 6
#define ZUIN_SIZE 4
#define PINYIN_SIZE 10
#define MAX_PHRASE_LEN 10
#define MAX_PHONE_SEQ_LEN 50
#define MAX_INTERVAL ( ( MAX_PHONE_SEQ_LEN + 1 ) * MAX_PHONE_SEQ_LEN / 2 )
#define MAX_CHOICE (567)
#define MAX_CHOICE_BUF (50)                   /* max length of the choise buffer */

/* specified to Chewing API */
#define CHEWING_API

#ifndef max
#define max(a, b) \
	( (a) > (b) ? (a) : (b) )
#endif
#ifndef min
#define min(a, b) \
	( (a) < (b) ? (a) : (b) )
#endif

typedef unsigned short uint16;


typedef union {
	unsigned char s[ MAX_UTF8_SIZE + 1];
	wchar_t wch;
} wch_t;

typedef struct {
	uint16 phone_id;
	int phrase_id;
	int child_begin, child_end;
} TreeType;

typedef struct {
	int from, to;
} IntervalType;

typedef struct {
	char chiBuf[ MAX_PHONE_SEQ_LEN * MAX_UTF8_SIZE + 1 ];
	IntervalType dispInterval[ MAX_INTERVAL ];
	int nDispInterval;
	int nNumCut;
} PhrasingOutput;

typedef struct {
    int type;
    char keySeq[ PINYIN_SIZE ];
} PinYinData;

typedef struct {
	int kbtype;
	int pho_inx[ ZUIN_SIZE ];
	uint16 phone;
	PinYinData pinYinData;
} ZuinData;

typedef struct {
	/** @brief all kinds of lengths of available phrases. */
	struct {
		int len;
		/** @brief phone id. */
		int id;
	} avail[ MAX_PHRASE_LEN ];  
	/** @brief total number of availble lengths. */
	int nAvail;
	/** @brief the current choosing available length. */
	int currentAvail;
} AvailInfo;
/**
 *	@struct AvailInfo
 *	@brief information of available phrases or characters choices.
 */

typedef struct {
	/** @brief total page number. */
	int nPage;
	/** @brief current page number. */
	int pageNo;
	/** @brief number of choices per page. */
	int nChoicePerPage;
	/** @brief store possible phrases for being chosen. */
	char totalChoiceStr[ MAX_CHOICE ][ MAX_PHRASE_LEN * MAX_UTF8_SIZE + 1 ];
	/** @brief number of phrases to choose. */
	int nTotalChoice;
	int oldCursor, oldChiSymbolCursor;
	int isSymbol;
} ChoiceInfo;

/** @brief use "asdfjkl789" as selection key */
#define HSU_SELKEY_TYPE1 1
/** @brief use "asdfzxcv89" as selection key */
#define HSU_SELKEY_TYPE2 2

typedef struct {
	int selectAreaLen;
	int maxChiSymbolLen;
	int selKey[ MAX_SELKEY ];
	int bAddPhraseForward;
	int bSpaceAsSelection;
	int bEscCleanAllBuf;
	/** @brief
            HSU_SELKEY_TYPE1 = asdfjkl789,
            HSU_SELKEY_TYPE2 = asdfzxcv89.
         */
	int hsuSelKeyType;
} ChewingConfigData;

typedef struct {
	AvailInfo availInfo;
	ChoiceInfo choiceInfo;
	PhrasingOutput phrOut;
	ZuinData zuinData;
	ChewingConfigData config;
    /** @brief current input buffer, content==0 means Chinese code */
	wch_t chiSymbolBuf[ MAX_PHONE_SEQ_LEN ];
	int chiSymbolCursor;
	int chiSymbolBufLen;
	int PointStart;
	int PointEnd;
	wch_t showMsg[ MAX_PHONE_SEQ_LEN ];
	int showMsgLen;

	uint16 phoneSeq[ MAX_PHONE_SEQ_LEN ];
	int nPhoneSeq;
	int cursor;
	char selectStr[ MAX_PHONE_SEQ_LEN ][ MAX_PHONE_SEQ_LEN * MAX_UTF8_SIZE + 1 ];
	IntervalType selectInterval[ MAX_PHONE_SEQ_LEN ];
	int nSelect;
	IntervalType preferInterval[ MAX_INTERVAL ]; /* add connect points */
	int nPrefer;
	int bUserArrCnnct[ MAX_PHONE_SEQ_LEN + 1 ];
	int bUserArrBrkpt[ MAX_PHONE_SEQ_LEN + 1 ];   
	int bArrBrkpt[ MAX_PHONE_SEQ_LEN + 1 ];
	int bSymbolArrBrkpt[ MAX_PHONE_SEQ_LEN + 1 ];
	/* "bArrBrkpt[10]=True" means "it breaks between 9 and 10" */
	int bChiSym, bSelect, bCaseChange, bFirstKey, bFullShape, bAutoShiftCur;
	/* Symbol Key buffer */
	char symbolKeyBuf[ MAX_PHONE_SEQ_LEN ];
} ChewingData;

typedef struct {
	/** @brief the content of Edit buffer. */
	wch_t chiSymbolBuf[ MAX_PHONE_SEQ_LEN ];
	/** @brief the length of Edit buffer. */
	int chiSymbolBufLen;
	/** @brief current position of the cursor. */
	long chiSymbolCursor;
	long PointStart;
	long PointEnd;
	/** @brief the zuin-yin symbols have already entered. */
	wch_t zuinBuf[ ZUIN_SIZE ];
	/** @brief indicate the method of showing sentence break. */
	IntervalType dispInterval[ MAX_INTERVAL ]; /* from prefer, considering symbol */
	int nDispInterval;
	/** @brief indicate the break points going to display.*/ 
	int dispBrkpt[ MAX_PHONE_SEQ_LEN + 1 ];
	/** @brief the string going to commit. */
	wch_t commitStr[ MAX_PHONE_SEQ_LEN ];
	int nCommitStr;
	/** @brief information of character selections. */
	ChoiceInfo* pci;
	/** @brief indicate English mode or Chinese mode. */
	int bChiSym;
	int selKey[ MAX_SELKEY ];
	/** @brief return value. */
	int keystrokeRtn;
	int bShowMsg; 
	/** @brief user message. */
	wch_t showMsg[ MAX_PHONE_SEQ_LEN ];
	int showMsgLen;
} ChewingOutput;
/**
 *   @struct ChewingOutput
 *   @brief  information for Chewing output.
 */

typedef struct {
	ChewingData *data;
	ChewingOutput *output;
	int cand_no;
} ChewingContext;
/**
 * @struct ChewingContext
 * @brief context of Chewing IM
 */

typedef struct {
	char phrase[ MAX_PHRASE_LEN * MAX_UTF8_SIZE + 1 ];
	int freq;
} Phrase;

typedef struct {
	int from, to, pho_id, source;
	Phrase *p_phr;
} PhraseIntervalType;

/* tree.c */
int Phrasing( PhrasingOutput *ppo, uint16 phoneSeq[], int nPhoneSeq, 
		char selectStr[][ MAX_PHONE_SEQ_LEN * MAX_UTF8_SIZE + 1 ], 
		IntervalType selectInterval[], int nSelect, 
		int bArrBrkpt[], int bUserArrCnnct[] );
int IsIntersect( IntervalType in1, IntervalType in2 );
void ReadTree( const char * );
int TreeFindPhrase( int begin, int end, const uint16 *phoneSeq );

/* key2pho.c */
uint16 UintFromPhone( const char *phone );
uint16 UintFromPhoneInx( const int ph_inx[] );
int PhoneFromKey( char *pho, const char *inputkey, int kbtype, int searchTimes );
int PhoneInxFromKey( int key, int type, int kbtype, int searchTimes );

/* choice module */
int ChoiceFirstAvail( ChewingData * );
int ChoiceNextAvail( ChewingData * );
int ChoicePrevAvail( ChewingData * );
int ChoiceSelect( ChewingData *, int selectNo );
int ChoiceEndChoice( ChewingData * );

#endif

