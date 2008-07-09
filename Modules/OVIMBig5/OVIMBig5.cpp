// OVIMBig5.cpp: Big-5 Code Input Method
// 2005-2008 By Weizhong Yang
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

extern unsigned short OVIMBig5Table[19582][2];
//Convert Big5 to Unicode

unsigned int convert(unsigned int in)
{ 

	int i;
	for (i = 0; i < 19582 ; i++) {
		if (OVIMBig5Table[i][0] == in)
			return OVIMBig5Table[i][1];
	}
	return 0;
}

class OVIMBig5;

const size_t ebMaxKeySeq=4; 
class KeySeq
{
public:
    KeySeq() {
        clear();
    }
    void add(char c) {
        if (len == ebMaxKeySeq)
			return;
        buf[len++] = c;
        buf[len] = 0;
    }
    void remove() {
        if (!len)
			return;
        buf[--len] = 0;
    }
    void clear() {
        len = 0;
		buf[0] = 0;
    }
    char buf[ebMaxKeySeq];
    size_t len;
};

class OVBig5Context : public OVInputMethodContext
{
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*)
	{
        clear();
    }
    virtual void clear()
	{
	    keyseq.clear();
    }
	
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv)
    {
		if (key->isOpt() || key->isCommand() || key->isCtrl()) return 0;
		
		char * notitext;
		if (!strcasecmp(srv->locale(), "zh_TW"))
			notitext = "[Big5 內碼] ";
		else if (!strcasecmp(srv->locale(), "zh_CN"))
			notitext = "[Big5 内码] ";
		else
			notitext = "[Big5 Hex] ";		

        if (key->code()==ovkReturn || key->code()==ovkSpace) {
            if (!(strlen(keyseq.buf)))
				return 0;   // empty buffer, do nothing
				
			unsigned short i = convert(strtol(keyseq.buf, (char **) NULL, 16));
            keyseq.clear();
			if (i) {
                buf->clear()->append(srv->UTF16ToUTF8(&i, 1))->send()->clear();
                textbar->clear()->hide();
            } 
			else {
                buf->clear()->update();
                textbar->clear()->hide();
				if (!strcasecmp(srv->locale(), "zh_TW")) {
					srv->notify("錯誤：您輸入的不是正確的 Big5 內碼");
				}
				else if (!strcasecmp(srv->locale(), "zh_CN")) {
					srv->notify("错误：您输入的不是正确的 Big5 内码");
				}
				else {
					srv->notify("Error. No such code!");
				}	
                srv->beep();
            }
            return 1;   // key processed
        }

		if (key->code() == ovkEsc) {
			keyseq.clear();
            buf->clear()->update();;
			textbar->clear()->hide();
			return 1;
		}
		
		if (key->code()==ovkDelete || key->code()==ovkBackspace) {
			if (!strlen(keyseq.buf)) return 0;
			keyseq.remove();
			if (!strlen(keyseq.buf)) 
				textbar->clear()->hide();
			else
				textbar->clear()->append(notitext)->append(keyseq.buf)->update()->show();				
			buf->clear()->append(keyseq.buf)->update();
			return 1;
		}
		
		if (strlen(keyseq.buf) >= ebMaxKeySeq)
			return 1; // Max to 4 letters.
		
        if (isprint(key->code())) {
			if((key->code()>='a'&&key->code()<='f') || (key->code()>='0'&&key->code()<='9')) {
				keyseq.add(key->code());
				buf->clear()->append(keyseq.buf)->update();
				textbar->clear()->append(notitext)->append(keyseq.buf)->update()->show();
			}
			else {
			    srv->beep();
			}
			return 1;   // key processed
        }        
        return 0;   // key not processed
    }
    
protected:
	KeySeq keyseq;
};

class OVIMBig5 : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMBig5"; }
    
    virtual const char* localizedName(const char *locale)
	{
        if (!strcasecmp(locale, "zh_TW"))
            return "Big5 內碼";
        else if (!strcasecmp(locale, "zh_CN"))
            return "Big5 内码";
        return "Big5 Hexadecimal";        
    }

    virtual OVInputMethodContext* newContext()
	{
        return new OVBig5Context; 
    }
};

OV_SINGLE_MODULE_WRAPPER(OVIMBig5);
