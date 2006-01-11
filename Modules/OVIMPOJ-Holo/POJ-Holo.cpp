// POJ-Holo.cpp: Implementation of POJ-Holo's key sequence
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "POJ-Holo.h"

const char *pojHoloToneComposeKey=" 12345678";
const char *pojHoloVowel="aeimnoquAEIMNOQU";
const char *pojHoloNasel="\xe2\x81\xbf";
const char *pojHoloToneTable[]= {
    "a", "\xc3\xa1", "\xc3\xa0", "a", "\xc3\xa2", "\xc3\xa1", "\xc4\x81", "a\xcc\x8d",
    "e", "\xc3\xa9", "\xc3\xa8", "e", "\xc3\xaa", "\xc3\xa9", "\xc4\x93", "e\xcc\x8d",
    "i", "\xc3\xad", "\xc3\xac", "i", "\xc3\xae", "\xc3\xad", "\xc4\xab", "i\xcc\x8d",
    "m", "\xe1\xb8\xbf", "m\xcc\x80", "m", "m\xcc\x82", "\xe1\xb8\xbf", "m\xcc\x84", "m\xcc\x8d",
    "n", "\xc5\x84", "\xc7\xb9", "n", "n\xcc\x82", "\xc5\x84", "n\xcc\x84", "n\xcc\x8d",
    "o", "\xc3\xb3", "\xc3\xb2", "o", "\xc3\xb4", "\xc3\xb3", "\xc5\x8d", "o\xcc\x8d",
    "o\xc2\xb7","\xc3\xb3\xc2\xb7","\xc3\xb2\xc2\xb7","o\xc2\xb7","\xc3\xb4\xc2\xb7","o\xc2\xb7","\xc5\x8d\xc2\xb7","o\xcc\x8d\xc2\xb7",
    "u", "\xc3\xba", "\xc3\xb9", "u", "\xc3\xbb", "\xc3\xba", "\xc5\xab", "u\xcc\x8d",
    "A", "\xc3\x81", "\xc3\x80", "a", "\xc3\x82", "\xc3\x81", "\xc4\x80", "A\xcc\x8d",
    "E", "\xc3\x89", "\xc3\x88", "E", "\xc3\x8a", "\xc3\x89", "\xc4\x92", "E\xcc\x8d",
    "I", "\xc3\x8d", "\xc3\x8c", "I", "\xc3\x8e", "\xc3\x8d", "\xc4\xaa", "I\xcc\x8d",
    "M", "\xe1\xb8\xbe", "M\xcc\x80", "M", "M\xcc\x82", "\xe1\xb8\xbe", "M\xcc\x84", "M\xcc\x8d",
    "N", "\xc5\x83", "\xc7\xb8", "N", "N\xcc\x82", "\xc5\x83", "N\xcc\x84", "N\xcc\x8d",
    "O", "\xc3\x93", "\xc3\x92", "O", "\xc3\x94", "\xc3\x93", "\xc5\x8c", "O\xcc\x8d",
    "O\xc2\xb7","\xc3\x93\xc2\xb7","\xc3\x92\xc2\xb7","O\xc2\xb7","\xc3\x94\xc2\xb7","\xc3\x93\xc2\xb7","\xc5\x8c\xc2\xb7","O\xcc\x8d\xc2\xb7",
    "U", "\xc3\x9a", "\xc3\x99", "U", "\xc3\x9b", "\xc3\x9a", "\xc5\xaa", "U\xcc\x8d"
};

// vcomposed state
enum {
    POJ_Holo_VowelNone=0,
	POJ_Holo_VowelMN=1,
	POJ_Holo_VowelAEIOU=2
};

POJHoloKeySequence::POJHoloKeySequence() {
    clear();
}

bool POJHoloKeySequence::isEmpty() {
    if (len) return false;
    return true;
}

void POJHoloKeySequence::clear() {
    len=0;
    prebindwait=0;
    presettone=0;
    bindvowel=0;
    memset(seq, 0, POJ_Holo_MaxSeqLen);
    memset(composebuf, 0, POJ_Holo_MaxBufLen);
}

bool POJHoloKeySequence::isComposeKey(char c) {
    if (strchr(pojHoloToneComposeKey, c)) return true;
    return false;
}

bool POJHoloKeySequence::add(char c, int layout) {   
    // if the sequence buffer is already full
    if (len >= POJ_Holo_MaxSeqLen) return false;
    
    // ignore space and non-print characters
    if (c==' ' || !isprint(c)) return true;
    
    // determine if it's tone
    if (c >= '1' && c <= '8') {
        presettone=c-'0';
        return true;
    }
    
    // if a prebinding tone mark is waiting for the pojHoloVowel to come in
    // if it's pojHoloVowel, set presettone; after that, prebindwait=0
    // (so if the c is not pojHoloVowel, the waiting state is canceled anyway)
    if (prebindwait) {
        if (vowelorder(c) != -1)  {
            presettone=prebindwait;
            remove();                   // remove the waiting tone mark
            bindvowel=len+1;     // as we're going to add one char
        }
        prebindwait=0;      
    }

    int tmptone=toneMark(c);
    if (tmptone) {
        // ignore pojToneByNumber; we don't allow tone marks to be used in
        // that keyboard layout
        if (prebindwait) return false;
        if (presettone) return false;
        if (layout==POJ_Holo_ToneByPreceedingSymbol)
            prebindwait=tmptone;
        else if (layout==POJ_Holo_ToneBySucceedingSymbol) {
            // check if the preceding char is a pojHoloVowel
            if (len) 
                if (vowelorder(seq[len-1])!=-1) 
                {
                    presettone=tmptone;
                    bindvowel=len;
                }
            return true;
        }
    }
            
    // repleace "q" to "ou"
    if (c=='q') { if (add('o')) return add('u'); else return false; }
    if (c=='Q') { if (add('O')) return add('O'); else return false; }
    
    seq[len++]=c;
    seq[len]=0;
    return true;
}

