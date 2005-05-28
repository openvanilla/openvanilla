// POJ-Holo.cpp: Implementation of POJ-Holo's key sequence

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "POJ-Holo.h"

const char *pojHoloToneComposeKey=" 12345678";
const char *pojHoloVowel="aeimnoquAEIMNOQU";
const char *pojHoloNasel="ⁿ";
const char *pojHoloToneTable[]= {
    "a", "á", "à", "a", "â", "á", "ā", "a̍",
    "e", "é", "è", "e", "ê", "é", "ē", "e̍",
    "i", "í", "ì", "i", "î", "í", "ī", "i̍",
    "m", "ḿ", "m̀", "m", "m̂", "ḿ", "m̄", "m̍",
    "n", "ń", "ǹ", "n", "n̂", "ń", "n̄", "n̍",
    "o", "ó", "ò", "o", "ô", "ó", "ō", "o̍",
    "o·","ó·","ò·","o·","ô·","o·","ō·","o̍·",
    "u", "ú", "ù", "u", "û", "ú", "ū", "u̍",
    "A", "Á", "À", "a", "Â", "Á", "Ā", "A̍",
    "E", "É", "È", "E", "Ê", "É", "Ē", "E̍",
    "I", "Í", "Ì", "I", "Î", "Í", "Ī", "I̍",
    "M", "Ḿ", "M̀", "M", "M̂", "Ḿ", "M̄", "M̍",
    "N", "Ń", "Ǹ", "N", "N̂", "Ń", "N̄", "N̍",
    "O", "Ó", "Ò", "O", "Ô", "Ó", "Ō", "O̍",
    "O·","Ó·","Ò·","O·","Ô·","Ó·","Ō·","O̍·",
    "U", "Ú", "Ù", "U", "Û", "Ú", "Ū", "U̍"
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
    bzero(seq, POJ_Holo_MaxSeqLen);
    bzero(composebuf, POJ_Holo_MaxBufLen);
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
        b=stpcpy(composebuf, seq);
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
