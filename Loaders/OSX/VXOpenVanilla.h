// VXOpenVanilla.h

#ifndef __VXOpenVanilla_h
#define __VXOpenVanilla_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>
#include "VXUtility.h"

class VXDictionary : public OVDictionary
{
public:
    VXDictionary(NSMutableDictionary *dict) : d(dict) {}
    virtual int keyExist(const char *key)
    {
        return [d objectForKey: VXSTR(key)] ? 1 : 0;
    }
    
    virtual int getInteger(const char *key)
    {
        NSString *s;
        return (s=[d objectForKey: VXSTR(key)]) ? [s intValue] : 0;
    }
    
    virtual int setInteger(const char *key, int value)
    {
        [d setObject: [[NSString stringWithFormat: @"%d", value] autorelease]
            forKey: VXSTR(key)];
        return value;
    }
    
    virtual const char* getString(const char *key)
    {
        NSString *s;
        return (s=[d objectForKey: VXSTR(key)]) ? [s UTF8String] : "";
    }
    
    virtual const char* setString(const char *key, const char *value)
    {
        [d setObject: VXSTR(value) forKey: VXSTR(key)];
        return value;
    }
    
protected:
    NSMutableDictionary *d;
};

class VXService : public OVService
{
public:
    virtual void beep()
    {
        murmur ("VXService::beep() not yet implemented!");
    }
    
    virtual const char *toUTF8(const char *encoding, const void *src, int len=0)
    {
        murmur ("VXService not yet implemented!");
        return NULL;
    }
    
    virtual int toUTF8(const char *encoding, const char *src, char *dest,
        int maxdestlen=0)
    {
        murmur ("VXService not yet implemented!");
        return 0;
    }
    
    virtual int fromUTF8(const char *encoding, const char *src, void *dest,
        int maxdestlen=0)
    {
        murmur ("VXService not yet implemented!");
        return 0;
    }
    
    virtual void notify(const char *msg)
    {
        murmur ("VXService not yet implemented!");
    }
};


#endif

/*

class VXKeyCode : public OVKeyCode
{
public:
    VXKeyCode(char charcode, UInt32 modifiers) : c(charcode), m(modifiers) {}
    virtual int code()       { return c; }
    virtual int isShift()    { return m & (shiftKey | rightShiftKey); }
    virtual int isCtrl()     { return m & (controlKey | rightControlKey); }
    virtual int isAlt()      { return m & (optionKey | rightOptionKey); }
    virtual int isOpt()      { return isAlt(); }
    virtual int isCommand()  { return m & cmdKey; }
    virtual int isCapslock() { return m&(alphaLock|kEventKeyModifierNumLockMask); }
protected:
    char c;
    UInt32 m;
};

class VXBuffer : public OVBuffer
{
public:
    VXBuffer(TSBComposingBuffer *buf) : b(buf);
    virtual OVBuffer* clear() { b->clear(); return this; } 
    virtual OVBuffer* send() { b->send(); return this; }
    virtual OVBuffer* update() { b->update(FALSE); return this; }
    virtual OVBuffer* update(int cursorPos, int markFrom=-1, int markTo=-1)
        { b->update(FALSE, cursorPos, markFrom, markTo); return this; }
    virtual int isEmpty() { return b->isEmpty(); }
    virtual OVBuffer* append(const char *s)
    {
        NSString *ns=[[NSString alloc] initWithUTF8String: s];
        b->append((CFStringRef)ns);
        [ns release];
    }

protected:
    TSBComposingBuffer *b;
};

class VXInfoBox : public OVInfoBox 
{
public:
    virtual OVInfoBox* clear()  { [c clearContent]; return this; }
    virtual OVInfoBox* append(const char *s) { [c append: s]; return this; }
    virtual OVInfoBox* hide() { [w orderOut]; return this; }
    virtual OVInfoBox* show() { [w orderFront]; return this; }
    virtual OVInfoBox* update() { [c update]; return this; }
    virtual int onScreen() { return [w isOnScreen]; }
protected:
    CVInfoBoxController *c;
    CVInfoBoxWindow *w;
};

inline static NSString *VXSTR(const char *s)
{
    return [[NSString stringWithUTF8String: s] autorelease];
}

*/
