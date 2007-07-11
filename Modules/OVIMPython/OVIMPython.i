%module(directors="1") OVIMPython
%{
#include "../../Framework/Headers/OpenVanilla.h"
%}
%feature("director");  

// A pure base class that defines the virtual destructor
class OVBase {
public:
    virtual ~OVBase() {}
};

// Abstract interface that accesses key code and key states (e.g. if SHIFT is
// hit at the time the key code is sent)
class OVKeyCode : public OVBase  {
public:
    virtual int code()=0;
    virtual int isShift()=0;
    virtual int isCapslock()=0;    
    virtual int isCtrl()=0;
    virtual int isAlt()=0;
    virtual int isOpt() { return isAlt(); }
    virtual int isCommand() { return isAlt(); }
    virtual int isNum()=0;
    virtual int isFunctionKey() {
        return isCtrl() || isAlt() || isOpt() || isCommand() || isNum();
    }
};

// Abstract interface for the pre-edit and composing buffer.
class OVBuffer : public OVBase {
public:
    virtual OVBuffer* clear()=0;
    virtual OVBuffer* append(const char *s)=0;
    virtual OVBuffer* send()=0;
    virtual OVBuffer* update()=0;
    virtual OVBuffer* update(int cursorPos, int markFrom=-1, int markTo=-1)=0;
    virtual int isEmpty()=0;
};

class OVCandidate : public OVBase  {
public:
    virtual OVCandidate* clear()=0;
    virtual OVCandidate* append(const char *s)=0;
    virtual OVCandidate* hide()=0;
    virtual OVCandidate* show()=0;
    virtual OVCandidate* update()=0;
    virtual int onScreen()=0;
};

class OVDictionary : public OVBase {
public:
    virtual int keyExist(const char *key)=0;
    virtual int getInteger(const char *key)=0;
    virtual int setInteger(const char *key, int value)=0;
    virtual const char* getString(const char *key)=0;
    virtual const char* setString(const char *key, const char *value)=0;
    virtual int getIntegerWithDefault(const char *key, int value) {
        if (!keyExist(key)) setInteger(key, value); return getInteger(key);
    }
    virtual const char* getStringWithDefault(const char *key, const char *value) 
    {
        if (!keyExist(key)) setString(key, value); return getString(key);
    }
};

class OVService : public OVBase {
public:
    virtual void beep()=0;
    virtual void notify(const char *msg)=0;    
    virtual const char *locale()=0;
    virtual const char *userSpacePath(const char *modid)=0;
    virtual const char *pathSeparator()=0;

    // limited support for encoding conversion
    virtual const char *toUTF8(const char *encoding, const char *src)=0;
    virtual const char *fromUTF8(const char *encoding, const char *src)=0;
    
    // the endian order of these two functions is always that of machine order
    // (ie. on a big endian machine it's UTF16BE, otherwise UTF16LE)
    // no byte order mark (0xfffe or 0xfeff) is ever used here) 
    // also note no boundry check mechanism exists, the Loader takes care 
    // about it
    virtual const char *UTF16ToUTF8(unsigned short *src, int len)=0;
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr)=0;
};

class OVInputMethodContext : public OVBase  {
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*) {}
    virtual void clear() {}
    virtual void end() {}
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*)=0;
};

class OVModule : public OVBase {
public:
    virtual const char *moduleType()=0;
    virtual const char *identifier()=0;
    virtual const char *localizedName(const char* locale) { return identifier(); }
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) { return 1; }
    virtual void update(OVDictionary *moduleCfg, OVService *srv) {}
};

class OVInputMethod : public OVModule
{
public:
    virtual OVInputMethodContext* newContext()=0; //the context object is assigned within c++ wrapper
    virtual const char* moduleType() { return "OVInputMethod"; }
};

class OVOutputFilter : public OVModule
{
public:
    virtual const char* process(const char *source, OVService *srv)=0;
    virtual const char* moduleType() { return "OVOutputFilter"; }
};

enum 
{
    ovkEsc=27, ovkSpace=32, ovkReturn=13, 
    ovkDelete=127, ovkBackspace=8,
    ovkUp=30, ovkDown=31, ovkLeft=28, ovkRight=29,
    ovkHome=1, ovkEnd=4, ovkPageUp=11, ovkPageDown=12,
    ovkTab=9
};

