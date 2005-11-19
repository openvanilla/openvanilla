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
	OVKPPhraseTool(SQLite3* db, char* last) { phdb_ = db; last_ = last; }
    // we "override" this module and changes its module type    
	virtual const char *moduleType();
    virtual const char *identifier();
    virtual const char *localizedName(const char* locale);
    
    virtual OVInputMethodContext* newContext();
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv,
    						const char *modulePath);
    virtual void update(OVDictionary *moduleCfg, OVService *srv);

    char getActivateKey();

	virtual SQLite3* getDb() { return phdb_; }
	virtual char* getLast() { return last_; }
	
protected:
    char wk;
    SQLite3* phdb_;
    char* last_;
};

class OVKPPhraseToolContext : public OVInputMethodContext {    
public:
    OVKPPhraseToolContext(OVKPPhraseTool *p) : parent(p), working(false) {}
    virtual void start(OVBuffer*, OVCandidate*, OVService*);
    virtual void clear();
    
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);

protected:
    OVKPPhraseTool *parent;
    bool working;
    OVKeySequenceSimple seq;
};

class OVOFPhraseCatcher : public OVOutputFilter {
public:
	OVOFPhraseCatcher(char* last) { last_ = last; }
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
							const char *modulePath);

    virtual const char *identifier();
    virtual const char *localizedName(const char* locale);
    
    virtual const char *process (const char *src, OVService *srv);

protected:
    bool display;
    char* last_;
};

#endif