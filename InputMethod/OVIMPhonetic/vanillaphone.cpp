// vanillaphone.cpp

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vanillaphone.h"

VPSymbol vpStandardKeyTable[97]=
{
   0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
// 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 
// '!',  '"',  '#',  '$',  '%',  '&',  ''',  '(',
   0,    0,    0,    0,    0,    0,    0,    0,
// 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
// ')',  '*',  '+',  ',',  '-',  '.',  '/',  '0',   
   0,    0,    0,    vpE,  vpERR,vpOU, vpENG,vpAN,  
// 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
// '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',
   vpB,  vpD,  vpTone3,vpTone4,vpZH,vpTone2,vpTone5,vpA,
// 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 
// '9',  ':',  ';',  '<',  '=',  '>',  '?',  '@',  
   vpAI, 0,    vpANG,0,    0,    0,    0,    0,
// 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 
// 'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',
   vpM,  vpR,  vpH,  vpK,  vpG,  vpQ,  vpSH, vpC,
// 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 
// 'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',
   vpO,  vpU,  vpER, vpAO, vpV,  vpS,  vpEI, vpEN,
// 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 
// 'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  
   vpP,  vpJ,  vpN,  vpCH, vpI,  vpX,  vpT,  vpL,
// 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 
// 'Y',  'Z',  '[',  '\',  ']',  '^',  '_',  '`'
   vpZ,  vpF,  0,    0,    0,    0,    0,    0
};

VPSymbol vpEtenKeyTable[97]=
{
   0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
// 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,  
// '!',  '"',  '#',  '$',  '%',  '&',  ''',  '(',
   0,    0,    0,    0,    0,    0,    vpC,  0,
// 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
// ')',  '*',  '+',  ',',  '-',  '.',  '/',  '0',
   0,    0,    0,    vpZH, vpENG,vpCH, vpSH, vpANG,
// 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
// '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',
   vpTone5,vpTone2,vpTone3,vpTone4,0,0,vpQ,  vpAN,
// 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 
// '9',  ':',  ';',  '<',  '=',  '>',  '?',  '@',  
   vpEN, 0,    vpZ,  0,    vpERR,0,    0,    0,
// 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 
// 'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',
   vpA,  vpB,  vpX,  vpD,  vpI,  vpF,  vpJ,  vpH,
// 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 
// 'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',
   vpAI, vpR,  vpK,  vpL,  vpM,  vpN,  vpO,  vpP,
// 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
// 'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  
   vpEI, vpER, vpS,  vpT,  vpV,  vpG,  vpE,  vpU,
// 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 
// 'Y',  'Z',  '[',  '\',  ']',  '^',  '_',  '`'
   vpOU, vpAO, 0,    0,    0,    0,    0,    0
};

char vpSymbolBig5[]="\0\0\
\xa3\x74\xa3\x75\xa3\x76\xa3\x77\xa3\x78\xa3\x79\xa3\x7a\xa3\x7b\
\xa3\x7c\xa3\x7d\xa3\x7e\xa3\xa1\xa3\xa2\xa3\xa3\xa3\xa4\xa3\xa5\
\xa3\xa6\xa3\xa7\xa3\xa8\xa3\xa9\xa3\xaa\xa3\xb8\xa3\xb9\xa3\xba\
\xa3\xab\xa3\xac\xa3\xad\xa3\xae\xa3\xaf\xa3\xb0\xa3\xb1\xa3\xb2\
\xa3\xb3\xa3\xb4\xa3\xb5\xa3\xb6\xa3\xb7\xa3\xbd\xa3\xbe\xa3\xbf\
\xa3\xbb";

VPUTF16 vpSymbolUTF16[42]=
{
0,     12549, 12550, 12551, 12552, 12553, 12554, 12555, 12556, 12557,
12558, 12559, 12560, 12561, 12562, 12563, 12564, 12565, 12566, 12567,
12568, 12569, 12583, 12584, 12585, 12570, 12571, 12572, 12573, 12574,
12575, 12576, 12577, 12578, 12579, 12580, 12581, 12582, 714,   711,   
715,   729 
};

// convert functions ========================================================

extern "C" VPSymbol VPStandardKeyToSymbol (VPChar c)
{
	VPChar cc=VPToUpper(c);
	if (cc > 96) return 0;
	return vpStandardKeyTable[cc];
}

extern "C" VPSymbol VPEtenKeyToSymbol (VPChar c)
{
	VPChar cc=VPToUpper(c);
	if (cc > 96) return 0;
	return vpEtenKeyTable[cc];
}

