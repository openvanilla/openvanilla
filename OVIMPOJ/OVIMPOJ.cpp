// OVIMPOJ.cpp: prototype for POJ IM

// Copyright (c) 2004 by Lukhnos D. Liu, Kang-ming Liu, and Pek-tiong Tan

#define OVDEBUG 1

#include <OpenVanilla/OpenVanilla.h>
#include "OpenVanilla/OVLoadable.h"
#include "OpenVanilla/OVUtility.h"

char *vowel="aeimnoquAEIMNOQU";

char *nasel="ⁿ";

char *tonetable[]=
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

int vowelorder(char c)
{
    char *v=vowel;
    for (int i=0; *v; i++, v++) if (c==*v) return i;
    return -1;
}

char *vowel2tone(char c, int tone)  // tone must be between 1-8
{
    if (tone < 1 || tone > 8) return NULL;
    int o=vowelorder(c);
    if (o==-1) return NULL;
    return tonetable[o*8+(tone-1)];
}

#define pojMaxSeqLen 10

// vcomposed state
enum {
    pojVowelNONE=0,
	pojVowelMN=1,
	pojVowelAEIOU=2
};

class POJKeySeq
{
public:
    POJKeySeq() { clear(); }

    int isCompose(char c)
    {
        if (strchr("12345678", c)) return 1;
        return 0;
    }
    
    int add(char c)
    {   
        if (len == pojMaxSeqLen) return 0;
        
        // repleace "q" to "ou"
        if (c=='q') { if (add('o')) return add('u'); else return 0; }
        if (c=='Q') { if (add('O')) return add('O'); else return 0; }
        
        seq[len++]=c;
        seq[len]=0;
        return 1;
    }
    
    void remove()
    {
        if (!len) return;
        
        // "ou" rule
        if (tolower(seq[len-1])=='u')
        {
            if (len>1 && tolower(seq[len-2])=='o')
            {
                len-=2;
                seq[len]=0;
                return;
            }
        }
        
        seq[--len]=0;
    }

    void clear()
    {
        len=0;
        seq[0]=0;
    }
    
    int length()
    {
        return len;
    }
    
    char *keybuf()
    {
        return seq;
    }
    
    char *compose(char *buf, int asciioutput=0)
    {
        murmur ("composing syllable, internal representation=%s\n", seq);
        
        *buf=0;
        
        // if asciioutput is on, directly copy the internal representation
        if (asciioutput) return strcpy(buf, seq);
        
        int tone=1;    
        char *b=buf;

        if (!len)
        {
            *buf=0;
            return buf;
        }

        if (seq[len-1] >= '1' && seq[len-1] <= '8') tone=seq[len-1]-'0';

        char c, *s=seq;
        int vcomposed=0;        // if the first vowel is composed
        
        while (c=*s)        // C style, very C style
        {
            char cnext=*(s+1);

            if (isCompose(c)) break;    // if we encountering tone mark, the end
            
            int vo=vowelorder(c);

            // nasel? ("nn")
            if (tolower(c)=='n' && tolower(cnext)=='n')
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
            	murmur("VowelMNRule: seq: %s, buf: %s",seq,buf);
            	vcomposed = pojVowelNONE;
            	*b=0;
            }

            // if vowel already composed, or not vowel,
            // here we also apply "i" rule (if "i" is followed by a vowel,
            // the tone mark will never be placed upon "i")
            if (vcomposed || ( vo == -1 ) || IRULE )
            {
                *b++=*s++;
                continue;
            }
#undef IRULE

            // "ou" and "OU" rule
            if (c=='o' && cnext=='u')
            {
                c='q';
                s++;    // shift one char
            }
            
            if (c=='O' && cnext=='U')
            {
                c='Q';
                s++;    // shift one char
            }
        
            char *vstr=vowel2tone(c, tone);
            if (vstr)
            {
                // compose the tone mark
                murmur("char: %c, tone: %d, vowel2tone: %s", *s, tone, vstr);
                strcat(b, vstr);
                b+=strlen(vstr);
                vcomposed =
                	(tolower(c) == 'm' || tolower(c) == 'n') ? pojVowelMN : pojVowelAEIOU;
            }
            s++;
        }
        *b=0;
        return buf;
    }

protected:
    int len;
    char seq[pojMaxSeqLen+1];
};

class OVIMPOJ;

class OVIMPOJContext : public OVIMContext
{
public:
    OVIMPOJContext(OVIMPOJ *p) : parent(p) {}
    virtual int activate(OVService*) { return 1; }
    virtual int deactivate(OVService*) { return 1; }
    virtual int clear() { return 1; }
    
    // as this method uses a parent's method--which is not yet declared,
    // we have to move the implementation of this method towards the end
    // of this .cpp file
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, 
        OVService *srv);
    
protected:
    OVIMPOJ *parent;
    POJKeySeq seq;
};

class OVIMPOJ : public OVInputMethod
{
public:
    OVIMPOJ()
    {
        asciioutput=0;
    }
    
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVIMPOJ"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        return strlen(strcpy((char*)s, "OpenVanilla Pe̍h-oē-jī (POJ)"));
    }
    
    virtual int initialize(OVDictionary* globalconfig, OVDictionary* localconfig,
        OVService* srv, char* path)
    {
        if (!localconfig->keyExist("改用ASCII輸出")) 
            localconfig->setInt("改用ASCII輸出", 0);
        
        asciioutput=localconfig->getInt("改用ASCII輸出");
        
        return 1;
    }
    
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
    {
        return 1;
    }
    
    virtual int update(OVDictionary*, OVDictionary* localconfig)
    {
        asciioutput=localconfig->getInt("改用ASCII輸出");
        murmur ("config changed, asciioutput option now set to %d\n", asciioutput);

        return 1;
    }
    
    virtual OVIMContext *newContext()
    {
        return new OVIMPOJContext(this);
    }
    
    virtual void deleteContext(OVIMContext* s)
    {
        delete s; 
    }
    
    virtual int isAsciiOutput()
    {
        return asciioutput;
    }
    
protected:
    int asciioutput;
};

int OVIMPOJContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
    OVService *srv) 
{
    int ascii=parent->isAsciiOutput();
    char composebuf[256];
        
    // if backspace of delete key is hit
    if (key->isCode(2, ovkBackspace, ovkDelete) && buf->length())
    { 
        seq.remove();
        buf->clear()->append(seq.compose(composebuf, ascii))->update();
        return 1;            
    }
    
    // if enter or compose key is hit
    if ((key->code()==ovkReturn || seq.isCompose(key->code())) && buf->length()) 
    {
        if (key->code()!=ovkReturn) seq.add(key->code());
        buf->clear()->append(seq.compose(composebuf, ascii))->send();
        seq.clear();
        return 1;
    }
    
    // key=[a-z][A-Z], so add to our key sequence
    if (key->isAlpha())
    {
        seq.add(key->code());
        buf->clear()->append(seq.compose(composebuf, ascii))->update();
        return 1;
    }
    
    // we find it's not a "valid" character, so we send up the buffer,
    // then tell the app the process the character itself    
    if (buf->length()) buf->send();
    seq.clear();        
    return 0;
}

// standard wrappers
OVLOADABLEWRAPPER(OVIMPOJ);
OVLOADABLECANUNLOAD;

