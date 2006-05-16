/**
 * chewingutil.h
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

#ifndef _CHEWING_UTIL_H
#define _CHEWING_UTIL_H

#define SYMBOL_KEY_OK 0
#define SYMBOL_KEY_ERROR 1
#define DECREASE_CURSOR 1
#define NONDECREASE_CURSOR 0

void AutoLearnPhrase( ChewingData *pgdata );
void SetUpdatePhraseMsg( ChewingData *pgdata, char *addWordSeq, int len, int state );
int NoSymbolBetween( ChewingData *pgdata, int begin, int end );
int ChewingIsEntering( ChewingData *pgdata );
void CleanAllBuf( ChewingData * );
int SemiSymbolInput(int key, ChewingData *pgdata);
int SpecialEtenSymbolInput( int key, ChewingData *pgdata );
int SpecialSymbolInput( int key, ChewingData *pgdata );
int FullShapeSymbolInput( int key, ChewingData *pgdata );
int SymbolInput( int key, ChewingData *pgdata );
int SymbolChoice( ChewingData *pgdata, int sel_i );
int HaninSymbolInput(ChoiceInfo *pci, AvailInfo *pai, const uint16 phoneSeq[],  int selectAreaLen);
int WriteChiSymbolToBuf( wch_t csBuf[], int csBufLen, ChewingData *pgdata );
int ReleaseChiSymbolBuf( ChewingData *pgdata, ChewingOutput *);
int AddChi( uint16 phone, ChewingData *pgdata );
int CallPhrasing( ChewingData *pgdata );
int MakeOutputWithRtn( ChewingOutput *pgo, ChewingData *pgdata, int keystrokeRtn );
void MakeOutputAddMsgAndCleanInterval( ChewingOutput *pgo, ChewingData *pgdata );
int AddSelect( ChewingData *pgdata, int sel_i );
int CountSelKeyNum( int key, ChewingData *pgdata );
int ChewingIsChiAt( int cursor, ChewingData *pgdata );
static int ChewingKillSelectIntervalAcross( int cursor, ChewingData *pgdata );
int ChewingKillChar(
		ChewingData *pgdata, 
		int cursorToKill, 
		int chiSymbolCursorToKill, 
		int minus );
void RemoveSelectElement( int i, ChewingData *pgdata );
int IsPreferIntervalConnted( int cursor, ChewingData *pgdata );
int OpenSymbolChoice( ChewingData *pgdata );

#endif

