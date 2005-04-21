#define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

class OVOFBasicFilter : public OVOutputFilter {
public:
    OVOFBasicFilter() {
        murmur("basic filter module object newed");
        buf=NULL;
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
            murmur ("basic filter init: module path=%s", modulePath);
            moduleCfg->setInteger("hello", 100);
        return 1;
    }

    
    virtual const char *identifier() { return "OVOFBasicFilter"; }
    virtual const char *process (const char *src, OVService *srv) {
        murmur ("processing!");
        if (buf) free(buf);
        buf=(char*)calloc(1, strlen(src)+1);
        strcpy(buf, src);
        char *s=buf;
        while (*s) {
            if (*s=='a') *s='A';
            if (*s=='b') *s='B';

            s++;
        }
        return buf;
    }
    
protected:
    char *buf;
};

// OV_SINGLE_MODULE_WRAPPER(OVOFBasicFilter);

extern "C" unsigned int OVGetLibraryVersion() {
    murmur("ovversion");
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService* s, const char* p) { 
    murmur("library init, locale=%s, path=%s, userspace=%s, seperator=%s", s->locale(), p, s->userSpacePath("OVOFBasicFilter"), s->pathSeparator());
    return 1; 
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    murmur ("get module");
    return (idx==0) ? new OVOFBasicFilter : NULL;
}

