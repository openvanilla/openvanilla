// OpenVanilla.h

#ifndef __OpenVanilla_h
#define __OpenVanilla_h

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

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
    ovEncodingUTF16BE=2,     
//  ovEncodingUTF16LE=3,
//  ovEncodingUTF32BE=4,
//  ovEncodingUTF32LE=5,
    ovEncodingBig5=16,
    ovEncodingBig5HKSCS=16,
    ovEncodingNone8BitEncodingMask=7
};

enum
{
    ovLangAll=0,
    ovLangTradChinese=1,
    ovLangSimpChinese=2,
    ovLangJapanese=3,
    ovLangKorean=4
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
    virtual OVTextBar* append(void *s, OVEncoding e=ovEncodingUTF8, int l=0)
        { return this; }
    
    // functions that change the display status
    virtual int onscreen() { return 0; }
    virtual int hide() { return 0; }
    virtual int show() { return 0; }
    virtual int update() { return 0; }
};


class OVBuffer : public OVObject
{
public:
    virtual OVBuffer* clear() { return this; }
    virtual OVBuffer* send(OVLanguage lang=ovLangAll) { return this; }
    virtual OVBuffer* updatedisplay(OVLanguage lang=ovLangAll)
            { return updatedisplay(-1, -1, -1, lang); }
    virtual OVBuffer* updatedisplay(int cursorpos, int hilitefrom, 
        int hiliteto, OVLanguage lang=ovLangAll)
            { return this; }
    virtual OVBuffer* append (void *s, OVEncoding e=ovEncodingUTF8, int l=0)
        { return this; }
    virtual int length() { return 0; }    
};


class OVDictionary : public OVObject
{
public:
    virtual int keyexist(void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
        { return 0; }
        
    virtual int getint(void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
        { return 0; }
    virtual int putint(void *key, int value, OVEncoding e=ovEncodingUTF8,
        int keylen=0) { return 0; }
            	
	virtual int getstring(void *key, void *str, OVEncoding e=ovEncodingUTF8,
		int keylen=0, int maxlen=0) { return 0; }
	virtual int putstring(void *key, void *value, OVEncoding e=ovEncodingUTF8,
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
    virtual int uppercode() { return toupper(code()); }
    virtual int lowercode() { return tolower(code()); }
    
    virtual int iscode(int argc, ...)
    {
        int r=0;
        va_list ap;
        va_start(ap, argc);
        for (int i=0; i<argc; i++) { int j=va_arg(ap, int); if (j==code()) r=1; }
        va_end(ap);
        return r;
    }

    virtual int isshift() { return 0; }
    virtual int iscapslock() { return 0; }
    virtual int isctrl() { return 0; }
    virtual int isalt() { return 0; }
    virtual int isopt() { return 0; }
    virtual int iscommand() { return 0; }
    virtual int isprintable() { return isprint(code()); }
};

class OVIMSession : public OVObject
{
public:
    virtual int activate(OVService*) { return 1; }
    virtual int deactivate(OVService*) { return 1; }
    virtual int clear(OVService*) { return 1; }

    virtual int keyevent(OVKeyCode*, OVBuffer*, OVTextBar*, OVService*) 
        { return 0; }        
};

class OVInputMethod : public OVObject
{
public:
    virtual int identifier(char* p)
        { *p=0; return 0; }

    virtual int initialize(OVDictionary*, OVDictionary*, OVService*, char*)
        { return 1; }
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
        { return 1; }    
    virtual int update(OVDictionary*, OVDictionary*) { return 1; }
    
    virtual OVIMSession *newsession() { return new OVIMSession; }
    virtual void deletesession(OVIMSession* s) { delete s; }
};


enum 
{
    ovkEscape=27, 
    ovkSpace=32,
    ovkReturn=13, ovkMacEnter=3,
    ovkDelete=8, ovkBackspace=127,
    ovkUp=30, ovkDown=31, ovkLeft=28, ovkRight=29,
    ovkHome=1, ovkEnd=4,
    ovkPageUp=11, ovkPageDown=12,
    ovkUndefined=0xffff
};

#endif  // of #ifndef __OpenVanilla_h
