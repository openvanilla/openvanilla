#ifndef __DummyLoader_h
#define __DummyLoader_h

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "OpenVanilla.h"

class DummyKeyCode : public OVKeyCode  {
public:
    DummyKeyCode (int p=0) : c(p) {}
    virtual int code() { return c; }
    virtual int isShift() { return 0; }
    virtual int isCapslock() { return 0; }
    virtual int isCtrl() { return 0; }
    virtual int isAlt()  { return 0; }
protected:
    int c;
};

// Abstract interface for the pre-edit and composing buffer.
class DummyBuffer : public OVBuffer {
public:
    DummyBuffer() {
        strcpy(buf, "");
        state=0;
    }
    virtual OVBuffer* clear() {
        state=0;
        strcpy(buf, "");
        return this;
    }
    virtual OVBuffer* append(const char *s) {
        strcat(buf, s);
        return this;
    }
    virtual OVBuffer* send() {
        state=1;
        return this;
    }
    virtual OVBuffer* update() {
        state=0;
        return this;
    }
    virtual OVBuffer* update(int cursorPos, int markFrom=-1, int markTo=-1) {
        return update();
    }
    virtual int isEmpty() {
        if (!strlen(buf)) return 1;
        return 0;
    }
    
    int state;
    char buf[512];
};

class DummyCandidate : public OVCandidate  {
public:
    DummyCandidate() {
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
        onscreen=0;
        return this;
    }
    virtual OVCandidate* show() {
        onscreen=1;
        return this;
    }
    virtual OVCandidate* update() {
        return this;
    }
    virtual int onScreen() {
        return onscreen;
    }


    char buf[512];
    int onscreen;
};

// Abstract interface for a simple dictionary. It is recommended that this
// dictionary be implemented as a type-free dictionary, i.e. you can replace
// the key with any value of any type, and type conversion between integer and
// string is done transparently, like what is done in e.g. sqlite3.
class DummyDictionary : public OVDictionary {
public:
    virtual int keyExist(const char *key) {
        return 0;
    }
    virtual int getInteger(const char *key) {
        return 0;
    }
    virtual int setInteger(const char *key, int value) {
        return value;
    }
    virtual const char* getString(const char *key) {
        return "123456789";
    }
    virtual const char* setString(const char *key, const char *value) {
        return "123456789";
    }
};

class DummyService : public OVService {
public:
    virtual void beep() {}
    virtual void notify(const char *msg) {}
    virtual const char *locale() { return "zh_TW"; }
    virtual const char *userSpacePath(const char *modid) { return "/tmp"; }
    virtual const char *pathSeparator() { return "/"; }
    virtual const char *toUTF8(const char *encoding, const char *src) 
        { return src; }
    virtual const char *fromUTF8(const char *encoding, const char *src)
        { return src; }
    virtual const char *UTF16ToUTF8(unsigned short *src, int len) {
        return "UTF16"; }
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr) {
        return 0;
    }
};


#endif

