// OVIMAutoPhonetic.cpp
#define OV_DEBUG

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <sys/syslimits.h>
#include "OVOSDef.h"
#include "OVSQLite3.h"
#include "AutoPhonetic.h"

class OVIMAutoPhonetic;

class OVIMAutoPhoneticContext : public OVInputMethodContext {
public:
    OVIMAutoPhoneticContext(OVIMAutoPhonetic *p);
    ~OVIMAutoPhoneticContext();
    virtual void start(OVBuffer*, OVCandidate*, OVService*);
    virtual void clear();
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);
    
protected:
    int processEvent(const TEvent &e);
    
    int updateCandidateWindow();
    int closeCandidateWindow();
    int candidateEvent();
    int candidatePageUp();
    int candidatePageDown();

    PhoneticSyllable ps;
    OVKeyCode *k;
    OVBuffer *b;
    OVCandidate *c;
    OVService *s;
    OVIMAutoPhonetic *parent;
    CandidateList candi;
    int page;
};

class OVIMAutoPhonetic : public OVInputMethod {
public:
    virtual OVInputMethodContext *newContext();
    virtual int initialize(OVDictionary *, OVService *, const char *);
    virtual void update(OVDictionary *, OVService *);
    virtual const char *identifier();
    virtual const char *localizedName(const char *);
    virtual const char *selectKey();
    
protected:
    friend class OVIMAutoPhoneticContext;
    char selkey[96];
    SQLite3 *db;
    PhoneticService *srv;
    PhoneticConfig cfg;
};

OV_SINGLE_MODULE_WRAPPER(OVIMAutoPhonetic);

OVInputMethodContext *OVIMAutoPhonetic::newContext() {
    return new OVIMAutoPhoneticContext(this);
}

const char *OVIMAutoPhonetic::selectKey() {
    return selkey;
}

int OVIMAutoPhonetic::initialize(OVDictionary *cfg, OVService * s, const char *p) {
    db=new SQLite3;
    char dbfile[PATH_MAX];
    sprintf(dbfile, "%s/OVIMAutoPhonetic/tsi.db", p);
    murmur("OVIMAutoPhonetic: database file is %s", dbfile);
    
    if (int err=db->open(dbfile)) {
        s->notify("SQLite3 error");
        murmur("SQLite3 error! code=%d", err);
        return 0;
    }
    update(cfg, s);
    
    srv=new PhoneticService(db, s);
    return 1;
}

void OVIMAutoPhonetic::update(OVDictionary *c, OVService *) {
    cfg.layout=c->getIntegerWithDefault("keyboardLayout", 0);
    cfg.tonetolerance=c->getIntegerWithDefault("toneTolerance", 0);
    if (cfg.layout !=0 && cfg.layout !=1) cfg.layout=0;
    if (cfg.tonetolerance !=0 && cfg.tonetolerance !=1) cfg.tonetolerance=0;
    
    strcpy(selkey, c->getStringWithDefault("selectKey", "123456789"));
    murmur("OVIMAutoPhonetic: config update! keyboard layout=%d, tone tolerance=%d, select key=%s", cfg.layout, cfg.tonetolerance, selkey);
}

const char *OVIMAutoPhonetic::identifier() {
    return "OVIMAutoPhonetic";
}

const char *OVIMAutoPhonetic::localizedName(const char *lc) {
    if (!strcasecmp(lc, "zh_TW")) return "選詞注音輸入法實驗（SQLite 版）";
    return "Phrase-based Phonetic experiment (SQLite version)";
}


OVIMAutoPhoneticContext::OVIMAutoPhoneticContext(OVIMAutoPhonetic *p) :
    ps(p->srv, p->cfg), parent(p) 
{
}

OVIMAutoPhoneticContext::~OVIMAutoPhoneticContext() {
}

void OVIMAutoPhoneticContext::start(OVBuffer*, OVCandidate*, OVService* s) {
    ps.reset();
    ps.updateConfig(parent->cfg);
    candi.clear();
}

void OVIMAutoPhoneticContext::clear() {
    ps.reset();
    candi.clear();
}


