// OVPhoneticLib.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "OVPhoneticLib.h"

struct OVPhoneticDataNode {
    unsigned short code;
    unsigned short candistrpos;
};

int _OVPDNCompare(const void *p, const void *q)
{
    OVPhoneticDataNode *x=(OVPhoneticDataNode*)p, *y=(OVPhoneticDataNode*)q;
    return (short)(x->code - y->code);
}

struct OVPhoneticData {
    OVPhoneticData(unsigned short *d);
    
    int maxCandidiateStringLength() { return maxcandistrlen; }
    int find(unsigned short code, unsigned short *str);

    int nodecount;
    int maxcandistrlen;
    OVPhoneticDataNode *nodes;
    unsigned short *candistr;
};

OVPhoneticData::OVPhoneticData(unsigned short *d)
{
    nodecount=d[0];
    maxcandistrlen=d[1];
    nodes=(OVPhoneticDataNode*)(d+2);
    candistr=d+2+nodecount*2;    
}

int OVPhoneticData::find(unsigned short code, unsigned short *str)
{
    OVPhoneticDataNode k={code, 0};
    OVPhoneticDataNode *f=(OVPhoneticDataNode*)bsearch
        (&k, nodes, nodecount, sizeof(OVPhoneticDataNode), _OVPDNCompare);
    
    if (!f) return 0;
    int l=(f+1)->candistrpos - f->candistrpos;
    memcpy(str, candistr + f->candistrpos, l*sizeof(unsigned short));
    return l;
}

extern unsigned short ovPhoneticData[];
OVPhoneticData ovpData(ovPhoneticData);

// VP stands for Vanilla Phonetics library, it's really from the old, old
// VanillaInput...

char VPSymbolToStandardLayoutChar(unsigned short);
const char *VPSymbolToStandardLayoutString(unsigned short);   // syllabic
const char *VPUTF16ToUTF8(unsigned short *s, int l);    // utility
unsigned short VPStandardKeyToSymbol (char);
unsigned short VPEtenKeyToSymbol (char);
unsigned short VPSymbolToOrdinal (unsigned short);      
unsigned short VPOrdinalToSymbol (unsigned short);
unsigned short VPCombineSymbol(unsigned short, unsigned short); // into syllabic
unsigned short VPDeleteSymbolLastPart(unsigned short);  // syllabic
unsigned short VPSymbolCharUTF16(unsigned short s);
int VPSymbolStringUTF16(unsigned short, unsigned short*); // syllabic
int VPCheckFormation (unsigned short);  // syllabic

OVPhoneticSyllable::OVPhoneticSyllable(int layout)
{
    clear();
    setLayout(layout);
}

int OVPhoneticSyllable::layout()
{
    return keylayout;
}

void OVPhoneticSyllable::setLayout(int layout)
{
    keylayout=layout;
}

int OVPhoneticSyllable::empty()
{
    return syllable;
}

int OVPhoneticSyllable::addKey(char c)
{
    unsigned short s= (keylayout==OVPStandardLayout) ? 
        VPStandardKeyToSymbol(c) : VPEtenKeyToSymbol(c);
    if (!s) return 0;
    return syllable=VPCombineSymbol(syllable, s);
}

int OVPhoneticSyllable::removeLast()
{
    syllable=VPDeleteSymbolLastPart(syllable);
    return empty();
}

void OVPhoneticSyllable::clear()
{
    syllable=0;
}

const char *OVPhoneticSyllable::compose()
{
    unsigned short s[16];
    int l=VPSymbolStringUTF16(syllable, s);
    return VPUTF16ToUTF8(s, l);   
}

const char *OVPhoneticSyllable::standardLayoutCode()
{
    return VPSymbolToStandardLayoutString(syllable);
}


OVPCandidate::OVPCandidate()
{
    count=0;
    candidates=NULL;
}

OVPCandidate::~OVPCandidate()
{
    if (!count) return;
    for (int i=0; i<count; i++) delete candidates[i];
    delete[] candidates;
}

