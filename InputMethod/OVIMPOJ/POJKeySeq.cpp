// POJKeySeq.cpp

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>
#include <stdlib.h>
#include <string.h>
#include "POJKeySeq.h"

const char *toneComposeKey=" 12345678";
const char *vowel="aeimnoquAEIMNOQU";
const char *nasel="ⁿ";
const char *tonetable[]=
{
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
    pojVowelNONE=0,
	pojVowelMN=1,
	pojVowelAEIOU=2
};

POJKeySeq::POJKeySeq()
{
    clear();
}

int POJKeySeq::isCompose(char c)
{
    if (strchr(toneComposeKey, c)) return 1;
    return 0;
}

int POJKeySeq::add(char c, int layout)
{   
    if (len == pojMaxSeqLen) return 0;
    
    if (c==32) return 1;
    
    if (c >= '1' && c <= '8')
    {
        presettone=c-'0';
        return 1;
    }
    
    // if a prebinding tone mark is waiting for the vowel to come in
    // if it's vowel, set presettone; after that, prebindwait=0
    // (so if the c is not vowel, the waiting state is canceled anyway)
    if (prebindwait)
    {
        if (vowelorder(c) != -1) 
        {
            presettone=prebindwait;
            remove();       // remove the waiting tone mark
            bindvowel=len+1;    // as we're going to add one char
        }
        
        prebindwait=0;      
    }

    int tmptone=tonemark(c);
    if (tmptone)
    {
        // ignore pojToneByNumber; we don't allow tone marks to be used in
        // that keyboard layout
        if (prebindwait) return 0;
        if (presettone) return 0;
        if (layout==pojTonePrebind) prebindwait=tmptone;
        else if (layout==pojTonePostbind)
        {
            // check if the preceding char is a vowel
            if (len) 
                if (vowelorder(seq[len-1])!=-1) 
                {
                    presettone=tmptone;
                    bindvowel=len;
                }
            return 1;
        }
    }
            
    // repleace "q" to "ou"
    if (c=='q') { if (add('o')) return add('u'); else return 0; }
    if (c=='Q') { if (add('O')) return add('O'); else return 0; }
    
    seq[len++]=c;
    seq[len]=0;
    return 1;
}

void POJKeySeq::remove()
{
    if (!len) return;
    
    // "ou" rule
    if (tolower(seq[len-1])=='u')
    {
        if (len>1 && tolower(seq[len-2])=='o')
        {
            len-=2;
            seq[len]=0;
            if (bindvowel && len<bindvowel)
            {
                bindvowel=0;
                presettone=0;
            }
            return;
        }
    }
    
    seq[--len]=0;
    if (bindvowel && len<bindvowel)
    {
        bindvowel=0;
        presettone=0;
    }
}

void POJKeySeq::clear()
{
    len=0;
    prebindwait=0;
    presettone=0;
    bindvowel=0;
    seq[0]=0;
}

char *POJKeySeq::normalize()
{
    int t=presettone;
    presettone=0;
    
    if (!t) return seq;
    if (!len) return seq;
    
    // there is no tone 6 in Holo, tone 1 needs no extra mark,
    // tone 4 is not necessary (if syllable ends in h/k/p/t and tone!=8,
    // it's tone 4)
    if (t==1 || t==6 || t==4) return seq;
    
    // only tone 8 is possible for syllable ending in h, k, p, t
    char c=tolower(seq[len-1]);
    int hkpt=(c=='h' || c=='k' || c=='p' || c=='t');
    if (hkpt && t!=8) return seq;
    if (!hkpt && t==8) return seq;
    
    presettone=t;
    return seq;
}

char *POJKeySeq::finalize()
{
    if (len == pojMaxSeqLen) return 0;    
    normalize();
    if (presettone)
    {
        seq[len++]=presettone+'0';
        seq[len]=0;
    }
    
    return seq;
}

