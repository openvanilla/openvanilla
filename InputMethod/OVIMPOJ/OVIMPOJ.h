// OVIMPOJ.h: prototype for POJ IM

// Copyright (c) 2004 by Lukhnos D. Liu, Kang-ming Liu, and Pek-tiong Tan

#ifndef __OVIMPOJ_h
#define __OVIMPOJ_h

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLoadable.h>
#include <OpenVanilla/OVUtility.h>
#include "../OVIMXcin/VXCIN.h"
#include "../OVIMXcin/VXCandidate.h"
#include "../../Loaders/OSX/VXUtility.h"
#include "POJKeySeq.h"
#include "../OVIMXcin/VXCandidate.h"

class OVIMPOJ;

class OVIMPOJContext : public OVIMContext
{
public:
    OVIMPOJContext(OVIMPOJ *p, VXCIN *ctab);
    ~OVIMPOJContext();
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, 
        OVService *srv);
    
protected:
    virtual int candidateEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, 
        OVService *srv);
    void OVIMPOJContext::queryAndCompose(char *qstr, char *disp, OVBuffer *buf, 
        OVTextBar *textbar, OVService *srv);

    OVIMPOJ *parent;
    POJKeySeq seq;
    VXCandidate candi;
    VXCIN *cintab;
    NSMutableArray *list;
};


class OVIMPOJ : public OVInputMethod
{
public:
    OVIMPOJ() : asciioutput(0), beep(0), keylayout(pojToneByNumber), cintab(NULL) {}
    virtual int identifier(char* s);
    virtual int name(char *locale, void *s, OVEncoding *enc);
    virtual int initialize(OVDictionary*, OVDictionary*, OVService*, char*);
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*);
    virtual int update(OVDictionary*, OVDictionary* localconfig);
    virtual OVIMContext *newContext();

    virtual int isAsciiOutput() { return asciioutput; }
    virtual int isBeep() { return beep; }
    virtual int getKeyLayout() { return keylayout; }
    
protected:
    int asciioutput;
    int keylayout;
    int beep;
    VXCIN *cintab;
};


#endif