OVPCandidate *OVPFindCandidate(OVPhoneticSyllable *syl)
{
    unsigned short *s=new unsigned short[ovpData.maxCandidiateStringLength()];
    int l=ovpData.find(syl->syllable, s);
    int c=0, i;
    
    // scan the candidate string
    for (i=0; i<l; i++)
    {
        if (s[i] >= 0xd800 && s[i] <= 0xdbff) i++;
        c++;
    }
    
    OVPCandidate *rc=new OVPCandidate;
    if (!rc) return NULL;
    rc->count=c;
    rc->candidates=new char* [c];
    
    c=0;
    for (i=0; i<l; i++)
    {
        const char *p;
        if (s[i] < 0xd800 || s[i] > 0xdbff)
        {
            p=VPUTF16ToUTF8(s+i, 1);
        }
        else
        {
            p=VPUTF16ToUTF8(s+i, 2);
            i++;
        }
        rc->candidates[c]=new char [strlen(p)+1];
        strcpy(rc->candidates[c++], p);
    }
    
    return rc;
}

/*
int main()
{
    OVPhoneticSyllable s(OVPEtenLayout);
    
    while(!feof(stdin))
    {
        char c=getchar();
        if (c==10 || c==32)
        {
            printf ("syllable=%s\n", s.compose());
            printf ("standard layout code=%s\n", s.standardLayoutCode());
            OVPCandidate *cc=OVPFindCandidate(&s);
            printf ("%d candidates found\n", cc->count);
            for (int i=0; i<cc->count; i++)
                printf ("%s", cc->candidates[i]);
            printf ("\n\n");
            
            s.clear();
            delete cc;
        }
        else s.addKey(c);
    }
}
*/

// constants
enum
{
    vpConsonantMask=0x001f,		// 0000 0000 0001 1111, 21 consonants
    vpMiddleVowelMask=0x0060,   // 0000 0000 0110 0000, 3 middle vowels
    vpVowelMask=0x0780,		    // 0000 0111 1000 0000, 13 vowels
    vpToneMask=0x3800,		    // 0011 1000 0000 0000, 5 tones (tone1=0x00)
    vpB=0x0001, vpP=0x0002, vpM=0x0003, vpF=0x0004,
    vpD=0x0005, vpT=0x0006, vpN=0x0007, vpL=0x0008,
    vpG=0x0009, vpK=0x000a, vpH=0x000b, 
    vpJ=0x000c, vpQ=0x000d, vpX=0x000e,
    vpZH=0x000f, vpCH=0x0010, vpSH=0x0011, vpR=0x0012,
    vpZ=0x0013, vpC=0x0014, vpS=0x0015,
    vpI=0x0020, vpU=0x0040, vpV=0x0060,
    vpA=0x0080, vpO=0x0100, vpER=0x0180, vpE=0x0200,
    vpAI=0x0280, vpEI=0x0300, vpAO=0x0380, vpOU=0x0400,
    vpAN=0x0480, vpEN=0x0500, vpANG=0x0580, vpENG=0x0600,
    vpERR=0x0680,
    vpTone1=0x0000, vpTone2=0x0800, vpTone3=0x1000, vpTone4=0x1800, vpTone5=0x2000
};

unsigned short vpStandardKeyTable[97]=
{
   0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,     // 0x20
// '!',  '"',  '#',  '$',  '%',  '&',  ''',  '(',
   0,    0,    0,    0,    0,    0,    0,    0,
// ')',  '*',  '+',  ',',  '-',  '.',  '/',  '0',   
   0,    0,    0,    vpE,  vpERR,vpOU, vpENG,vpAN,  
// '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',
   vpB,  vpD,  vpTone3,vpTone4,vpZH,vpTone2,vpTone5,vpA,
// '9',  ':',  ';',  '<',  '=',  '>',  '?',  '@',  
   vpAI, 0,    vpANG,0,    0,    0,    0,    0,
// 'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',
   vpM,  vpR,  vpH,  vpK,  vpG,  vpQ,  vpSH, vpC,
// 'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',
   vpO,  vpU,  vpER, vpAO, vpV,  vpS,  vpEI, vpEN,
// 'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  
   vpP,  vpJ,  vpN,  vpCH, vpI,  vpX,  vpT,  vpL,
// 'Y',  'Z',  '[',  '\',  ']',  '^',  '_',  '`'
   vpZ,  vpF,  0,    0,    0,    0,    0,    0
};

