#ifndef OVKPPHRASE_H
#define OVKPPHRASE_H

#define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include "OVSQLite3.h"
#include "OVKeySequence.h"

class OVKPPhraseToolContext;

class OVKPPhraseTool : public OVInputMethod {
public:
    // we "override" this module and changes its module type    
	virtual const char *moduleType() { return "OVKeyPreprocessor"; }
    virtual const char *identifier() { return "OVKPPhraseTool"; }
    virtual const char *localizedName(const char* locale) {
        if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN")) return "詞彙管理工具";
        return "Phrase tools";
    }
    
    virtual OVInputMethodContext* newContext();
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, const char *modulePath);
    virtual void update(OVDictionary *moduleCfg, OVService *srv);

    char getActivateKey() { return wk; }

protected:
    char wk;
};

class OVKPPhraseToolContext : public OVInputMethodContext {    
public:
    OVKPPhraseToolContext(OVKPPhraseTool *p) : parent(p), working(false) {}
    virtual void start(OVBuffer*, OVCandidate*, OVService*) { clear(); }
    virtual void clear() { working=false; seq.clear(); }
    
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);

protected:
    OVKPPhraseTool *parent;
    bool working;
    OVKeySequenceSimple seq;
};

class OVOFPhraseCatcher : public OVOutputFilter {
public:
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
        display=moduleCfg->getIntegerWithDefault("displayCaughtPhrase", 0);
        return true;
    }

    virtual const char *identifier() { return "OVOFPhraseCatcher"; }
    virtual const char *localizedName(const char* locale) {
        if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN")) return "詞彙攔截模組";
        return "Phrase catcher";
    }
    
    virtual const char *process (const char *src, OVService *srv);

protected:
    bool display;
};

OVInputMethodContext* OVKPPhraseTool::newContext() {
    return new OVKPPhraseToolContext(this);
}

// OpenVanilla Library Wrappers

extern "C" unsigned int OVGetLibraryVersion() { return OV_VERSION; }

extern "C" int OVInitializeLibrary(OVService* s, const char* mp);

extern "C" OVModule *OVGetModuleFromLibrary(int idx);

#endif