// persumes that ONLY ONE SYMBOL is converted AND there ARE NO OTHER MASKED SYMBOLS
extern "C" VPOrdinal VPSymbolToOrdinal(VPSymbol s)
{
	if (!VPCheckFormation(s)) return 0;
	
	VPSymbol ss;
	
	if ((ss = (s & vpConsonantMask))) return ss;
	if ((ss = (s & vpMiddleVowelMask))) return (ss >> 5)+21;
	if ((ss = (s & vpVowelMask))) return (ss >> 7)+24;
	if ((ss = (s & vpToneMask))) return (ss >> 11)+37;
	return 0;
}

extern "C" VPSymbol VPOrdinalToSymbol(VPOrdinal o)
{
	if (o < 1 || o > 41) return 0;  // malformed ordinal
	
	if (o < 22) return (VPSymbol)o;					// 1~21 == consonants
	if (o < 25) return (VPSymbol)((o-21) << 5);		// 22~24 == middle vowels
	if (o < 38) return (VPSymbol)((o-24) << 7);		// 25~37 == vowels
	return (VPSymbol)((o-37) << 11);				// 38~41 == vpTone[2-5]
}


// symbol manipulation =============================================

extern "C" VPSymbol VPCombineSymbol(VPSymbol old, VPSymbol n)
{
	VPSymbol m;
	if ((m = n & vpToneMask)) return (old & (~vpToneMask)) | m;
	if ((m = n & vpVowelMask)) return (old & (~vpVowelMask)) | m;		
	if ((m = n & vpMiddleVowelMask)) return (old & (~vpMiddleVowelMask)) | m;
	if ((m = n & vpConsonantMask)) return (old & (~vpConsonantMask)) | m;
	return old;
}


extern "C" VPSymbol VPDeleteSymbolLastPart(VPSymbol s)
{
	if (s & vpToneMask) return (s & ~vpToneMask);
	if (s & vpVowelMask) return (s & ~vpVowelMask);
	if (s & vpMiddleVowelMask) return (s & ~vpMiddleVowelMask);
	return 0;
}

// HEREBY BEGINS THE FIND-CANDIDATE FUNCTION... ==========================
extern unsigned short vpList0[];
extern unsigned short vpNodeTable[];
extern unsigned short vpNodelist[];
extern unsigned short vpCharData[];

struct VPNode
{
	unsigned short id;
	unsigned short datapos;
	unsigned short datasize;
	unsigned short listpos;
	unsigned short listsize;
};

extern "C" int VPFindCandidate(VPSymbol s, VPUTF16 *data)
{
	VPNode* nodetable=(VPNode*)vpNodeTable;
	VPNode* firstlayer=(VPNode*)vpList0;

	int querysequence[5];
	int *qsp=querysequence;
	memset(querysequence, 0, 5*sizeof(int));

	// decompose a phonetic symbol into parts (query sequences)
	if (s & vpConsonantMask) *qsp++=VPSymbolToOrdinal (s&vpConsonantMask);
	if (s & vpMiddleVowelMask) *qsp++=VPSymbolToOrdinal (s&vpMiddleVowelMask);
	if (s & vpVowelMask) *qsp++=VPSymbolToOrdinal (s&vpVowelMask);
	if (s & vpToneMask) *qsp++=VPSymbolToOrdinal (s&vpToneMask);
	qsp=querysequence;
	
	int i;
	VPNode *n, *nn;

	// BINARY-SEARCH COULD BE IMPLEMENTED THROUGHOUT...
	
	if (!*qsp) return 0;
	if (*qsp > 37) return 0;
	n=&firstlayer[*qsp];
	
	/* for (i=0; i<38; i++)	// the size of firstlayer is always fixed
	{
		n=&firstlayer[i];
		if (n->id==*qsp) break;		// got it!
	}
	if (i==38) return 0;	// found nothing, returns */


	while (1)
	{
		if (!*++qsp)		// no more query sequence, copy the result
		{
			// WARNING: NO BOUNDARY CHECK, VERY ARCHITECTURE-SPECIFIC CODE
			
			for (i=0; i< n->datasize; i++) data[i]=vpCharData[n->datapos+i];
			return n->datasize;
		}
		else		// more query sequence, so have to find subnode
		{
			for (i=0; i<n->listsize; i++)
			{
				nn=&nodetable[vpNodelist[n->listpos+i]];
				if (nn->id==*qsp) break;
			}
			if (i==n->listsize) return 0;   // found nothing
			n=nn;
		}
	}
}


// text services =====================================================

extern "C" VPUTF16 VPSymbolCharUTF16(VPSymbol s)
{
	VPOrdinal o=VPSymbolToOrdinal(s);
	if (!o) return 0;
	return vpSymbolUTF16[o];
}

