// DummyLoader.h: the SCIM-OV Bridge

#ifndef __DummyLoader_h
#define __DummyLoader_h

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include OV_HEADER
#include "DummyDictionary.h"

using namespace scim;
class DummyBuffer;
class DummyCandidate;

class DIMEInstance : public IMEngineInstanceBase
{
public:
   DIMEInstance(IMEngineFactoryBase *factory, const String& encoding, 
     int id=-1) : IMEngineInstanceBase(factory, encoding, id) {}
    virtual ~DIMEInstance() {}

    // for compatiblity with scim-1.0
    // void move_preedit_caret(unsigned int){}
    void trigger_property (const String &property){}

protected:	
    friend class DummyBuffer;
    friend class DummyCandidate;
};


class DummyKeyCode : public OVKeyCode  {
public:
    DummyKeyCode (int p=0)          { chr=p; shift=capslock=ctrl=alt=0; }
    virtual int code()              { return chr; }
    virtual int isShift()           { return shift; }
    virtual int isCapslock()        { return capslock; }
    virtual int isCtrl()            { return ctrl; }
    virtual int isAlt()             { return alt; }
    virtual int isOpt()             { return alt; }
    virtual int isNum()             { return 0; }
    
    virtual void setCode(int x)     { chr=x; }
    virtual void setShift(int x)    { shift=x; }
    virtual void setCapslock(int x) { capslock=x; }
    virtual void setCtrl(int x)     { ctrl=x; }
    virtual void setAlt(int x)      { alt=x; }
protected:
    int chr;
    int shift, capslock, ctrl, alt;
};

// Abstract interface for the pre-edit and composing buffer.
class DummyBuffer : public OVBuffer {
public:
    DummyBuffer(DIMEInstance *i) {
        im=i;
        strcpy(buf, "");
    }
    virtual OVBuffer* clear() {
        strcpy(buf, "");
        im->update_preedit_string(WideString());
        im->hide_preedit_string();
        return this;
    }
    virtual OVBuffer* append(const char *s) {
        strcat(buf, s);
        return this;
    }
    virtual OVBuffer* send() {
        WideString bs=utf8_mbstowcs(buf);
        clear();
        im->commit_string(bs);
        return this;
    }
    virtual OVBuffer* update() {
        im->update_preedit_string(utf8_mbstowcs(buf));
        if (strlen(buf)) im->show_preedit_string();
        else im->hide_preedit_string();     
        return this;
    }
    virtual OVBuffer* update(int cursorPos, int markFrom=-1, int markTo=-1) {
        update();
        im->update_preedit_caret(cursorPos);
        return this;
    }
    virtual int isEmpty() {
        if (!strlen(buf)) return 1;
        return 0;
    }

protected:
    char buf[512];
    DIMEInstance *im;
};

class DummyCandidate : public OVCandidate  {
public:
    DummyCandidate(DIMEInstance *i) {
        im=i;
        onscreen=0;
        strcpy(buf, "");
    }
    
    virtual OVCandidate* clear() {
        strcpy(buf, "");
        return this;
    }
    virtual OVCandidate* append(const char *s) {
        strcat(buf, s);
        return this;
    }
    virtual OVCandidate* hide() {
        if (onscreen) {
            im->hide_aux_string();
            onscreen=0;
        }
        return this;
    }
    virtual OVCandidate* show() {
        if (!onscreen) {
            im->show_aux_string();
            onscreen=1;
        }
        return this;
    }
    virtual OVCandidate* update() {
        im->update_aux_string(utf8_mbstowcs(buf));
        return this;
    }
    virtual int onScreen() {
        return onscreen;
    }

protected:
    DIMEInstance *im;
    char buf[512];
    int onscreen;
};


// we borrow this function from OVPhoneticLib.cpp to help us do UTF-16->UTF-8
const char *VPUTF16ToUTF8(unsigned short *s, int l);

class DummyService : public OVService {
public:
    virtual void beep() {}
    virtual void notify(const char *msg) { fprintf(stderr, "%s\n", msg); }
    virtual const char *locale() { return "zh_TW"; }
    virtual const char *userSpacePath(const char *modid) { return "/tmp"; }
    virtual const char *pathSeparator() { return "/"; }
    virtual const char *toUTF8(const char *encoding, const char *src) 
        { return src; }
    virtual const char *fromUTF8(const char *encoding, const char *src)
        { return src; }
    virtual const char *UTF16ToUTF8(unsigned short *src, int len) {
        return VPUTF16ToUTF8(src, len);
    }
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr) {
        return 0;
    }
};

#endif