int OVIMAutoPhoneticContext::keyEvent(OVKeyCode* pk, OVBuffer* pb, OVCandidate* pc, OVService* params) {
    k=pk; b=pb; c=pc; s=params;
    if (candi.size()) {
        // if candidateEvent() returns 0, it means key processed,
        // but nothing happens to the sentence, so we ignore it
        if (!candidateEvent()) return 1;    
        b->update();
        return 1;
    }
       
    if (k->isFunctionKey() && b->isEmpty()) return 0;

    KeyCode kc=k->code();
    KeyModifier km=0;
    if (k->isCtrl()) km |= K_CTRL;
    if (k->isOpt()) km |= K_OPT;
    if (k->isCommand()) km |= K_COMMAND;
    if (k->isShift()) km |= K_SHIFT;
    if (k->isCapslock()) km |= K_CAPSLOCK;
        
    return processEvent(ps.keyEvent(kc, km));        
}

int OVIMAutoPhoneticContext::processEvent(const TEvent &e) {
    murmur("event type=%d, dump=%s", e.type, ps.dump().c_str());
    int cursor, hstart, hend;
    string str;
    switch(e.type) {
        case T_IGNORE:
            if (b->isEmpty()) return 0;
            b->update();
            return 1;
        case T_CLEAR:
            b->clear()->update();
            ps.reset();
            return 1;
        case T_UPDATE:
            str=ps.presentation();
            cursor=ps.cursor();
            hstart=ps.markFrom();
            hend=hstart+ps.markLength();
            if (hstart==hend) hstart=hend=-1;
            b->clear();
            if (str.length()) b->append(str.c_str());
            b->update(cursor, hstart, hend);
            return 1;
        case T_COMMIT:
            str=ps.presentation();
            b->clear();
            if (str.length()) b->append(str.c_str());
            b->send();
            ps.reset();
            return 1;
        case T_CANDIDATE:
            str=ps.presentation();
            b->clear();
            if (str.length()) b->append(str.c_str());
            b->update();
            candi=ps.fetchCandidateList();
            page=0;
            return updateCandidateWindow();
        default:
            break;
    }
    murmur ("AutoPhoneticContext: wrong event type %d passed here", e.type);
    return 0;
}

int OVIMAutoPhoneticContext::candidateEvent() {
    char kc=k->code();
    if (kc==ovkEsc || kc==ovkBackspace || kc==ovkDelete) {
        processEvent(ps.cancelCandidate());
        closeCandidateWindow();
        return 1;
    }
    
    if (kc==ovkSpace || kc==ovkRight || kc==ovkDown || kc==ovkPageDown) {
        candidatePageDown();
        return 0;
    }
    if (kc==ovkLeft || kc==ovkUp || kc==ovkPageUp || kc=='<') {
        candidatePageUp();
        return 0;
    }

    int perpage=strlen(parent->selectKey());
    int i=0, l=perpage;
    for (i=0; i<perpage; i++) if(parent->selkey[i]==kc) break;
    if (i==l) {         // not a valid candidate key
        if (kc==ovkReturn) i=0;
    }
    if (i==l) {
        s->beep();
        return 0;
    }
    else {
        processEvent(ps.chooseCandidate(i+page*perpage,
            candi[i+page*perpage]));
        closeCandidateWindow();
    }    
    return 1;
}

int OVIMAutoPhoneticContext::updateCandidateWindow() {
    int candicount=candi.size();
    if (!candicount) return 1;
    int perpage=strlen(parent->selectKey());
    int pgstart=page*perpage;
        
    c->clear();
    char dispstr[32];
    
    for (int i=0; i<perpage; i++) {
        if (pgstart+i >= candicount) break;     // stop if idx exceeds candi counts
        sprintf(dispstr, "%c.", parent->selkey[i]);
        c->append(dispstr)->append(candi[page*perpage+i].c_str())
            ->append(" ");
    }
    // add current page number
    sprintf(dispstr, "(%d/%d)", page+1, (candicount-1)/perpage +1);
    c->append(dispstr);
    c->update();
    if (!c->onScreen()) c->show();
    return 1;
}

int OVIMAutoPhoneticContext::closeCandidateWindow() {
    if (c->onScreen()) c->hide()->clear()->update();
    if (candi.size()) {
        candi.clear();
    }
    return 1;        
}

int OVIMAutoPhoneticContext::candidatePageUp() {
    int maxpage=(candi.size()-1) / strlen(parent->selectKey());
    if (!maxpage) s->beep();
    else {
        if (!page) page=maxpage; else page--;
        updateCandidateWindow();
    }
    return 1;
}

int OVIMAutoPhoneticContext::candidatePageDown() {
    int maxpage=(candi.size()-1) / strlen(parent->selectKey());
    if (!maxpage) s->beep();
    else {
        if (page==maxpage) page=0; else page++;
        updateCandidateWindow();
    }
    return 1;
}