void POJHoloKeySequence::remove() {
    if (!len) return;
    
    // "ou" rule
    if (tolower(seq[len-1])=='u') {
        if (len>1 && tolower(seq[len-2])=='o') {
            len-=2;
            seq[len]=0;
            if (bindvowel && len<bindvowel) {
                bindvowel=0;
                presettone=0;
            }
            return;
        }
    }
    
    seq[--len]=0;
    if (bindvowel && len<bindvowel) {
        bindvowel=0;
        presettone=0;
    }
}

void POJHoloKeySequence::normalize()
{
    int t=presettone;
    presettone=0;
    
    if (!t) return;
    if (!len) return;

    // if already finalized, we remove the finalized mark
    if (seq[len-1] >= '1' && seq[len-1] <= '8') seq[--len]=0;  
    
    // there is no tone 6 in Holo, tone 1 needs no extra mark,
    // tone 4 is not necessary (if syllable ends in h/k/p/t and tone!=8,
    // it's tone 4)
    if (t==1 || t==6 || t==4) return;
    
    // only tone 8 is possible for syllable ending in h, k, p, t
    char c=tolower(seq[len-1]);
    int hkpt=(c=='h' || c=='k' || c=='p' || c=='t');
    if (hkpt && t!=8) return;
    if (!hkpt && t==8) return;
    
    presettone=t;
    return;
}

const char *POJHoloKeySequence::sequence() {
    return seq;
}

const char *POJHoloKeySequence::finalize() {
    // if it already exceeds maximum length, we can't append tone
    if (len == POJ_Holo_MaxSeqLen) return seq; 
    if (!len) return seq;
    normalize();
    
    if (presettone) {
        seq[len++]=presettone+'0';
        seq[len]=0;
    }
    
    return seq;
}

const char *POJHoloKeySequence::compose(bool pureascii)
{
    strcpy(composebuf, "");
    if (!len) return composebuf;

    char *b;    
    // if asciioutput is on, directly copy the internal representation
    // presumes normailization
    if (pureascii) {
        b=composebuf;
        strcpy(composebuf, seq);
        while (*b++) ;
        if (presettone) *b++ = presettone+'0';
        *b=0;
        return composebuf;
    }
    
    int tone=1;    
    b=composebuf;

    if (presettone) tone=presettone;

    char c, *s=seq;
    int vcomposed=0;        // if the first pojHoloVowel is composed
    
    while((c=*s))   {       // C style, very C style
        char cnext=*(s+1);

        if (isComposeKey(c)) break; // if we encountering tone mark, the end
        
        int vo=vowelorder(c);

        // pojHoloNasel? ("nn") -- note that this rule should never apply
        // when "nn" appears in the beginning of the sequence
        if (tolower(c)=='n' && tolower(cnext)=='n' && tolower(*(s+2))!='g') {
            strcat(b, pojHoloNasel);
            b+=strlen(pojHoloNasel);
            s+=2;
            continue;
        }

#define IRULE ( tolower(c) == 'i' && (vowelorder(cnext) != -1 ) && (tolower(cnext) != 'n' ) && (tolower(cnext) != 'm' ) )

        // If previously we have M or N, and now we have another
        // pojHoloVowel right here, we should discard tone on M,N.
        // (That means re-copy all leading characters.)
        if (vcomposed == POJ_Holo_VowelMN && ( vo != -1 ) && !IRULE ) {
        	char *t=seq;
        	b=composebuf;
        	while(t<s) *b++=*t++;
        	*b=0;
        	vcomposed = POJ_Holo_VowelNone;
        }

        // if pojHoloVowel already composed, or not pojHoloVowel,
        // here we also apply "i" rule (if "i" is followed by a pojHoloVowel,
        // the tone mark will never be placed upon "i")
        if (vcomposed || ( vo == -1 ) || IRULE ) {
            *b++=*s++;
            *b=0;   // to supress junk
            continue;
        }
#undef IRULE

        // "ou" and "OU" rule
        if (c=='o' && tolower(cnext)=='u') {
            c='q';
            s++;    // shift one char
        }
        
        if (c=='O' && tolower(cnext)=='u') {
            c='Q';
            s++;    // shift one char
        }
    
        char *vstr=(char*)vowel2tone(c, tone);
        if (vstr) {
            // compose the tone mark
            strcat(b, vstr);

            b+=strlen(vstr);
            vcomposed = (tolower(c) == 'm' || tolower(c) == 'n') ? POJ_Holo_VowelMN : POJ_Holo_VowelAEIOU;
        }
        s++;
    }
    *b=0;
    return composebuf;
}

int POJHoloKeySequence::vowelorder(char c)
{
    const char *v=pojHoloVowel;
    for (int i=0; *v; i++, v++) if (c==*v) return i;
    return -1;
}

// tone must be between 1-8
const char *POJHoloKeySequence::vowel2tone(char c, int tone)  {
    if (tone < 1 || tone > 8) return NULL;
    int o=vowelorder(c);
    if (o==-1) return NULL;
    return pojHoloToneTable[o*8+(tone-1)];
}

int POJHoloKeySequence::toneMark(char c)
{
    switch (c) {
        case '\'': return 2;
        case '`': return 3;
        case '^': return 5;
        case '=': return 7;
        case '|': return 8;
    }
    return 0;
}
