// OVIMXcin.h

#ifndef __OVIMXcin_h
#define __OVIMXcin_h

//#include <Cocoa/Cocoa.h>                // to be removed after de-Cocoa-ized
#include <OpenVanilla/OpenVanilla.h>
#include "OVCIN.h"
#include "OVKeySequence.h"
#include "OVCandidate.h"
#include <string>
#include <vector>

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

class OVXcinContext : public OVIMContext
{
public:
    OVXcinContext(OVIMXcin* p, OVCIN* tab) : parent(p), keyseq(tab), cintab(tab),
        autocomposing(0) {}
    virtual int activate(OVService *) { return 1; }
    virtual int deactivate(OVService *) { return 1; }
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf,
        OVTextBar *textbar, OVService *srv);

protected:
    virtual void updateDisplay(OVBuffer *buf);
    virtual int compose(OVBuffer *buf, OVTextBar *textbar, OVService *srv);

    virtual int candidateEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv);
        
    virtual void cancelAutoCompose(OVTextBar *textbar);
    
    OVIMXcin* parent;
    XcinKeySequence keyseq;
    OVCandidate candi;
    OVCIN* cintab;
    
    int autocomposing;
	
	vector<string> candidateStringVector;
};

class OVIMXcin : public OVInputMethod
{
public:
    OVIMXcin(char *lpath, char *cfile, char *en=NULL, char *cn=NULL, 
        OVEncoding enc=ovEncodingUTF8);
    virtual ~OVIMXcin();    
    virtual int identifier(char *s);
    virtual int name(char *locale, void *s, OVEncoding *enc);
    virtual int initialize(OVDictionary*, OVDictionary*, OVService*, char* path);
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*);
    virtual int update(OVDictionary*, OVDictionary*);
    virtual OVIMContext *newContext();
    
    virtual int maxSeqLen() { return cfgMaxSeqLen; }
    virtual int isBeep() { return cfgBeep; }
    virtual int isAutoCompose() { return cfgAutoCompose; }
    virtual int isHitMaxAndCompose() { return cfgHitMaxAndCompose; }
    
protected:
    char loadpath[PATH_MAX];
    char cinfile[PATH_MAX];
    OVCIN* cintab;
    
    char ename[64];
    char cname[64];
    OVEncoding cnameencoding;
    
    int cfgMaxSeqLen;
    int cfgBeep;
    int cfgAutoCompose;
    int cfgHitMaxAndCompose;
};

#endif