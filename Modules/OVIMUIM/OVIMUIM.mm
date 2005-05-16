// OVIMUIM.cpp
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

#define OV_DEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <Cocoa/Cocoa.h>
#include <UIM/uim.h>

class OUEncodingConvertor {
public:
    OUEncodingConvertor(NSStringEncoding e) { tocode=e; }
    char *convert(const char *fromstr) {
        NSString *s=[NSString stringWithCString:fromstr encoding:tocode];
        const char *u8=[s UTF8String];
        char *rs=(char*)calloc(1, strlen(u8)+1);
        strcpy(rs, u8);
        return rs;
    }
protected:
    NSStringEncoding tocode;
};

int OUEncodingIsConvertible(const char *to, const char *from) {
    return 1;
}

void *OUEncodingCreate(const char *to, const char *from) {
    return new OUEncodingConvertor(CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingEUC_JP));
}

char *OUEncodingConvert(void *obj, const char *str) {
    return ((OUEncodingConvertor*)obj)->convert(str);
}

void OUEncodingRelease(void *obj) {
    delete (OUEncodingConvertor*)obj;
}



class OVIMUIMContext : public OVInputMethodContext
{
public:
    OVIMUIMContext() {
        ovbuf=NULL;
        ovcandi=NULL;
        ovsrv=NULL;
    }
    
    void setUIMContext(uim_context c) {
        uc = c;
    }
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* c, OVService* srv) {
        murmur("keyEvent, keycode=%d", k->code());
        
        // retain objects for the uim callbacks
        ovbuf=b;
        ovcandi=c;
        ovsrv=srv;
        
        int keycode=k->code();
        if (k->isFunctionKey()) return 0;

        // convert keycode to UIM keycode
        switch (k->code()) {
            case ovkReturn: 
                murmur("return!");
                keycode=UKey_Return; break;
        }

        int r=uim_press_key(uc, keycode, 0);
        uim_release_key(uc, keycode, 0);
        
        if (!r) {
            murmur("uim consumed the key!");
            return 1;
        }
        return 0;
    }
    
// UIM callbacks
    void uimClear() {
        murmur("uimClear");
        murmur("ovbuf=%x", ovbuf);

        if (ovbuf) ovbuf->clear()->update();
    }
    
    void uimPush(int state, const char *s) {
        murmur("uimPush");
        if (s)
            murmur("uimPush, state=%d, string=%d, strlen=%s", state, strlen(s), s);
            
        if (state==4) return;
        if (!state) return;
        if (ovbuf) ovbuf->append(s)->update();
    }
    
    void uimUpdate() {
        murmur("uimUpdate");
        if (ovbuf) ovbuf->update();
    }
    void uimCommit(const char *s) {
        murmur("uimCommit, string=%s", s);
        if (ovbuf) ovbuf->clear()->append(s)->send();
    }

protected:
    OVBuffer *ovbuf;
    OVCandidate *ovcandi;
    OVService *ovsrv;
    uim_context uc;
};


void OUPreeditClear(void *ptr) {
    if (ptr) {
        murmur("PTR not null! %x", ptr);
        ((OVIMUIMContext*)ptr)->uimClear();
    }
    else murmur ("PTR null!");
}

void OUPreeditPush(void *ptr, int state, const char *s) {
    ((OVIMUIMContext*)ptr)->uimPush(state, s);
}

void OUPreeditUpdate(void *ptr) {
    ((OVIMUIMContext*)ptr)->uimUpdate(); 
}

void OUPreeditCommit(void *ptr, const char *s) {
    ((OVIMUIMContext*)ptr)->uimCommit(s);
}


class OVIMUIM : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMUIM"; }
    virtual OVInputMethodContext *newContext() {
        OVIMUIMContext *ovc=new OVIMUIMContext;
        murmur("context addr=%x", ovc);
        uim_context uc;

        // create UIM-anthy
        murmur("uim context create");
        uc=uim_create_context(ovc, "UTF-8", "ja", "anthy", &enccvtr, OUPreeditCommit);
        if (uc) {
            murmur("set context");
            ovc->setUIMContext(uc);
            murmur("after set");
        }
        else return NULL;
        
        uim_set_preedit_cb(uc, OUPreeditClear, OUPreeditPush, OUPreeditUpdate);

        // mode 1 is anthy's Hiragana mode
        uim_set_mode(uc, 1);

        return ovc;
    }
    virtual int initialize(OVDictionary *, OVService*, const char *mp) {    
        // THIS IS ACTUALLY NOT RIGHT, BUT WE PRESUME OVIMUIM::INIT WILL ONLY
        // BE CALLED ONCE

        murmur("OVUIM init");
        // setup UIM code convertor (currently only EUC_JP -> UTF8)        
        enccvtr.is_convertible=OUEncodingIsConvertible;
        enccvtr.create=OUEncodingCreate;
        enccvtr.convert=OUEncodingConvert;
        enccvtr.release=OUEncodingRelease;
        
        // setup UIM
        uim_init();
        return 1;
    }
    virtual const char* localizedName(const char *locale) {
        return "UIM (anthy) - Hiragana";
    }
    
protected:
    struct uim_code_converter enccvtr;
};

OV_SINGLE_MODULE_WRAPPER(OVIMUIM);

