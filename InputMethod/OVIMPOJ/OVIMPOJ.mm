// OVIMPOJ.cpp: prototype for POJ IM

// Copyright (c) 2004 by Lukhnos D. Liu, Kang-ming Liu, and Pek-tiong Tan

#define OVDEBUG 1

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLoadable.h>
#include <OpenVanilla/OVUtility.h>
#include "VXCIN.h"
#include "VXUtility.h"


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

enum
{
    pojToneByNumber=0,
    pojTonePrebind=1,
    pojTonePostbind=2  
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

int tonemark(char c)
{
    switch (c)
    {
        case '\'': return 2;
        case '`': return 3;
        case '^': return 5;
        case '=': return 7;
        case 'x': return 8;
    }
    return 0;
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
        if (strchr(" 12345678", c)) return 1;
        return 0;
    }
    
    int add(char c, int layout=0)
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

    void clear()
    {
        len=0;
        prebindwait=0;
        presettone=0;
        bindvowel=0;
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
    
    
    char *normalize()
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
    
    char *finalize()
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
    
    char *compose(char *buf, int asciioutput=0)
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
        
        while (c=*s)        // C style, very C style
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
        
            char *vstr=vowel2tone(c, tone);
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

protected:
    int len;
    char seq[pojMaxSeqLen+1];
    
    int presettone;
    int prebindwait;
    int bindvowel;
};

class OVIMPOJ;

class OVIMPOJContext : public OVIMContext
{
public:
    OVIMPOJContext(OVIMPOJ *p) : parent(p), candi(0), list(nil) {}
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
    
    int candi;
    NSMutableArray* list;
    void composeCandidate(char *pojs, NSArray* ar, OVTextBar *textbar);
    int copyAndDispose(OVBuffer *buf, int c, char *append);
};

class OVIMPOJ : public OVInputMethod
{
public:
    OVIMPOJ()
    {
        asciioutput=0;
        keylayout=pojToneByNumber;
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
        if (!localconfig->keyExist("ASCII_Output")) 
            localconfig->setInt("ASCII_Output", 0);
        
        if (!localconfig->keyExist("Keyboard_Layout"))
            localconfig->setInt("Keyboard_Layout", pojToneByNumber);       
        
        if (!localconfig->keyExist("POJ_cin_path"))
            localconfig->setString("POJ_cin_path", 
            "/Library/OpenVanilla/Development/POJ.cin");
            
        if (!localconfig->keyExist("script"))
        	localconfig->setInt("script", 1);
        if (!localconfig->keyExist("lang"))
        	localconfig->setInt("lang", 1);
        
        asciioutput=localconfig->getInt("ASCII_Output");
        keylayout=localconfig->getInt("Keyboard_Layout");
        
        script=localconfig->getInt("script");
        lang=localconfig->getInt("lang");
        
        char buf[256];
        localconfig->getString("POJ_cin_path", buf);
        murmur ("POJ.cin path=%s", buf);
        cin.read(buf);
        
        return 1;
    }
    
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
    {
        return 1;
    }
    
