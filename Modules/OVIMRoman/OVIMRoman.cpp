// OVIMRoman.cpp
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

#define OV_DEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

class OVIMRomanContext : public OVInputMethodContext
{
public:
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv) {
        if (!isprint(k->code()) || k->isFunctionKey()) return 0;
        
        // we only send printable characters so that they can pass thru 
        // full-width character filter (if enabled)
        char s[2];
        sprintf(s, "%c", k->code());
        b->append(s)->send();
        return 1;
    }
};

class OVIMRoman : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMRoman"; }
    virtual OVInputMethodContext *newContext() { return new OVIMRomanContext; }
    virtual int initialize(OVDictionary *, OVService*, const char *mp) {
        return 1;
    }
    virtual const char* localizedName(const char *locale) {
        if (!strcasecmp(locale, "zh_TW")) return "英數字輸入";
        return "Roman (alphanumeric)";
    }
};

OV_SINGLE_MODULE_WRAPPER(OVIMRoman);

