#define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

class OVOFZhengnaeng : public OVOutputFilter {
public:
    OVOFZhengnaeng() {
        buf=NULL;
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
        return 1;
    }

    virtual const char *localizedName(const char *locale)
	{
		if (!strcasecmp(locale, "zh_TW")) return "發正念濾嘴";
		if (!strcasecmp(locale, "zh_CN")) return "发正念滤嘴";
		return "Release Zheng Naeng as You Type";
	}

    virtual const char *identifier() { return "OVOFZhengnaeng"; }
    virtual const char *process (const char *src, OVService *srv) {
		srv->notify("輸入大法好！");
		return src;
    }
    
protected:
    char *buf;
};

// OV_SINGLE_MODULE_WRAPPER(OVOFZhengnaeng);

extern "C" unsigned int OVGetLibraryVersion() {
    murmur("ovversion");
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService* s, const char* p) { 
    murmur("library init, locale=%s, path=%s, userspace=%s, seperator=%s", s->locale(), p, s->userSpacePath("OVOFZhengnaeng"), s->pathSeparator());
    return 1; 
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    murmur ("get module");
    return (idx==0) ? new OVOFZhengnaeng : NULL;
}