extern "C" int VPSymbolStringUTF16(VPSymbol s, VPUTF16 *data)
{		
	int l=0;
	if (s & vpConsonantMask)
	{
		*data++ = VPSymbolCharUTF16(s & vpConsonantMask);
		l++;
	}
	
	if (s & vpMiddleVowelMask)
	{
		*data++ = VPSymbolCharUTF16(s & vpMiddleVowelMask);
		l++;
	}
	
	if (s & vpVowelMask)
	{
		*data++ = VPSymbolCharUTF16(s & vpVowelMask);
		l++;
	}

	if (s & vpToneMask)
	{
		*data++ = VPSymbolCharUTF16(s & vpToneMask);
		l++;
	}
	return l;
}

extern "C" int VPSymbolCharBig5(VPSymbol s, char* c)
{
	VPOrdinal o=VPSymbolToOrdinal(s);
	if (!o) return 0;
	*c++=vpSymbolBig5[o*2];
	*c++=vpSymbolBig5[o*2+1];
	*c++=0;
	return 2;
}

extern "C" int VPSymbolStringBig5(VPSymbol s, VPChar *str)
{
	int l=0;
	if (s & vpConsonantMask)
	{
		VPSymbolCharBig5(s & vpConsonantMask, str);
		l+=2;
		str+=2;
	}
	
	if (s & vpMiddleVowelMask)
	{
		VPSymbolCharBig5(s & vpMiddleVowelMask, str);
		l+=2;
		str+=2;

	}
	
	if (s & vpVowelMask)
	{
		VPSymbolCharBig5(s & vpVowelMask, str);
		l+=2;
		str+=2;

	}

	if (s & vpToneMask)
	{
		VPSymbolCharBig5(s & vpToneMask, str);
		l+=2;
		str+=2;
	}
	
	return l;
}

// service functions =============================================

extern "C" int VPCheckFormation (VPSymbol s)
{
	VPSymbol i;

	if (!s) return 0;		// tone 1 = 0, but the symbol must come with a sound...
	
	i = s & vpConsonantMask;
	if (i && i > 21) return 0;		// 21 consonants
	
	// i = (s & vpMiddleVowelMask) >> 5;		// only three middle vowels, redundant step
	
	i = (s & vpVowelMask) >> 7;
	if (i && i > 13) return 0;		// 13 vowels (excluding middle-vowels)
	
	i = (s & vpToneMask) >> 11;
	if (i && i > 4) return 0;		// tone[2-5], so a total of 5 (remember tone1==0)
	
	// ignore 0xc000 (the highest 2 bits), as it's meaningless
	return 1;
}

extern "C" VPChar VPToUpper (VPChar c)
{
	// not elegant at all, but anyway it doesn't waste much CPU time...
	if (c > 0x60 && c < 0x7b) return (c-0x20);
	return (c);
}

extern "C" VPChar VPToLower (VPChar c)
{
	// not elegant at all, but anyway it doesn't waste much CPU time...
	if (c > 0x40 && c < 0x5b) return (c+0x20);
	return (c);
}

// punctuation service functions


// punctuation mark service functions ====================================

#define vpPMLeftSingleQuote		0x300c
#define vpPMRightSingleQuote	0x300d
#define vpPMLeftDoubleQuote		0x300e
#define vpPMRightDoubleQuote	0x300f
#define vpPMLeftBookMark		0x300a
#define vpPMRightBookMark		0x300b
#define vpPMShortComma			0x3001
#define vpPMComma				0xff0c
#define vpPMColon				0xff1a
#define vpPMSemiColon			0xff1b
#define vpPMPeriod				0x3002
#define vpPMWave				0xff5e
#define vpPMExclamation			0xff01
#define vpPMAt					0xff20
#define vpPMPound				0xff03
#define vpPMDollar				0xff04
#define vpPMPercent				0xff05
#define vpPMCircumflex			0xfe3f
#define vpPMAmpersend			0xff06
#define vpPMAsterisk			0xff0a
#define vpPMLeftParenthesis		0xff08
#define vpPMRightParenthesis	0xff09
#define vpPMUnderline			0xff3f
#define vpPMPlus				0xff0b
#define vpPMEqual				0xff1d
#define vpPMBackslash			0xff3c
#define vpPMPipe				0xff5c
#define vpPMQuestionMark		0xff1f
#define vpPMApostrophe			0x2019
#define vpPMFWRightSingleQuote  0x2019		// same with vpPMApostrophe, FW=full width
#define vpPMFWLeftSingleQuote   0x2018
#define vpPMFWDoubleQuote		0x201d
#define vpPMSlash				0xff0f
#define vpPMFWLeftBlockBracket  0x3014
#define vpPMFWRightBlockBracket 0x3015
#define vpPMFWLeftCurveBracket  0xff5b
#define vpPMFWRightCurveBracket 0xff5d
#define vpPMFWDash				0xff0d
#define vpPMFWNBHorizontalBar	0x2500		// NB=non-breaking
#define vpPMFWNBVerticalBar		0x2502
#define vpPMENDash				0x2013
#define vpPMEMDash				0x2014

