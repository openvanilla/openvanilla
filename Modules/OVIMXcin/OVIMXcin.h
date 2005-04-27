// OVIMXcin.h

#ifndef __OVIMXcin_h
#define __OVIMXcin_h

//#include <Cocoa/Cocoa.h>                // to be removed after de-Cocoa-ized
#include <OpenVanilla/OpenVanilla.h>
#include "OVCIN.h"
#include "OVKeySequence.h"
#include "OVCandidateList.h"
#include <string>

using namespace std;

class XcinKeySequence : public OVKeySequenceSimple
{
public:
    XcinKeySequence(OVCIN* cintab);
    virtual int length() { return len; }
    virtual int add(char c);
    virtual int valid(char c);
    virtual string* compose(string* s);
    virtual char* getSeq() { return seq; }
    
protected:
    OVCIN* cinTable;
};

class OVIMXcin;

class OVXcinContext : public OVInputMethodContext
{
public:
    OVXcinContext(OVIMXcin* p, OVCIN* tab) : parent(p), keyseq(tab), cintab(tab),
        autocomposing(0) {}
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf,
        OVCandidate *textbar, OVService *srv);

protected:
    virtual void updateDisplay(OVBuffer *buf);
    virtual int compose(OVBuffer *buf, OVCandidate *textbar, OVService *srv);

    virtual int candidateEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar,
        OVService *srv);
        
    virtual void cancelAutoCompose(OVCandidate *textbar);
    
    OVIMXcin* parent;
    XcinKeySequence keyseq;
    OVCandidateList candi;
    OVCIN* cintab;
    
    int autocomposing;
    
    vector<string> candidateStringVector;
};

class OVIMXcin : public OVInputMethod
{
public:
    OVIMXcin(char *lpath, char *cfile, char *en=NULL, char *cn=NULL);
    virtual ~OVIMXcin();
    virtual const char* identifier();
    virtual const char* localizedName(const char* locale);
    virtual int initialize(OVDictionary*, OVService*, const char*);
    //virtual int update(OVDictionary*, OVDictionary*);
    virtual OVInputMethodContext *newContext();
    
    virtual int maxSeqLen() { return cfgMaxSeqLen; }
    virtual int isBeep() { return cfgBeep; }
    virtual int isAutoCompose() { return cfgAutoCompose; }
    virtual int isHitMaxAndCompose() { return cfgHitMaxAndCompose; }
    virtual bool isShiftSelKey() { return doShiftSelKey; };
    
protected:
    char loadpath[PATH_MAX];
    char cinfile[PATH_MAX];
    OVCIN* cintab;
    
    char ename[64];
    char cname[64];
    
    int cfgMaxSeqLen;
    int cfgBeep;
    int cfgAutoCompose;
    int cfgHitMaxAndCompose;
    
    bool doShiftSelKey;
};

#endif