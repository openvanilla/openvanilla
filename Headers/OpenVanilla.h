// OpenVanilla.h
// Copyright (c) 2004 The OpenVanilla Team (http://openvanilla.org)


#ifndef __OpenVanilla_h
#define __OpenVanilla_h

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

const unsigned int ovVersion=0x00070000;    // OpenVanilla 0.7.0

class OVObject {
public:
    virtual ~OVObject() {}
};

class OVKeyCode : public OVObject
{
public:
    virtual int code()=0;    
    virtual int isShift()=0;
    virtual int isCapslock()=0;
    virtual int isCtrl()=0;
    virtual int isAlt()=0;
    virtual int isOpt()=0;
    virtual int isCommand()=0;

    virtual int isPrintable() { return isprint(code()); }
    virtual int isAlpha() { return isalpha(code()); }
    virtual int isNumeric() { return (code() >= '0' && code() <= '9'); }
    virtual int upper() { return toupper(code()); }
    virtual int lower() { return tolower(code()); }
    
    virtual int isKey(int c)
    {
        return (c == code());
    }
    
    virtual int isCode(int argc, ...)
    {
        int r=0;
        va_list ap;
        va_start(ap, argc);
        for (int i=0; i<argc; i++) 
            { int j=va_arg(ap, int); if (j==code()) { r=1; break; } }
        va_end(ap);
        return r;
    }
};

class OVBuffer : public OVObject {
public:
    virtual OVBuffer* clear()=0;
    virtual OVBuffer* send()=0;
    virtual OVBuffer* update()=0;
    virtual OVBuffer* update(int cursor=-1, int hilitefrom=-1, int hiliteto=-1)=0;
    virtual OVBuffer* append(const char *text)=0;
    virtual int isEmpty()=0;
        
    virtual OVBuffer* appendChar(char c)
        { char s[2]; s[0]=c; s[1]=0; return append(s); }
};
        
class OVInfoBox : public OVObject {
public:
    virtual OVInfoBox* setSelectKey(const char *key)=0;
    virtual OVInfoBox* show()=0;
    virtual OVInfoBox* hide()=0;
    virtual OVInfoBox* finalize()=0;
    virtual OVInfoBox* clear()=0;
    virtual OVInfoBox* append(const char *text)=0;
    virtual OVInfoBox* next()=0;
    virtual OVInfoBox* update()=0;        
    virtual int onScreen()=0;
};
    
class OVService : public OVObject {
public:
    virtual OVService* beep()=0;
};
    
class OVDictionary : public OVObject {
public:
    virtual int keyExist(const char *key)=0;
    virtual int setInt(const char *key, int value)=0;
    virtual int getInt(const char *key)=0;
    virtual const char* setString(const char *key, const char *value)=0;
    virtual char* getString(const char *key, char *buf, int maxLen=0)=0;
        
    virtual int getIntDefault(const char *key, int dfv) {
        if (keyExist(key)) return getInt(key);
        return setInt(key, dfv);
    }
    virtual char* getStringDefault(const char *key, char *buf, const char *dfv,
       int maxlen=0) {
        if (keyExist(key)) return getString(key, buf, maxlen);
        return setString(key, dfv);
    }
};

class OVIMContext : public OVObject {
public:
    virtual int activate(OVInfoBox *infobox) { return 1; }
    virtual int deactivate(OVBuffer *buf, OVInfoBox *infobox) { clear(); return 1; }
    virtual void clear() {}

    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, 
        OVInfoBox *infobox, OVService *srv) {
        clear();
        return 0;
    }        

    virtual int candidateEvent(OVKeyCode *key, OVBuffer *buf, 
        OVInfoBox *infobox, OVService *srv) {
        clear();
        return 0;
    }        
};

class OVComponent : public OVObject {
public:
    virtual const char *identifier()=0;
    virtual const char *localizedName(const char *locale) { return identifier(); }
    
    virtual int initialize(OVDictionary *modulePref, const char *modulePath, 
        const char *userPath, const char *seperator) { return 1; }
    virtual void terminate(OVDictionary *modulePref) { return 1; }
    virtual void update(OVDictionary *modulePref) { return 1; }
};
                           
class OVInputMethod : public OVComponent {
public:
    virtual OVIMContext *newConext()=0;
};

class OVBufferFilter : public OVComponent {
public:
    virtual const char *process(const char *source);
};




#endif
