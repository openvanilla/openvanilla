// IMExample.cpp: a simple compose-and-commit input method
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

#define OVDEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <ctype.h>
#include <stdio.h>

class IMExampleContext : public OVInputMethodContext
{
public:
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVInfoBox* i, OVService*)
    {
        murmur("IMExample: key event!");
        if (isprint(k->code()))
        {
            char str[2];
            sprintf(str, "%c", k->code());
            b->append(str)->update();
            i->show();
            i->append(str)->update();
            return 1;
        }
        
        b->send();
        i->hide();
                
        return 0;
    }
};

class IMExample : public OVInputMethod
{
public:
    virtual int initialize(OVDictionary *, OVDictionary *, OVService*,
        const char *mp, const char *up, const char *s)
    {
        murmur("IMExample::init, modPath=%s, usrPath=%s, seperator=%s",
            mp, up, s);
        return 1;
    }
    virtual const char* identifier() { return "OVIMExample-simple"; }
    virtual OVInputMethodContext *newContext() { return new IMExampleContext; }
};

class IMExample;
// OVSINGLEMODULEWRAPPER(IMExample);

extern "C" unsigned int OVGetLibraryVersion() { return OVVERSION; } 
extern "C" int OVInitializeLibrary(OVDictionary*, OVService*, const char* p, 
        const char* s) 
{
    murmur("LIBINIT: path=%s, seperator=%s", p, s);
    return 1;
}
extern "C" OVModule *OVGetModuleFromLibrary(int idx)
{
    return (idx==0) ? new IMExample : NULL;
}


/*

const int maxComposingStringLength=36;

class ExampleComposingString
{
public:
    ExampleComposingString() { len=0; }
    void remove() { if (len) composeStr[--len]=0; }
    const char *getString() { return composeStr; }
    void add(char c)
    {
        if (len < maxComposingStringLength) composeStr[len++]=c;
        composeStr[len]=0;
    }
    
protected:
    int len;
    char composeStr[maxComposingStringLength];
};

class OVIMExampleContext : public OVInputMethodContext
{
public:
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv)
    {
        // if there's something in buffer, we commit
        if (key->code()==ovkReturn && buf->length())
        {
            buf->send();
            textbar->clear()->hide();
            return 1;
        }
        
        // process backspace
        if (key->code()==ovkBackspace && buf->length())
        {
            str.remove();
            buf->clear()->update();            
            if (strlen(str.getString()))
            {
                buf->append(str.getString())->update();
                textbar->clear()->append("composing: ")->append(str.getString())->
                    update()->show();
            }
            else
            {
                textbar->clear()->hide();
            }
            
            return 1;
        }
        
        // if it's printable character, we append both the pre-edit buffer
        // and textbar
        if (isprint(key->code()))
        {
            char b[2];
            sprintf(b, "%c", key->code());
            buf->clear()->append(b)->update();
            textbar->clear()->append("composing: ")->append(b)->update()->show();
            return 1;
        }
        
        // otherwise, we dump everything, clean up the buffer, hide the textbar
        // and return 0
        buf->send();
        textbar->clear()->hide();
        return 0;
    }
    
protected:
    ExampleComposingString str;
};

*/
