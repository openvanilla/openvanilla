// OVIMRomanNew.cpp
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

#define OV_DEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int is_punc(char i){
	if(i >= '!' && i <= '@'){
		return 1;
	}
	if(i >= '[' && i <= '\''){
		return 1;
	}
	if(i >= '{' && i <= '~'){
		return 1;
	}
	return 0;
}

const size_t ebMaxKeySeq=40;
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


class OVIMRomanNewContext : public OVInputMethodContext
{
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*) {
        clear();
    }
    virtual void clear() {
		keyseq.clear();
    } 
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv) {
        
        // we only send printable characters so that they can pass thru 
        // full-width character filter (if enabled)
		
		if (k->code()==ovkSpace || k->code()==ovkReturn || is_punc(k->code())) {
            if (!(strlen(keyseq.buf))) return 0;   // empty buffer, do nothing
			if(k->code()!=ovkReturn) keyseq.add(k->code());
			b->clear()->append(keyseq.buf)->send();
			keyseq.clear();
            return 1;   // key processed
		}
		if (k->code()==ovkDelete || k->code()==ovkBackspace) {
			if(!strlen(keyseq.buf)) return 0;
			keyseq.remove();
			b->clear()->append(keyseq.buf)->update();
			return 1;
		}
		
		if (!isprint(k->code()) || k->isFunctionKey()) return 0;
		
		if(strlen(keyseq.buf) >= ebMaxKeySeq) return 1;
		
		if (isprint(k->code())){
			char s[2];
			sprintf(s, "%c", k->code());
			keyseq.add(k->code());
			b->clear()->append(keyseq.buf)->update();
			return 1;
		}
		return 0;
    }
protected:
	KeySeq keyseq;
};

class OVIMRomanNew : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMRomanNew"; }
    virtual OVInputMethodContext *newContext() { return new OVIMRomanNewContext; }
    virtual int initialize(OVDictionary *, OVService*, const char *mp) {
        return 1;
    }
    virtual const char* localizedName(const char *locale) {
        if (!strcasecmp(locale, "zh_TW")) return "新英數";
        if (!strcasecmp(locale, "zh_CN")) return "新英数";
        return "New Roman (alphanumeric)";
    }
};

OV_SINGLE_MODULE_WRAPPER(OVIMRomanNew);

