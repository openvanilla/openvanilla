// VXCIN.h

#ifndef __VXCIN_h
#define __VXCIN_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>

class VXCIN
{
public:
    VXCIN();
    ~VXCIN();
    void read(char *fname, OVEncoding enc=ovEncodingUTF8, int shiftselkey=0);
    NSMutableArray* find(char *key);
    
protected:
    NSMutableDictionary* keytable;
    NSMutableDictionary* chartable;
    char selkey[32];
    char ename[256];
    char cname[256];
};

#endif