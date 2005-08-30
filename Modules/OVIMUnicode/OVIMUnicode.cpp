// OVIMUnicode.cpp 2004 by Weizhong Yang
// http://openvanilla.org

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
	#include <OpenVanilla/OVLibrary.h>
#else
	#include "OpenVanilla.h"
	#include "OVLibrary.h"
	#define strcasecmp stricmp
#endif

class OVIMUnicode;

const size_t ebMaxKeySeq=4; 
class KeySeq {
public:
    KeySeq() {
        clear();
    }
    void add(char c) {
        if (len == ebMaxKeySeq) return;
        buf[len++]=c;
        buf[len]=0;
    }
    void remove() {
        if (!len) return;
        buf[--len]=0;
    }
    void clear() {
        len=0; buf[0]=0;
    }
    char buf[ebMaxKeySeq];
    size_t len;
};

class OVUnicodeContext : public OVInputMethodContext {
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*) {
        clear();
    }
    virtual void clear() {
	    keyseq.clear();
    }
	
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar,
        OVService *srv)
    {
		if (key->isOpt() || key->isCommand() || key->isCtrl()) return 0;

        if (key->code()==ovkReturn || key->code()==ovkSpace) {
            if (!(strlen(keyseq.buf))) return 0;   // empty buffer, do nothing
			unsigned short i = strtol(keyseq.buf, (char **) NULL, 16);
			
			buf->clear()->append(srv->UTF16ToUTF8(&i, 1))->send();
            textbar->clear()->hide();
			keyseq.clear();
            return 1;   // key processed
        }

		if (key->code()==ovkEsc) {
			textbar->clear()->hide();
			keyseq.clear();
			return 1;
		}
		
		if (key->code()==ovkDelete || key->code()==ovkBackspace) {
			if(!strlen(keyseq.buf)) return 0;
			keyseq.remove();
			if(!strlen(keyseq.buf)) 
				textbar->clear()->hide();
			else
				textbar->clear()->append((char *)"[OV Unicode Hex] U+")->append(keyseq.buf)->update()->show();
			buf->clear()->append(keyseq.buf)->update();
			return 1;
		}
		
		if(strlen(keyseq.buf) >= ebMaxKeySeq) return 1; // Max to 4 letters.
		
        if (isprint(key->code()))
        {
			if((key->code()>='a'&&key->code()<='f') || (key->code()>='0'&&key->code()<='9'))
			{
				keyseq.add(key->code());
				buf->clear()->append(keyseq.buf)->update();
				textbar->clear()->append((char *)"[OV Unicode Hex] U+")->append(keyseq.buf)->update()->show();
			}
			return 1;   // key processed
        }        
        return 0;   // key not processed
    }
    
protected:
	KeySeq keyseq;
};

class OVIMUnicode : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMUnicode"; }
    
    virtual const char* localizedName(const char *locale) {
        if (!strcasecmp(locale, "zh_TW"))
            return "Unicode 內碼";
        else if (!strcasecmp(locale, "zh_CN"))
            return "Unicode 内码";
        return "Unicode hexadecimal";        
    }

    virtual OVInputMethodContext* newContext()  {
        return new OVUnicodeContext; 
    }
};

OV_SINGLE_MODULE_WRAPPER(OVIMUnicode);
