// OpenVanilla.h
// Copyright (c) 2004 by Lukhnos D. Liu (lukhnos@openvanilla.org)
// and Kang-ming Liu (gugod@gugod.org)


#ifndef __OpenVanilla_h
#define __OpenVanilla_h

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

const unsigned int ovVersion=0x00600200;    // for 0.6.2 (0x00,0x06,0x02,0x00)

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
    ovEncodingUTF16Auto=1,	// use this if you have e.g. short x=0xae10;
    ovEncodingUTF16BE=2,     
    ovEncodingUTF16LE=4,
    ovEncodingBig5=16,
    ovEncodingBig5HKSCS=16,
    ovEncodingEUC_CN=24,
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
    ovExceptionMemory=1,
    ovExceptionBuffer=2,
    ovExceptionNotSupported=3
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

    virtual OVTextBar* hide() { return this; }
    virtual OVTextBar* show() { return this; }
    virtual OVTextBar* update() { return this; }
    virtual int onScreen() { return 0; }
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
	
	virtual OVBuffer* appendChar(char c)
        { char buf[2]; buf[0]=c; buf[1]=0; return append(buf); }
	
    // returns the number of GRAPHEMES (ie. actual chars displayed) in the buffer
    virtual int length() { return 0; }    
};


class OVDictionary : public OVObject
{
public:
    virtual int keyExist(const void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
        { return 0; }
        
    virtual int getInt(const void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
        { return 0; }
    virtual int getIntDefault(const void *k, int defv, OVEncoding 
        e=ovEncodingUTF8, int kl=0)
            { if (!keyExist(k, e, kl)) setInt(k, defv, e, kl); 
              return getInt(k, e, kl); }
    virtual int setInt(const void *key, int value, OVEncoding e=ovEncodingUTF8,
        int keylen=0) { return 0; }
            	
	virtual int getString(const void *key, void *str, OVEncoding e=ovEncodingUTF8,
		int keylen=0, int maxlen=0) { return 0; }
    virtual int getStringDefault(const void *k, void *s, const void *defv, 
        OVEncoding e=ovEncodingUTF8, int kl=0, int ml=0)
            { if (!keyExist(k, e, kl)) setString(k, defv, e, kl);
              return getString(k, s, e, kl); }
	virtual int setString(const void *key, const void *value, 
        OVEncoding e=ovEncodingUTF8, int keylen=0, int valuelen=0) { return 0; }
		
    virtual int newDictionary(const void *key, OVEncoding e=ovEncodingUTF8, 
        int keylen=0) { return 0; }
    virtual OVDictionary* getDictionary(const void *key, 
        OVEncoding e=ovEncodingUTF8, int keylen=0)   { return NULL; }
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
    virtual int isAlpha() { return isalpha(code()); }
    virtual int isNumeric() { return (code() <= '9' && code() >= '0'); }
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
    ovkDelete=127, ovkBackspace=8,
    ovkUp=30, ovkDown=31, ovkLeft=28, ovkRight=29,
    ovkHome=1, ovkEnd=4,
    ovkPageUp=11, ovkPageDown=12,
    ovkTab='\t'
};

#endif  // of #ifndef __OpenVanilla_h
