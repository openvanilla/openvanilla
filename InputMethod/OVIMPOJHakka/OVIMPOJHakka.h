// OVIMPOJHakka.h: prototype for POJHakka IM

// Copyright (c) 2004 by Lukhnos D. Liu, Kang-ming Liu, and Pek-tiong Tan

#ifndef __OVIMPOJHakka_h
#define __OVIMPOJHakka_h

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLoadable.h>
#include <OpenVanilla/OVUtility.h>
#include "VXCIN.h"
#include "VXCandidate.h"
#include "VXUtility.h"
#include "POJHakkaKeySeq.h"
#include "VXCandidate.h"

class OVIMPOJHakka;

class OVIMPOJHakkaContext : public OVIMContext
{
public:
    OVIMPOJHakkaContext(OVIMPOJHakka *p, VXCIN *ctab);
    ~OVIMPOJHakkaContext();
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, 
        OVService *srv);
    
protected:
    virtual int candidateEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, 
        OVService *srv);
    void OVIMPOJHakkaContext::queryAndCompose(char *qstr, char *disp, OVBuffer *buf, 
        OVTextBar *textbar, OVService *srv);

    OVIMPOJHakka *parent;
    POJHakkaKeySeq seq;
    VXCandidate candi;
    VXCIN *cintab;
    NSMutableArray *list;
};


class OVIMPOJHakka : public OVInputMethod
{
public:
    OVIMPOJHakka() : asciioutput(0), beep(0), keylayout(pojToneByNumber), cintab(NULL) {}
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
    int beep;
    int keylayout;
    VXCIN *cintab;
};


#endif