unsigned short vpEtenKeyTable[97]=
{
   0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,
   0,    0,    0,    0,    0,    0,    0,    0,     // 0x20 
// '!',  '"',  '#',  '$',  '%',  '&',  ''',  '(',
   0,    0,    0,    0,    0,    0,    vpC,  0,
// ')',  '*',  '+',  ',',  '-',  '.',  '/',  '0',
   0,    0,    0,    vpZH, vpENG,vpCH, vpSH, vpANG,
// '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',
   vpTone5,vpTone2,vpTone3,vpTone4,0,0,vpQ,  vpAN,
// '9',  ':',  ';',  '<',  '=',  '>',  '?',  '@',  
   vpEN, 0,    vpZ,  0,    vpERR,0,    0,    0,
// 'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',
   vpA,  vpB,  vpX,  vpD,  vpI,  vpF,  vpJ,  vpH,
// 'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',
   vpAI, vpR,  vpK,  vpL,  vpM,  vpN,  vpO,  vpP,
// 'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  
   vpEI, vpER, vpS,  vpT,  vpV,  vpG,  vpE,  vpU,
// 'Y',  'Z',  '[',  '\',  ']',  '^',  '_',  '`'
   vpOU, vpAO, 0,    0,    0,    0,    0,    0
};

unsigned short vpSymbolUTF16[42]=
{
    0,     12549, 12550, 12551, 12552, 12553, 12554, 12555, 12556, 12557,
    12558, 12559, 12560, 12561, 12562, 12563, 12564, 12565, 12566, 12567,
    12568, 12569, 12583, 12584, 12585, 12570, 12571, 12572, 12573, 12574,
    12575, 12576, 12577, 12578, 12579, 12580, 12581, 12582, 714,   711,   
    715,   729 
};

const char *vpStandardLayoutCode=" 1qaz2wsxedcrfv5tgbyhnujm8ik,9ol.0p;/-6347";

char vpComposeBuffer[32];

char VPSymbolToStandardLayoutChar(unsigned short s)
{
	unsigned short o=VPSymbolToOrdinal(o);
	if (!o) return 0;
    return vpStandardLayoutCode[o];
}

const char *VPSymbolToStandardLayoutString(unsigned short s)
{
    char *b=vpComposeBuffer;
	if (s & vpConsonantMask) 
	   *b++ = VPSymbolToStandardLayoutChar(VPSymbolToOrdinal(s&vpConsonantMask));
	if (s & vpMiddleVowelMask) 
	   *b++ = VPSymbolToStandardLayoutChar(VPSymbolToOrdinal(s&vpMiddleVowelMask));
	if (s & vpVowelMask)
	   *b++ = VPSymbolToStandardLayoutChar(VPSymbolToOrdinal(s&vpVowelMask));
	if (s & vpToneMask)
	   *b++ = VPSymbolToStandardLayoutChar(VPSymbolToOrdinal(s&vpToneMask));
	*b=0;
    return vpComposeBuffer;
}

const char *VPUTF16ToUTF8(unsigned short *s, int l)
{
    char *b=vpComposeBuffer;
    for (int i=0; i<l; i++)
    {
        if (s[i] < 0x80)
        {
            *b++=s[i];
        }
        else if (s[i] < 0x800)
        {
            *b++=(0xc0 | s[i]>>6);
            *b++=(0x80 | s[i] & 0x3f);
        }
        else if (s[i] < 0xd800 || s[i] > 0xdbff)
        {
            *b++ = (0xe0 | s[i]>>12);
            *b++ = (0x80 | s[i]>>6 & 0x3f);
            *b++ = (0x80 | s[i] & 0x3f);

        }
        else
        {
            unsigned int offset= 0x10000 - (0xd800 << 10) - 0xdc00;
            unsigned int codepoint=(s[i] << 10) + s[i+1]+offset;
            i++;
            *b++=(0xf0 | codepoint>>18);
            *b++=(0x80 | codepoint>>12 & 0x3f);
            *b++=(0x80 | codepoint>>6 & 0x3f);
            *b++=(0x80 | codepoint & 0x3F);
        }
    }
    
    *b=0;
    return vpComposeBuffer;
}

