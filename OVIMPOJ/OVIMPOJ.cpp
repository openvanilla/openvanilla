// OVIMPOJ.cpp: prototype for POJ IM

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
    "m", "ḿ","m̀","m","m̂","ḿ","m̄","m̍",
    "n", "ń","ǹ","n","n̂","ń","n̄","n̍",
    "o", "ó","ò","o","ô","ó","ō","o̍",
    "o·", "ó·","ò·","o·","ô·","o·","ō·","o̍·",
    "u", "ú","ù","u","û","ú","ū","u̍",
    "A", "Á", "À", "a", "Â", "Á", "Ā", "A̍",
    "E", "É", "È", "E", "Ê", "É", "Ē", "E̍",
    "I", "Í", "Ì", "I", "Î", "Í", "Ī", "I̍",
    "M","Ḿ","M̀","M","M̂","Ḿ","M̄","M̍",
    "N","Ń","Ǹ","N","N̂","Ń","N̄","N̍",
    "O","Ó","Ò","O","Ô","Ó","Ō","O̍",
    "O·","Ó·","Ò·","O·","Ô·","Ó·","Ō·","O̍·",
    "U","Ú","Ù","U","Û","Ú","Ū","U̍"
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
        seq[len++]=c;
        seq[len]=0;
        return 1;
    }
    
    void remove()
    {
        if (!len) return;
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
    
    int compose(char *buf)
    {
        int tone=1;
    
        char *b=buf;

        if (!len)
        {
            *buf=0;
            return 0;
        }

        if (seq[len-1] >= '1' && seq[len-1] <= '8') tone=seq[len-1]-'0';
        murmur ("tone=%d", tone);


        char *s=seq;
        int vcomposed=0;        // if the first vowel is composed
        
        while (*s)
        {
            if (isCompose(*s)) break;
            
            int vo=vowelorder(*s);
            murmur("char=%c, vowelorder=%d", *s, vo);
            murmur("To compose: %s",s);
            // vowel ?
            
            if (tolower(*s)=='n' && tolower(*(s+1))=='n')
            {
                strcat(b, nasel);
                b+=strlen(nasel);
                s+=2;
                continue;
            }
            
            if (vcomposed || ( vo == -1 ) ||
                 ( tolower(*s) == 'i' && (vowelorder(*(s+1)) != -1 ) )) 
            {
                *b++=*s++;
                murmur("Composed: %s",buf);
                continue;
            }
            
            char *vstr=vowel2tone(*s, tone);
            if (vstr)
            {
                murmur("char: %c, tone: %d, vowel2tone: %s", *s, tone, vstr);
                strcat(b, vstr);
                b+=strlen(vstr);
                vcomposed=1;
            }
            s++;
            murmur("Composed: %s",buf);
        }
        *b=0;
        return strlen(buf);            
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
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, OVService *srv) 
    {
        char composebuf[256];
        murmur("I'm in keyEvent, code=%d", key->code());
//        if (!textbar->onScreen()) textbar->show();
        
        if (key->isCode(2, ovkBackspace, ovkDelete) && buf->length())
        { 
            murmur("removing!");
            seq.remove();
            seq.compose(composebuf);
            murmur("Backspace: %c , composed: %s",key->code(),composebuf);
            buf->clear()->append(composebuf)->update();
            return 1;            
        }
        
        if ((key->code()==ovkReturn || seq.isCompose(key->code())) && buf->length()) {
            if (key->code()!=ovkReturn) seq.add(key->code());
            seq.compose(composebuf);
            buf->clear()->append(composebuf)->send();
            seq.clear();
            return 1;
        }
        
        if (key->isAlpha()) {
            seq.add(key->code());
            seq.compose(composebuf);
            murmur("isPrintable: %c , composed: %s",key->code(),composebuf);
            buf->clear()->append(composebuf)->update();
            return 1;
        }
        
        murmur ("charcode=(%c), kicking it back to app)", key->code());
        if (buf->length()) buf->send();
        seq.clear();
        
        return 0;
    }
    
protected:
    OVIMPOJ *parent;
    POJKeySeq seq;
};

class OVIMPOJ : public OVInputMethod
{
public:
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVIMPOJ"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        return strlen(strcpy((char*)s, "OpenVanilla Pe̍h-oē-jī/peh8-oe7-ji7"));
    }
    
    virtual int initialize(OVDictionary* globalconfig, OVDictionary* localconfig,
        OVService* srv, char* path)
    {
        return 1;
    }
    
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
    {
        return 1;
    }
    
    virtual int update(OVDictionary*, OVDictionary*)
    {
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
};

//
OVLOADABLEWRAPPER(OVIMPOJ);
OVLOADABLECANUNLOAD;