    virtual int update(OVDictionary*, OVDictionary* localconfig)
    {
        asciioutput=localconfig->getInt("ASCII_Output");
        keylayout=localconfig->getInt("Keyboard_Layout");
		script=localconfig->getInt("script");
		lang=localconfig->getInt("lang");

        murmur ("config changed, asciioutput now=%d, keyboard layout=%d", asciioutput, keylayout);
        
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
    
    virtual int keyLayout()
    {
        return keylayout;
    }
    
    virtual NSArray *find(char *key)
    {
        return cin.find(key);
    }
    
    int script;
    int lang;
    
protected:
    int asciioutput;
    int keylayout;
    VXCIN cin;
};

int OVIMPOJContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
    OVService *srv) 
{
    int ascii=parent->isAsciiOutput();
    char composebuf[256];

	char *dash="-";
	char *space=" ";
	char *append=NULL;

    if (candi)
    {
        int c=key->code();
        
        if ((c >= '0' && c <= '9') || c==' ' || c=='-')
        {
            int cc=1;
            if (c==' ') append=space;
            if (c=='-') append=dash;
            
            if (c >= '0' && c <='9') cc=c-'0';
            if (!cc) cc=10; else cc--;
            
            if (!copyAndDispose(buf, cc, append)) return 1;
            textbar->hide();
            candi=0;
            return 1;
        }
        
        if (c==ovkEscape)
        {
            textbar->hide();
            buf->clear()->update();
            candi=0;
            return 1;
        }
        
        return 1;
    }

        
    // if backspace of delete key is hit
    if (key->isCode(2, ovkBackspace, ovkDelete) && buf->length())
    { 
        seq.remove();
        buf->clear()->append(seq.compose(composebuf, 0))->update();
        return 1;            
    }
    
    // if enter or compose key is hit
    if ((key->code()==ovkReturn || seq.isCompose(key->code())) && buf->length()) 
    {
        murmur("composing! code=%d", key->code());
        char final[256];
        
        if (key->code()!=ovkReturn) seq.add(key->code());
        seq.normalize();
        strcpy(final, seq.compose(composebuf, ascii));
        buf->clear()->append(final)->update();
        
        char query[256];
        strcpy (query, seq.finalize());
                
        murmur ("key composed, final internal representation=%s", query);
        
        // query string
        NSArray *ar=parent->find(query);
        if (!ar)
        {
//            buf->append(" ")->send();
			murmur ("script=%d, lang=%d", parent->script, parent->lang);
            buf->send((unsigned int)0xf0000000+(unsigned int)(parent->script << 16)+(unsigned int)parent->lang);
        }
        else
        {
            candi=1;
            textbar->clear();
            composeCandidate(final, ar, textbar);
            textbar->update()->show();
        }

        seq.clear();        
        return 1;
    }
    
    // key=[a-z][A-Z], so add to our key sequence
    if (key->isAlpha() || (parent->keyLayout() && tonemark(key->code())))
    {
        murmur("alpha: %d", key->code());
        seq.add(key->code(), parent->keyLayout());
        char *b=seq.compose(composebuf, 0);
        murmur("composed=%s", b);
        buf->clear()->append(b)->update();
        return 1;
    }
    
    // we find it's not a "valid" character, so we send up the buffer,
    // then tell the app the process the character itself    
    if (buf->length())
    {
        seq.normalize();
        buf->clear()->append(seq.compose(composebuf, ascii))->send();
    }
    
    seq.clear();        
    return 0;
}

void OVIMPOJContext::composeCandidate(char *pojs, NSArray *ar, OVTextBar *textbar)
{
    if (list) [list release];
      
    textbar->clear();
    
    char buf[256];
    char pojstr[256];
    strcpy(buf, "1.");
    strcpy(pojstr, pojs);
//    strcat(pojstr, " ");
    
    list=[NSMutableArray new];
    [list addObject: (NSString*)VXCreateCFString(pojstr)];
    textbar->append(buf)->append(pojstr);
        
    for (int i=0; i<[ar count]; i++)
    {
        sprintf (buf, " %d.", i+2);
        NSString* str=[ar objectAtIndex: i];
        [list addObject: str];
        textbar->append(buf)->append([str UTF8String]);
    }    
    
    textbar->update();
}

int OVIMPOJContext::copyAndDispose(OVBuffer *buf, int c, char* append)
{
    if (c >= [list count]) return 0;
    
    NSString *s=[list objectAtIndex: c];
    buf->clear()->append([s UTF8String]);
    if (append) buf->append(append);
    buf->send();
    [list release];
    list=nil;
    return 1;
}

// standard wrappers
OVLOADABLEWRAPPER(OVIMPOJ);
OVLOADABLEOBJCWRAPPER;
