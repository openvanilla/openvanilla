// vanillaphone.h: vanillaphone library
#ifndef __VANILLAPHONE_H
#define __VANILLAPHONE_H


typedef char VPChar;			// internal representation of ascii
typedef unsigned short VPSymbol;	// maskable symbol
typedef unsigned short VPOrdinal;   // "order" of the symbol, e.g. vpB=1, vbI=22
typedef unsigned short VPUTF16;

// 0 is always error in our system -- we don't use BOOL!

// convert functions
extern "C" VPSymbol	VPStandardKeyToSymbol (VPChar);
extern "C" VPSymbol	VPEtenKeyToSymbol (VPChar);
extern "C" VPOrdinal VPSymbolToOrdinal (VPSymbol);
extern "C" VPSymbol	VPOrdinalToSymbol (VPOrdinal);

// symbol manipulations and seach functions
extern "C" VPSymbol	VPCombineSymbol(VPSymbol, VPSymbol);
extern "C" VPSymbol	VPDeleteSymbolLastPart(VPSymbol);
// extern "C" int		VPFindCandidate(VPSymbol, VPUTF16*);		// get candidate list

// text services
extern "C" VPUTF16	VPSymbolCharUTF16(VPSymbol);			// returns the UTF-16 for single char
extern "C" int		VPSymbolStringUTF16(VPSymbol, VPUTF16*);  // copies the UTF-16 string
extern "C" int		VPSymbolCharBig5(VPSymbol, VPChar*);	// ditto, but copies BIG5 instead
extern "C" int		VPSymbolStringBig5(VPSymbol, VPChar*);  // ditto

// punctuation services;
extern "C" VPUTF16  VPPunctuationWindowKeyToCode(VPChar);
extern "C" int		VPFindPunctuationCandidate(VPChar, VPUTF16*);

// service functions
extern "C" int		VPCheckFormation (VPSymbol);		// check the integrity of the symbol
extern "C" VPChar   VPToUpper (VPChar);
extern "C" VPChar   VPToLower (VPChar);

// constants
#define vpConsonantMask		0x001f		// 0000 0000 0001 1111, 21 consonants
#define vpMiddleVowelMask   0x0060		// 0000 0000 0110 0000, 3 middle vowels
#define vpVowelMask			0x0780		// 0000 0111 1000 0000, 13 vowels
#define vpToneMask			0x3800		// 0011 1000 0000 0000, 5 tones (tone1=0x00)
#define vpB			0x0001
#define vpP			0x0002
#define vpM			0x0003
#define vpF			0x0004
#define vpD			0x0005
#define vpT			0x0006
#define vpN			0x0007
#define vpL			0x0008
#define vpG			0x0009
#define vpK			0x000a
#define vpH			0x000b
#define vpJ			0x000c
#define vpQ			0x000d
#define vpX			0x000e
#define vpZH		0x000f
#define vpCH		0x0010
#define vpSH		0x0011
#define vpR			0x0012
#define vpZ			0x0013
#define vpC			0x0014
#define vpS			0x0015
#define vpI			0x0020
#define vpU			0x0040
#define vpV			0x0060
#define vpA			0x0080
#define vpO			0x0100
#define vpER		0x0180
#define vpE			0x0200
#define vpAI		0x0280
#define vpEI		0x0300
#define vpAO		0x0380
#define vpOU		0x0400
#define vpAN		0x0480
#define vpEN		0x0500
#define vpANG		0x0580
#define vpENG		0x0600
#define vpERR		0x0680
#define vpTone1		0x0000
#define vpTone2		0x0800
#define vpTone3		0x1000
#define vpTone4		0x1800
#define vpTone5		0x2000

#endif		// #ifdef __VANILLAPHONE_H
