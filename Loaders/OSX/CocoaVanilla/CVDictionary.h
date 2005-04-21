// CVDictionary.h

#ifndef __CVDictionary_h
#define __CVDictionary_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>

class CVDictionary : public OVDictionary {
public:
    CVDictionary(NSMutableDictionary *dict);
    virtual ~CVDictionary();
    virtual int keyExist(const char *key);
    virtual int getInteger(const char *key);
    virtual int setInteger(const char *key, int value);
    virtual const char* getString(const char *key);
    virtual const char* setString(const char *key, const char *value);
    
protected:
    NSMutableDictionary *d;
};

#endif