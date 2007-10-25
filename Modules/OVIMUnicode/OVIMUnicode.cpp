// OVIMUnicode.cpp: Unicode Input Method
// 2004-2007 By Weizhong Yang
//
// This program is Zonble-ware and adopts Zonble's License
// -- "How could a person who doesn't even believe in law adopt any license?"

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

const size_t ebMaxKeySeq=5;
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
			unsigned u = strtol(keyseq.buf, (char **) NULL, 16);
			commitUTF16(u, buf, srv);
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
    
    void commitUTF16(unsigned u, OVBuffer *buf, OVService *srv) {
        buf->clear();
        if (u <= 0xffff) {
            unsigned short s=u;
            buf->append(srv->UTF16ToUTF8((unsigned short*)&s, 1))->send();
        }
        else if (u <= 0xfffff) {
            unsigned short sur[2];
            sur[0]=(0xd800 - (0x10000 >> 10))+(u >> 10);
            sur[1]=0xdc00 + (u & 0x3ff);
            buf->append(srv->UTF16ToUTF8(sur, 2))->send();
        }
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
        return "Unicode Hexadecimal";        
    }

    virtual OVInputMethodContext* newContext()  {
        return new OVUnicodeContext; 
    }
};

OV_SINGLE_MODULE_WRAPPER(OVIMUnicode);
