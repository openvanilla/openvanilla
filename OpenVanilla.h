// OpenVanilla.h

#ifndef __OpenVanilla_h
#define __OpenVanilla_h

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

const unsigned int ovVersion=0x0000ffff;

class OVObject
{
public:
    virtual ~OVObject() {}
};

typedef int OVEncoding;
typedef int OVLanguage;
typedef int OVException;

enum
{
    ovEncodingUTF8=0,
//  ovEncodingUTF16Auto=1,
    ovEncodingUTF16BE=2,     
//  ovEncodingUTF16LE=3,
    ovEncodingBig5=16,
    ovEncodingBig5HKSCS=16,
    ovEncodingNon8BitEncodingMask=7
};

enum
{
    ovLangAll=0,
    ovLangTradChinese=1,
    ovLangSimpChinese=2,
    ovLangJapanese=3,
    ovLangKorean=4,
	ovLangWestern=255,
};

enum
{
    ovException=0,
    ovMemoryException=1
};

class OVTextBar : public OVObject
{
public:
    // functions that update the content of the text bar
    virtual OVTextBar* clear() { return this; }

    // receive a generic type pointer, the length is the number of CODE POINTS
    // (for byte type, length of total bytes; for UTF-16 type, surrogates incl'd)
    virtual OVTextBar* append(void *s, OVEncoding e=ovEncodingUTF8, int len=0)
        { return this; }

    virtual OVTextBar* hide() { return 0; }
    virtual OVTextBar* show() { return 0; }
    virtual OVTextBar* update() { return 0; }
    virtual int onscreen() { return 0; }
};


class OVBuffer : public OVObject
{
public:
    virtual OVBuffer* clear() { return this; }
    
    virtual OVBuffer* send(OVLanguage lang=ovLangAll) { return this; }
    virtual OVBuffer* update(OVLanguage lang=ovLangAll)
            { return update(-1, -1, -1, lang); }
    virtual OVBuffer* update(int cursorpos, int hilightfrom, 
        int hilightto, OVLanguage lang=ovLangAll)
            { return this; }
    virtual OVBuffer* append (void *s, OVEncoding e=ovEncodingUTF8, int len=0)
        { return this; }
    virtual OVBuffer* append (char c)
		{ char buf[2]; buf[0]=c; buf[1]=0; return append(buf); }
	
    // returns the number of GRAPHEMES (ie. actual chars displayed) in the buffer
    virtual int length() { return 0; }    
};


class OVDictionary : public OVObject
{
public:
    virtual int keyExist(void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
        { return 0; }
        
    virtual int getInt(void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
        { return 0; }
    virtual int putInt(void *key, int value, OVEncoding e=ovEncodingUTF8,
        int keylen=0) { return 0; }
            	
	virtual int getString(void *key, void *str, OVEncoding e=ovEncodingUTF8,
		int keylen=0, int maxlen=0) { return 0; }
	virtual int putString(void *key, void *value, OVEncoding e=ovEncodingUTF8,
		int keylen=0, int valuelen=0) { return 0; }
};


class OVService : public OVObject
{
public:
    virtual void beep()  {}
};


class OVKeyCode : public OVObject
{
public:
    virtual int code() { return 0; }
    virtual int upper() { return toupper(code()); }
    virtual int lower() { return tolower(code()); }
    
    virtual int isCode(int argc, ...)
    {
        int r=0;
        va_list ap;
        va_start(ap, argc);
        for (int i=0; i<argc; i++) 
            { int j=va_arg(ap, int); if (j==code()) { r=1; break; } }
        va_end(ap);
        return r;
    }

    virtual int isShift() { return 0; }
    virtual int isCapslock() { return 0; }
    virtual int isCtrl() { return 0; }
    virtual int isAlt() { return 0; }
    virtual int isOpt() { return 0; }
    virtual int isCommand() { return 0; }
    virtual int isPrintable() { return isprint(code()); }
};

class OVIMContext : public OVObject
{
public:
    virtual int activate(OVService*) { return 1; }
    virtual int deactivate(OVService*) { return 1; }
    virtual int clear() { return 1; }
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVTextBar*, OVService*) 
        { return 0; }        
};

const int ovMaxIdentifierLength=32;
const int ovMaxNameLength=128;

class OVInputMethod : public OVObject
{
public:
    virtual int identifier(char* s)
        { *s=0; return 0; }
    virtual int name(char *locale, void *s, OVEncoding *enc)
        { *(char*)s=0; *enc=ovEncodingUTF8; return 0; }

    virtual int initialize(OVDictionary* globalconfig, OVDictionary* localconfig,
        OVService* srv, char* path)
            { return 1; }
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
        { return 1; }    
    virtual int update(OVDictionary*, OVDictionary*) { return 1; }
    
    virtual OVIMContext *newContext() { return new OVIMContext; }
    virtual void deleteContext(OVIMContext* s) { delete s; }
};


enum 
{
    ovkEscape=27, 
    ovkSpace=32,
    ovkReturn=13, ovkMacEnter=3,
    ovkDelete=8, ovkBackspace=127,
    ovkUp=30, ovkDown=31, ovkLeft=28, ovkRight=29,
    ovkHome=1, ovkEnd=4,
    ovkPageUp=11, ovkPageDown=12
};

#endif  // of #ifndef __OpenVanilla_h