char *POJKeySeq::compose(char *buf, int asciioutput)
{
    murmur ("composing syllable, internal representation=%s", seq);
    
    *buf=0;
    
    // if asciioutput is on, directly copy the internal representation
    // presumes normailization
    if (asciioutput) 
    {
        char *b=stpcpy(buf, seq);
        if (!strlen(buf)) return buf;
        if (presettone) *b++ = presettone+'0';
        *b=0;
        return buf;
    }
    
    int tone=1;    
    char *b=buf;

    if (!len)
    {
        *buf=0;
        return buf;
    }

//      if (seq[len-1] >= '1' && seq[len-1] <= '8') tone=seq[len-1]-'0';
    if (presettone) tone=presettone;

    char c, *s=seq;
    int vcomposed=0;        // if the first vowel is composed
    
    while((c=*s))        // C style, very C style
    {
        char cnext=*(s+1);

        if (isCompose(c)) break;    // if we encountering tone mark, the end
        
        int vo=vowelorder(c);

        // nasel? ("nn") -- note that this rule should never apply
        // when "nn" appears in the beginning of the sequence
        if (tolower(c)=='n' && tolower(cnext)=='n' && tolower(*(s+2))!='g')
        {
            strcat(b, nasel);
            b+=strlen(nasel);
            s+=2;
            continue;
        }

#define IRULE ( tolower(c) == 'i' && (vowelorder(cnext) != -1 ) && (tolower(cnext) != 'n' ) && (tolower(cnext) != 'm' ) )

			// If previously we have M or N, and now we have another
			// vowel right here, we should discard tone on M,N.
			// (That means re-copy all leading characters.)
        if (vcomposed == pojVowelMN && ( vo != -1 ) && !IRULE )
        {
            murmur("VowelMNRule: char: %c, tone: %d", *s, tone);
        	char *t=seq;
        	b=buf;
        	while(t<s) {*b++=*t++;}
        	*b=0;  // to supress murmur dumping junk on this
        	murmur("VowelMNRule: seq: %s, buf: %s",seq,buf);
        	vcomposed = pojVowelNONE;
        	// *b=0;
        }

        // if vowel already composed, or not vowel,
        // here we also apply "i" rule (if "i" is followed by a vowel,
        // the tone mark will never be placed upon "i")
        if (vcomposed || ( vo == -1 ) || IRULE )
        {
            *b++=*s++;
            *b=0;   // to supress junk
            continue;
        }
#undef IRULE

        // "ou" and "OU" rule
        if (c=='o' && tolower(cnext)=='u')
        {
            c='q';
            s++;    // shift one char
        }
        
        if (c=='O' && tolower(cnext)=='u')
        {
            c='Q';
            s++;    // shift one char
        }
    
        char *vstr=(char*)vowel2tone(c, tone);
        if (vstr)
        {
            // compose the tone mark
            murmur("char: %c, tone: %d, vowel2tone: %s (strlen=%d)", *s, tone, vstr, strlen(vstr));
            
            murmur("before buf=%s, b=%s, len=%d", buf, b, strlen(b));
            strcat(b, vstr);
            murmur("after buf=%s, b=%s, len=%d", buf, b, strlen(b));

            b+=strlen(vstr);
            vcomposed =
            	(tolower(c) == 'm' || tolower(c) == 'n') ? pojVowelMN : pojVowelAEIOU;
        }
        s++;
    }
    *b=0;
    return buf;
}

int vowelorder(char c)
{
    const char *v=vowel;
    for (int i=0; *v; i++, v++) if (c==*v) return i;
    return -1;
}

const char *vowel2tone(char c, int tone)  // tone must be between 1-8
{
    if (tone < 1 || tone > 8) return NULL;
    int o=vowelorder(c);
    if (o==-1) return NULL;
    return tonetable[o*8+(tone-1)];
}

int tonemark(char c)
{
    switch (c)
    {
        case '\'': return 2;
        case '`': return 3;
        case '^': return 5;
        case '=': return 7;
        case '|': return 8;
    }
    return 0;
}

