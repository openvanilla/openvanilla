// OVIMezbasic.cpp

#define OVDEBUG
#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"
#include "OpenVanilla/OVUtility.h"

extern "C" char *EvalEZBasic(char *inbuf);

class OVezbasicIM;

const int ebMaxKeySeq=8192;
class KeySeq
{
public:
    KeySeq() { len=0; buf[0]=0; }
    void add(char c)
    {
        if (len == ebMaxKeySeq) return;
        buf[len++]=c;
        buf[len]=0;
    }
    void remove()
    {
        if (!len) return;
        buf[--len]=0;
    }
    void clear() { len=0; buf[0]=0; }
    char buf[ebMaxKeySeq];
    int len;
};

class OVezbasicContext : public OVIMContext
{
public:
    OVezbasicContext(OVezbasicIM *p) : parent(p) {}
    virtual ~OVezbasicContext() {}
    virtual int activate(OVService *) { return 1; }
    virtual int deactivate(OVService *) { return 1; }
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv)
    {        
        if (key->isCode(2, ovkReturn, ovkMacEnter))
        {
            if (!buf->length()) return 0;
            char *result=EvalEZBasic(keyseq.buf);
            buf->clear()->append(result)->send();
            keyseq.clear();
            return 1;
        }

        if (key->isCode(2, ovkBackspace, ovkDelete))
        {
            if (!buf->length()) return 0;
            keyseq.remove();
            buf->clear()->append(keyseq.buf)->update();
            return 1;
        }

        if (!key->isPrintable() || key->isOpt() || key->isCtrl()
            || key->isCommand())
        {
            keyseq.clear();
            if (buf->length()) buf->clear();
            return 0;            
        }
        
        if (key->isPrintable())
        {
            keyseq.add(key->code());
            buf->clear()->append(keyseq.buf)->update();
            return 1;
        }

        keyseq.clear();
        if (buf->length()) buf->clear();
        
        return 0;
    }
    
protected:
    OVezbasicIM *parent;
    KeySeq keyseq;
};

class OVezbasicIM : public OVInputMethod
{
public:
    OVezbasicIM() {}
    virtual ~OVezbasicIM() {}
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVezbasicIM"));
    }    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        return strlen(strcpy((char*)s, "OV ezbasic"));
    }

    virtual int initialize(OVDictionary*, OVDictionary*, OVService*, char*)
    {
        return 1;
    }
    
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
    {
        return 1;
    }

    virtual int update(OVDictionary*, OVDictionary*)
    {
        return 1;
    }

    virtual OVIMContext* newContext() 
    {
        return new OVezbasicContext(this); 
    }
    
    virtual void deleteContext(OVIMContext* s) 
    {
        delete s; 
    }
};


// use these two wrappers
OVLOADABLEWRAPPER(OVezbasicIM);
OVLOADABLECANUNLOAD;
