// OVIMBig5.cpp 2005 by Weizhong Yang
// openvanilla.org

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"
#include "OVIMBig5.h"

class OVIMBig5;

unsigned int convert(unsigned int in){ //Convert Big5 to Unicode
	int i;
	for (i =0; i< 19582 ; i++){
		if(map[i][0] == in)
			return map[i][1];
	}
	return 0;
}

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

class OVBig5Context : public OVIMContext
{
public:
    OVBig5Context(OVIMBig5 *p)
    {
        parent=p;
    }
    
    virtual ~OVBig5Context(){}
    
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
			unsigned short i = convert(strtol(keyseq.buf, (char **) NULL, 16));
			buf->clear()->send();
			textbar->clear()->hide();
			keyseq.clear();
			if(i) buf->append(&i, ovEncodingUTF16Auto, 1)->send();
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
				textbar->clear()->append((char *)"[OV Big5] 0x")->append(keyseq.buf)->update()->show();
			buf->clear()->append(keyseq.buf)->update();
			return 1;
		}
		
		if(strlen(keyseq.buf) > 3) return 1; //Max to 4 letters.
		
        if (key->isPrintable())
        {
			if((key->code() >= 'a' && key->code() <= 'f') || (key->code() >= '0' && key->code() <= '9'))
			{
				keyseq.add(key->code());
				buf->clear()->append(keyseq.buf)->update();
				textbar->clear()->append((char *)"[OV Big5] 0x")->append(keyseq.buf)->update()->show();
			}
			return 1;   // key processed
        }        
        return 0;   // key not processed
    }
    
protected:
    OVIMBig5 *parent;
	KeySeq keyseq;
};

class OVIMBig5 : public OVInputMethod
{
public:
    OVIMBig5(){}
    virtual ~OVIMBig5(){}
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVIMBig5"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        if (!strcasecmp(locale, "zh_TW"))
            return strlen(strcpy((char*)s, "OV 大五碼"));
        else if (!strcasecmp(locale, "zh_CN"))
            return strlen(strcpy((char*)s, "OV 大五码"));
        else
            return strlen(strcpy((char*)s, "OV Big5"));
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
        return new OVBig5Context(this); 
    }
    
    virtual void deleteContext(OVIMContext* s) 
    {
        delete s; 
    }
};


// use these two wrappers
OVLOADABLEWRAPPER(OVIMBig5)
OVLOADABLECANUNLOAD
