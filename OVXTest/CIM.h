// $Id$

// test text intended for deletion

// CIM.h:
//     Carbon Input Method, an input method framework written using
//     Apple's Carbon Application Framework

// Code based on Apple's BasicInputMethod. This is a rewritten version to
// provide C++ linkage and a more clear, minimalist input method framework.

// Copyright (c) 2004 by Lukhos D. Liu

// [BSD license]

#ifndef __CIM_h
#define __CIM_h

#define CIMCUSTOM

#include <Carbon/Carbon.h>
#include "CIMConst.h"

struct CIMSessionData;
typedef CIMSessionData **CIMSessionHandle;


const int cimIBMaxLen=1024;

class CIMInputBuffer
{
public:
    CIMInputBuffer() { instance=0; clear(); }
    void clear() { len=lastupdate=0; }
    void deletechar() { if (len) len--; }
    
    int length();   // returns actual Unicode (UTF-32) chars, not UTF-16 chars
    int put(UniChar c);
    int put(CFStringRef s);
    int put(char *s, CFStringEncoding e);
    OSErr send();
    OSErr update(Boolean send=FALSE, int cursorpos=-1, int hilitefrom=-1, 
                 int hiliteto=-1, ScriptCode sc=cimScript, 
                 LangCode=cimLanguage);
    
protected:
    int len;
    int lastupdate;
    UniChar buffer[cimIBMaxLen];
    ComponentInstance instance;
    CIMInputBuffer *bind(ComponentInstance inst)
        { instance=inst; return this; }
    friend ComponentResult CIMSessionEvent(CIMSessionHandle hndl, EventRef evnt);
    friend ComponentResult CIMSessionFix(CIMSessionHandle hndl);
};

struct CIMSessionData
{
    void init(ComponentInstance inst=nil)
        { instance=inst; buffer=new CIMInputBuffer; }
    void kill() { delete buffer; }
    
    ComponentInstance instance;
    CIMInputBuffer *buffer;
    
#ifdef CIMCUSTOM
    void *data;
#endif
};

ComponentResult CIMInitialize(ComponentInstance inst, MenuRef *mnu);
void            CIMTerminate(ComponentInstance inst);

ComponentResult CIMSessionOpen(ComponentInstance inst, CIMSessionHandle* hndl);
void            CIMSessionClose(CIMSessionHandle hndl);

ComponentResult CIMSessionActivate(CIMSessionHandle hndl);
ComponentResult CIMSessionDeactivate(CIMSessionHandle hndl);

ComponentResult CIMSessionEvent(CIMSessionHandle hndl, EventRef evnt);
ComponentResult CIMSessionFix(CIMSessionHandle hndl);
ComponentResult CIMSessionHidePalettes(CIMSessionHandle hndl);

ComponentResult CIMGetInputPosition(CIMSessionHandle hndl, Point *pnt);


#ifndef CIMCUSTOM
int CIMHandleInput(CIMInputBuffer *buf, unsigned char charcode, 
                   UInt32 keycode, UInt32 modifiers);
#else
int CIMCustomInitialize(MenuRef mnu);
void CIMCustomTerminate();
void *CIMCustomOpen();
int CIMCustomClose(void *data);
int CIMCustomActivate(void *data);
int CIMCustomDeactivate(void *data);
int CIMCustomHandleInput(CIMInputBuffer *buf, unsigned char charcode,
                         UInt32 keycode, UInt32 modifiers,  
                         Point *pnt, void *data);
int CIMCustomMenuHandler(UInt32 command, MenuRef mnu, void *data);
#endif

#endif      // of #ifndef __CIM_h
