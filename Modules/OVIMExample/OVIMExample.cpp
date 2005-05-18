// IMExample.cpp: a simple compose-and-commit input method
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

#define OV_DEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <ctype.h>
#include <stdio.h>

class IMExampleContext : public OVInputMethodContext
{
public:
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv)
    {
        return 0;
        murmur("IMExample: key event!");
        if (isprint(k->code()))
        {
            char str[2];
            sprintf(str, "%c", k->code());
            b->append(str)->update();
            srv->notify(str);
            i->show();
            i->append(str)->update();
            return 1;
        }
        
        if (k->code()==13) {
            if (!b->isEmpty()) {
                b->send();
                i->hide();
                return 1;
            }
            return 0;
        }
        
        if (!b->isEmpty()) {
            b->send();
            i->hide();
        }
                
        return 0;
    }
};

class IMExample : public OVInputMethod
{
public:
    virtual int initialize(OVDictionary *d, OVService*, const char *mp)
    {
        murmur("IMExample::init, modPath=%s", mp);
        murmur("setting 1=%d",
            d->getIntegerWithDefault("key1", 1));
        murmur("setting 2=%d",
            d->getIntegerWithDefault("key2", 2));
        murmur("setting 3=%s",
            d->getStringWithDefault("key3", "test"));
        
        return 1;
    }
    virtual const char* identifier() { return "OVIMExample-simple"; }
    virtual OVInputMethodContext *newContext() { return new IMExampleContext; }
};

class IMExample;
OV_SINGLE_MODULE_WRAPPER(IMExample);

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