unsigned short VPStandardKeyToSymbol (char c)
{
	char cc=toupper(c);
	if (cc > 96) return 0;
	return vpStandardKeyTable[cc];
}

unsigned short VPEtenKeyToSymbol (char c)
{
	char cc=toupper(c);
	if (cc > 96) return 0;
	return vpEtenKeyTable[cc];
}

// persumes that ONLY ONE SYMBOL is converted AND there ARE NO OTHER MASKED SYMBOLS
unsigned short VPSymbolToOrdinal(unsigned short s)
{
	if (!VPCheckFormation(s)) return 0;
	
	unsigned short ss;
	
	if ((ss = (s & vpConsonantMask))) return ss;
	if ((ss = (s & vpMiddleVowelMask))) return (ss >> 5)+21;
	if ((ss = (s & vpVowelMask))) return (ss >> 7)+24;
	if ((ss = (s & vpToneMask))) return (ss >> 11)+37;
	return 0;
}

unsigned short VPOrdinalToSymbol(unsigned short o)
{
	if (o < 1 || o > 41) return 0;  // malformed ordinal
	if (o < 22) return (unsigned short)o;			// 1~21 == consonants
	if (o < 25) return (unsigned short)((o-21) << 5);	// 22~24 == middle vowels
	if (o < 38) return (unsigned short)((o-24) << 7);	// 25~37 == vowels
	return (unsigned short)((o-37) << 11);				// 38~41 == vpTone[2-5]
}


unsigned short VPCombineSymbol(unsigned short old, unsigned short n)
{
	unsigned short m;
	if ((m = n & vpToneMask)) return (old & (~vpToneMask)) | m;
	if ((m = n & vpVowelMask)) return (old & (~vpVowelMask)) | m;		
	if ((m = n & vpMiddleVowelMask)) return (old & (~vpMiddleVowelMask)) | m;
	if ((m = n & vpConsonantMask)) return (old & (~vpConsonantMask)) | m;
	return old;
}

unsigned short VPDeleteSymbolLastPart(unsigned short s)
{
	if (s & vpToneMask) return (s & ~vpToneMask);
	if (s & vpVowelMask) return (s & ~vpVowelMask);
	if (s & vpMiddleVowelMask) return (s & ~vpMiddleVowelMask);
	return 0;
}

unsigned short VPSymbolCharUTF16(unsigned short s)
{
	unsigned short o=VPSymbolToOrdinal(s);
	if (!o) return 0;
	return vpSymbolUTF16[o];
}

int VPSymbolStringUTF16(unsigned short s, unsigned short *data)
{		
	int l=0;
	if (s & vpConsonantMask) 
	   { *data++ = VPSymbolCharUTF16(s & vpConsonantMask); l++; }
	if (s & vpMiddleVowelMask)
	   { *data++ = VPSymbolCharUTF16(s & vpMiddleVowelMask); l++; }
	if (s & vpVowelMask)
	   { *data++ = VPSymbolCharUTF16(s & vpVowelMask); l++; }
	if (s & vpToneMask)
	   { *data++ = VPSymbolCharUTF16(s & vpToneMask); l++; }
	return l;
}

int VPCheckFormation (unsigned short s)
{
	unsigned short i;
	if (!s) return 0;		// tone 1 = 0, but the symbol must come with a sound...
	i = s & vpConsonantMask;
	if (i && i > 21) return 0;		// 21 consonants
	// i = (s & vpMiddleVowelMask) >> 5; // only three middle vowels, redundant step
	i = (s & vpVowelMask) >> 7;
	if (i && i > 13) return 0;		// 13 vowels (excluding middle-vowels)
	i = (s & vpToneMask) >> 11;
	if (i && i > 4) return 0;	// tone[2-5], so a total of 5 (remember tone1==0)
	// ignore 0xc000 (the highest 2 bits), as it's meaningless
	return 1;
}

