// OVIMUnicode.cpp 2004 by Weizhong Yang

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"

class OVIMUnicode;

const int ebMaxKeySeq=4;
class KeySeq
{
public:
    KeySeq() { len=0; buf[0]=0; }
    void add(char c)
{
	if (len == ebMaxKeySeq) return;
	buf[len++]=c;
	buf[len]=0;
}
void remove()
{
	if (!len) return;
	buf[--len]=0;
}
void clear() { len=0; buf[0]=0; }
char buf[ebMaxKeySeq];
int len;
};

class OVUnicodeContext : public OVIMContext
{
public:
    OVUnicodeContext(OVIMUnicode *p)
    {
        parent=p;
    }
    
    virtual ~OVUnicodeContext(){}
    
    virtual int activate(OVService *)
    {
		keyseq.clear();
        return 1;
    }
    
    virtual int deactivate(OVService *)
    {
		keyseq.clear();
        return 1;
    }
	
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv)
    {
		if (key->isOpt() || key->isCommand() || key->isCtrl())
        {
			return 0;
        }
		    
        if (key->isCode(3, ovkReturn, ovkMacEnter, ovkSpace))
        {
            if (!(strlen(keyseq.buf))) return 0;   // if buffer is empty, don't process
			unsigned short i = strtol(keyseq.buf, (char **) NULL, 16);
			buf->clear()->append(&i, ovEncodingUTF16Auto, 1)->send();
            textbar->clear()->hide();
			keyseq.clear();
            return 1;   // key processed
        }

		if (key->code()==ovkEscape)
		{
			textbar->clear()->hide();
			keyseq.clear();
			return 1;
		}
		
		if (key->isCode(2, ovkDelete, ovkBackspace))
		{
			if(!strlen(keyseq.buf)) return 0;
			keyseq.remove();
			if(!strlen(keyseq.buf)) 
				textbar->clear()->hide();
			else
				textbar->clear()->append((char *)"U+")->append(keyseq.buf)->update()->show();
			return 1;
		}
		
		if(strlen(keyseq.buf) > 3) return 1; //Max to 4 letters.
		
        if (key->isPrintable())
        {
			if((key->code() >= 'a' && key->code() <= 'f') || (key->code() >= '0' && key->code() <= '9'))
			{
				keyseq.add(key->code());
				textbar->clear()->append((char *)"U+")->append(keyseq.buf)->update()->show();
			}
			return 1;   // key processed
        }        
        return 0;   // key not processed
    }
    
protected:
    OVIMUnicode *parent;
	KeySeq keyseq;
};

class OVIMUnicode : public OVInputMethod
{
public:
    OVIMUnicode(){}
    virtual ~OVIMUnicode(){}
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVIMUnicode"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        if (!strcasecmp(locale, "zh_TW"))
            return strlen(strcpy((char*)s, "OV Unicode Hex"));
        else if (!strcasecmp(locale, "zh_CN"))
            return strlen(strcpy((char*)s, "OV Unicode Hex"));
        else
            return strlen(strcpy((char*)s, "OV Unicode Hex"));
    }

    virtual int initialize(OVDictionary*, OVDictionary*, OVService*, char*)
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

    virtual OVIMContext* newContext() 
    {
        return new OVUnicodeContext(this); 
    }
    
    virtual void deleteContext(OVIMContext* s) 
    {
        delete s; 
    }
};


// use these two wrappers
OVLOADABLEWRAPPER(OVIMUnicode)
OVLOADABLECANUNLOAD