/**
 * global.h
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

#ifndef _CHEWING_GLOBAL_H
#define _CHEWING_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>

#define PHONE_TREE_FILE		"fonetree.dat"
#define DICT_FILE		"dict.dat"
#define PH_INDEX_FILE		"ph_index.dat"
#define CHAR_FILE		"us_freq.dat"
#define CHAR_INDEX_FILE		"ch_index.dat"

#define IS_USER_PHRASE 1
#define IS_DICT_PHRASE 0
#define CHINESE_MODE 1
#define SYMBOL_MODE 0

#define MAX_KBTYPE 10
#define MAX_SELKEY 10
#define TREE_SIZE (153251)
#define WCH_SIZE 4
#define ZUIN_SIZE 4
#define PINYING_SIZE 10
#define MAX_PHRASE_LEN 10
#define MAX_PHONE_SEQ_LEN 50
#define MAX_INTERVAL ( ( MAX_PHONE_SEQ_LEN + 1 ) * MAX_PHONE_SEQ_LEN / 2 )
#define MAX_CHOICE (250)
#define MAX_CHOICE_BUF (50)                   /* max length of the choise buffer */

#ifndef max
#define max(a, b) \
	( (a) > (b) ? (a) : (b) )
#endif
#ifndef min
#define min(a, b) \
	( (a) < (b) ? (a) : (b) )
#endif

typedef unsigned short uint16;

typedef struct {
	char *inp_cname;
	char *inp_ename;
	int kb_type;
} ChewingConf;

typedef union {
	unsigned char s[ WCH_SIZE ];
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
	char chiBuf[ MAX_PHONE_SEQ_LEN * 2 + 1 ];
	IntervalType dispInterval[ MAX_INTERVAL ];
	int nDispInterval;
} PhrasingOutput;

typedef struct {
    int type;
    char keySeq[ PINYING_SIZE ];
} PinYingData;

typedef struct {
	int kbtype;
	int pho_inx[ ZUIN_SIZE ];
	uint16 phone;
	PinYingData pinYingData;
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
	char totalChoiceStr[ MAX_CHOICE ][ MAX_PHRASE_LEN * 2 + 1 ];
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
	/** @brief
            HSU_SELKEY_TYPE1 = asdfjkl789,
            HSU_SELKEY_TYPE2 = asdfzxcv89.
         */
	int hsuSelKeyType;
} ConfigData;

typedef struct {
	AvailInfo availInfo;
	ChoiceInfo choiceInfo;
	PhrasingOutput phrOut;
	ZuinData zuinData;
	ConfigData config;
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
	char selectStr[ MAX_PHONE_SEQ_LEN ][ MAX_PHONE_SEQ_LEN * 2 + 1 ];
	IntervalType selectInterval[ MAX_PHONE_SEQ_LEN ];
	int nSelect;
	IntervalType preferInterval[ MAX_INTERVAL ]; /* add connect points */
	int nPrefer;
	int bUserArrCnnct[ MAX_PHONE_SEQ_LEN + 1 ];
	int bUserArrBrkpt[ MAX_PHONE_SEQ_LEN + 1 ];   
	int bArrBrkpt[ MAX_PHONE_SEQ_LEN + 1 ];
	int bSymbolArrBrkpt[ MAX_PHONE_SEQ_LEN + 1 ];
	/* "bArrBrkpt[10]=True" means "it breaks between 9 and 10" */
	int bChiSym, bSelect, bCaseChange, bFirstKey;

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
	char phrase[ MAX_PHRASE_LEN * 2 + 1 ];
	int freq;
} Phrase;

typedef struct {
	int from, to, pho_id, source;
	Phrase *p_phr;
} PhraseIntervalType;

/* tree.c */
int Phrasing( PhrasingOutput *ppo, uint16 phoneSeq[], int nPhoneSeq, 
		char selectStr[][ MAX_PHONE_SEQ_LEN * 2 + 1 ], 
		IntervalType selectInterval[], int nSelect, 
		int bArrBrkpt[], int bUserArrCnnct[] );
int IsContain( IntervalType, IntervalType );
int IsIntersect( IntervalType, IntervalType );
void ReadTree( const char * );
int TreeFindPhrase( int begin, int end, const uint16 *phoneSeq );

/* key2pho.c */
uint16 PhoneBg2Uint( const char *phone );
uint16 PhoneInx2Uint( const int ph_inx[] );
int Key2Pho( char *pho, const char *inputkey, int kbtype, int searchTimes );
int Key2PhoneInx( int key, int type, int kbtype, int searchTimes );

/* choice module */
int ChoiceFirstAvail( ChewingData * );
int ChoiceNextAvail( ChewingData * );
int ChoicePrevAvail( ChewingData * );
int ChoiceSelect( ChewingData *, int selectNo );
int ChoiceEndChoice( ChewingData * );

#endif

