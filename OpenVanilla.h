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

class OVTextBar : public OVObject
{
public:
    // functions that update the content of the text bar
    virtual int clear() { return 0; }
    virtual int append(void *s, OVEncoding e=ovEncodingUTF8, int l=0)
        { return 0; }
    
    // functions that change the display status
    virtual int onscreen() { return 0; }
    virtual int hide() { return 0; }
    virtual int show() { return 0; }
    virtual int update() { return 0; }
};


class OVBuffer : public OVObject
{
public:
    virtual int clear() { return 0; }
    virtual int send() { return 0; }
    virtual int updatedisplay(int cursorpos=-1, int hilitefrom=-1, int hiliteto=-1);
    virtual int append (void *s, OVEncoding e=ovEncodingUTF8, int l=0)
        { return 0; }
    virtual int length() { return 0; }    
};


class OVDictionary : public OVObject
{
public:
    virtual int getint(char*) { return 0; }
    virtual int getint(char*, int) { return 0; }
    virtual int putint(char*, int) { return 0; }
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

    virtual int keyevent(OVKeyCode*, OVBuffer*, OVTextBar*, OVService*) 
        { return 0; }
};

class OVInputMethod : public OVObject
{
public:
    virtual int identifier(char* p)
        { *p=0; return 0; }

    virtual int open(OVService*, OVDictionary*, OVDictionary*)
        { return 1; }
    virtual int close(OVService*, OVDictionary*, OVDictionary*)
        { return 1; }    
    virtual int update(OVDictionary*, OVDictionary*) { return 1; }
    
    virtual OVIMSession *newsession() { return new OVIMSession; }
    virtual void deletesession(OVIMSession* s) { delete s; }
};


enum 
{
    ovkEscape=27, 
    ovkSpace=32,
    ovkReturn=13, ovkMacEnter=0x120,
    ovkDelete=8, ovkBackspace=127,
    ovkUp=0x101, ovkDown=0x102, ovkLeft=0x103, ovkRight=0x104,
    ovkHome=0x111, ovkEnd=0x112,
    ovkPageUp=0x123, ovkPageDown=0x134,
    ovkUndefined=0xffff
};

#endif  // of #ifndef __OpenVanilla_h
