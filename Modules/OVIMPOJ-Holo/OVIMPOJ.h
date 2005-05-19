// OVIMPOJ.h: prototype for POJ IM

// Copyright (c) 2004-2005 by Lukhnos D. Liu, Kang-ming Liu, and Pek-tiong Tan

#ifndef __OVIMPOJ_h
#define __OVIMPOJ_h

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>
#include <OpenVanilla/OVLibrary.h>
#include "VXCIN.h"
#include "VXCandidate.h"
#include "VXUtility.h"
#include "POJKeySeq.h"
#include "VXCandidate.h"

class OVIMPOJ;

class OVIMPOJContext : public OVInputMethodContext
{
public:
    OVIMPOJContext(OVIMPOJ *p, VXCIN *ctab);
    ~OVIMPOJContext();
    
    virtual void clear();
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar,
        OVService *srv);
    
protected:
    virtual int candidateEvent(OVKeyCode *key, OVBuffer *buf,
        OVCandidate*textbar, OVService *srv);
    virtual void queryAndCompose(char *qstr, char *disp, 
        OVBuffer *buf, OVCandidate *textbar, OVService *srv);

    OVIMPOJ *parent;
    POJKeySeq seq;
    VXCandidate candi;
    VXCIN *cintab;
    NSMutableArray *list;
};


class OVIMPOJ : public OVInputMethod
{
public:
    OVIMPOJ();
    virtual const char* identifier();
    virtual const char* localizedName(const char *locale);
    virtual int initialize(OVDictionary*, OVService*, const char*);
    virtual void update(OVDictionary*, OVService*);  
    virtual OVInputMethodContext *newContext();

    virtual int isAsciiOutput() { return asciioutput; }
    virtual int isBeep() { return beep; }
    virtual int getKeyLayout() { return keylayout; }
    virtual int isFullPOJ() { return fullPOJOutput; }
    
protected:
    int fullPOJOutput;
    int asciioutput;
    int beep;
    int keylayout;
    VXCIN *cintab;
};


#endif