#define vpPUNCTUATIONWINDOWKEYSIZE  35

// don't forget \0 is internally included in the string, therefore size+1...
VPChar vpPunctuationWindowKey[vpPUNCTUATIONWINDOWKEYSIZE+1]=
    "4567890-=\\RTYUIOP[]FGHJKL;\'CVBNM,./";

VPUTF16 vpPunctuationWindowCode[vpPUNCTUATIONWINDOWKEYSIZE]=
{
	0x301d,0x301e,0x2018,0x2019,0x201c,0x201d,0x300e,0x300f,0x300c,0x300d,
    0x203b,0x3008,0x3009,0x300a,0x300b,0x3010,0x3011,0xff08,0xff09,
    0x25cb,0x25cf,0x2191,0x2193,0xff01,0xff1a,0xff1b,0x3001,
    0x25ce,0x00a7,0x2190,0x2192,0x3002,0xff0c,0x2027,0xff1f
};

extern "C" VPUTF16 VPPunctuationWindowKeyToCode(VPChar c)
{
	int i;
	VPChar cc=VPToUpper(c);
	
	for (i=0; i<vpPUNCTUATIONWINDOWKEYSIZE; i++)
	{
		if (vpPunctuationWindowKey[i]==cc) return vpPunctuationWindowCode[i];
	}
	return 0;
}

extern "C" int VPFindPunctuationCandidate(VPChar c, VPUTF16 *data)
{
	int len=0;
	VPChar cc=VPToUpper(c);
	
	// hereforth we extend then punctuation functionality offered by SpaceChewing:
	// keys like ":" can now have candiates (e.g. choices between colon and semicolon)
	
	switch (cc)
	{
		case ':':
//		case ';':						// can't use this
			*data++=vpPMColon;
			*data++=vpPMSemiColon;
			len=2;
			return len;
			
		case '{':
			*data++=vpPMLeftDoubleQuote;
			*data++=vpPMLeftBookMark;
			*data++=vpPMFWLeftBlockBracket;
			*data++=vpPMFWLeftCurveBracket;
			len=4;
			return len;
		
		case '}':
			*data++=vpPMRightDoubleQuote;
			*data++=vpPMRightBookMark;
			*data++=vpPMFWRightBlockBracket;
			*data++=vpPMFWRightCurveBracket;
			len=4;
			return len;
			
		case '\"':
//		case '\'':				// can't use this, otherwise Eten would die...
			*data++=vpPMShortComma;
			*data++=vpPMFWDoubleQuote;
			*data++=vpPMApostrophe;
			*data++=vpPMFWLeftSingleQuote;
			len=4;
			return len;
			
//		case '=':			// can't use this, otherwise Eten would die...
		case '+':
			*data++=vpPMPlus;
			*data++=vpPMEqual;
			len=2;
			return len;
		
		case '\\':
			*data++=vpPMBackslash;
			*data++=vpPMSlash;
			len=2;
			return len;
			
		case '_':
			*data++=vpPMFWDash;
			*data++=vpPMUnderline;
			*data++=vpPMFWNBHorizontalBar;
			*data++=vpPMFWNBVerticalBar;
			*data++=vpPMENDash;
			*data++=vpPMEMDash;
			len=6;
			return len;
	}
	
	// otherwise, a table-lookup makes life easier...
	

	const int tablesize=18;
	// don't forget again that \0 is part of the string (added by compiler),
	// therefore size+1...
	VPChar lookuptable[tablesize+1]="[]<>"		// 4 marks
								"~!@#$%"		// 6 marks
					            "^&*()"			// 5 marks
					            "?|`";			// 3 marks
	VPUTF16 lookupcode[tablesize]=
	{
		vpPMLeftSingleQuote, vpPMRightSingleQuote, vpPMComma, vpPMPeriod,
		vpPMWave, vpPMExclamation, vpPMAt, vpPMPound, vpPMDollar, vpPMPercent,
		vpPMCircumflex, vpPMAmpersend, vpPMAsterisk, vpPMLeftParenthesis, vpPMRightParenthesis,
		vpPMQuestionMark, vpPMPipe, vpPMFWLeftSingleQuote
	};
	
	int i;
	for (i=0; i<tablesize;i++)
	{
		if (cc==lookuptable[i])
		{
			*data++=lookupcode[i];
			len++;
		}
	}
			   
	return len;
}


