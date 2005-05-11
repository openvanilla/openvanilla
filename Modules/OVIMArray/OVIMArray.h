#ifndef _OVIMARRAY_H
#define _OVIMARRAY_H

#include <OpenVanilla/OpenVanilla.h>
#include "OVCIN.h"

class OVIMArrayContext : public OVInputMethodContext
{
public:
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv);
};

class OVIMArray : public OVInputMethod
{
public:
    virtual int initialize(OVDictionary *, OVService*, const char *mp);
    virtual const char* identifier() { return "OVOVIMArray"; }
    virtual OVInputMethodContext *newContext() { return new OVIMArrayContext; }
private:
    OVCIN* cintab;
};

#endif
