// OVOVSpeak.cpp: The Tibetan Input Method
// 2004-2006 by Weizhong Yang 
//
// http://openvanilla.org
//
// This program is Zonble-ware and adopts Zonble's License
// -- "How could a person who doesn't even believe in law adopt any license?"

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
            moduleCfg->setString("Voice", "Default");
	    if(!moduleCfg->keyExist("Voice")) moduleCfg->setString("Voice", "Default");
		     
	    cfgVoice = moduleCfg->getString("Voice");
	    return 1;
    }
    
    virtual const char *localizedName(const char *locale)
	{
		if (!strcasecmp(locale, "zh_TW")) return "系統語音模組";
		if (!strcasecmp(locale, "zh_CN")) return "系统语音模组";
		return "Speak as you type";
	}
    
    virtual const char *identifier() { return "OVOFSpeak"; }
    virtual const char *process (const char *src, OVService *srv) {
	char command[4096], v[256]; 
	/*
	if(strcmp(cfgVoice, "Default")) {
		sprintf(v, " -v %s ", cfgVoice);
	} */
	sprintf(command, "/usr/bin/say %s %s &" , v, src);
	system(command);
        return src;
    }
    
protected:
    char *buf;
    char *command;
    const char *cfgVoice;
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

