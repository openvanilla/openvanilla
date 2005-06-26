#define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

class OVOFSpeak : public OVOutputFilter {
public:
    OVOFSpeak() {
        murmur("basic filter module object newed");
        buf=NULL;
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
            murmur ("basic filter init: module path=%s", modulePath);
            moduleCfg->setInteger("hello", 100);
        return 1;
    }

    
    virtual const char *identifier() { return "OVOFSpeak"; }
    virtual const char *process (const char *src, OVService *srv) {
	char command[4096];
	sprintf(command, "/usr/bin/say %s &" , src);
	system(command);
        return src;
    }
    
protected:
    char *buf;
	char *command;
};

// OV_SINGLE_MODULE_WRAPPER(OVOFSpeak);

extern "C" unsigned int OVGetLibraryVersion() {
    murmur("ovversion");
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService* s, const char* p) { 
    murmur("library init, locale=%s, path=%s, userspace=%s, seperator=%s", s->locale(), p, s->userSpacePath("OVOFSpeak"), s->pathSeparator());
    return 1; 
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    murmur ("get module");
    return (idx==0) ? new OVOFSpeak : NULL;
}

