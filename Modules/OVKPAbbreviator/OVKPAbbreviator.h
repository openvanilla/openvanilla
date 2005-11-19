#ifndef OVKPABBREVIATOR_H
#define OVKPABBREVIATOR_H

#include "OVKPPhrase.h"

class OVKPAbbreviatorContext;

class OVKPAbbreviator : public OVKPPhraseTool {
public:
	OVKPAbbreviator(SQLite3* setDb, char* setLast)
		: OVKPPhraseTool(setDb, setLast) {}

    virtual const char *identifier();
    virtual const char *localizedName(const char* locale);
    
    virtual OVInputMethodContext* newContext();
};

class OVKPAbbreviatorContext : public OVKPPhraseToolContext {    
public:
    OVKPAbbreviatorContext(OVKPAbbreviator *p) : OVKPPhraseToolContext(p) {}

    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);
};

class OVOFAbbreviationCatcher : public OVOFPhraseCatcher {
public:
	OVOFAbbreviationCatcher(char* setLast) : OVOFPhraseCatcher(setLast) {}
    virtual const char *identifier() { return "OVOFAbbreviationCatcher"; }
    virtual const char *localizedName(const char* locale) {
        if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN"))
        	return "縮寫攔截模組";
        return "Abbreviation catcher";
    }
    
    virtual const char *process (const char *src, OVService *srv);
};

#endif