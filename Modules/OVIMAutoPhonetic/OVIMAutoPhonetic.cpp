// OVIMAutoPhonetic.cpp
#define OV_DEBUG

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/syslimits.h>
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

    PhoneticPhrase *ps;
    OVKeyCode *k;
    OVBuffer *b;
    OVCandidate *c;
    OVService *s;
    OVIMAutoPhonetic *parent;
    CandidateList *candi;
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
    int layout;
    char selkey[96];
    SQLite3 *db;
    OVServiceWrapper *srv;
};

OV_SINGLE_MODULE_WRAPPER(OVIMAutoPhonetic);

OVInputMethodContext *OVIMAutoPhonetic::newContext() {
    return new OVIMAutoPhoneticContext(this);
}

const char *OVIMAutoPhonetic::selectKey() {
    return selkey;
}

int OVIMAutoPhonetic::initialize(OVDictionary *cfg, OVService * s, const char *p) {
    srv=new OVServiceWrapper(s);
    
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
    return 1;
}

void OVIMAutoPhonetic::update(OVDictionary *cfg, OVService *) {
    layout=cfg->getIntegerWithDefault("keyboardLayout", 0);
    if (layout !=0 && layout !=1) layout=0;
    strcpy(selkey, cfg->getStringWithDefault("selectKey", "123456789"));
    murmur("OVIMAutoPhonetic: config update! keyboard layout=%d, select key=%s", layout, selkey);
}

const char *OVIMAutoPhonetic::identifier() {
    return "OVIMAutoPhonetic";
}

const char *OVIMAutoPhonetic::localizedName(const char *lc) {
    if (!strcasecmp(lc, "zh_TW")) return "選詞注音輸入法實驗（SQLite 版）";
    return "Phrase-based Phonetic experiment (SQLite version)";
}


OVIMAutoPhoneticContext::OVIMAutoPhoneticContext(OVIMAutoPhonetic *p) {
    parent=p;
    ps=new PhoneticPhrase(parent->srv, parent->layout, parent->db);
}

OVIMAutoPhoneticContext::~OVIMAutoPhoneticContext() {
    delete ps;
}

void OVIMAutoPhoneticContext::start(OVBuffer*, OVCandidate*, OVService* s) {
    ps->clear();
    ps->setKeyboardLayout(parent->layout);
    candi=NULL;
}

void OVIMAutoPhoneticContext::clear() {
    ps->clear();
}


int OVIMAutoPhoneticContext::keyEvent(OVKeyCode* pk, OVBuffer* pb, OVCandidate* pc, OVService* params) {
    k=pk; b=pb; c=pc; s=params;
    if (candi) {
        // if candidateEvent() returns 0, it means key processed,
        // but nothing happens to the sentence, so we ignore it
        if (!candidateEvent()) return 1;    
        b->update();
        return 1;
    }
       
    if (k->isFunctionKey() && b->isEmpty()) return 0;

    TEvent t;
    switch (k->code()) {
        case ovkEsc:        t=ps->keyEsc();         break;
        case ovkBackspace:  t=ps->keyBackspace();   break;
        case ovkDelete:     t=ps->keyBackspace();   break;
        case ovkRight:      t=ps->keyRight();       break;
        case ovkLeft:       t=ps->keyLeft();        break;
        case ovkUp:         t=ps->keyUp();          break;
        case ovkDown:       t=ps->keyDown();        break;
        case ovkHome:       t=ps->keyHome();        break;
        case ovkEnd:        t=ps->keyEnd();         break;
        case ovkReturn:     t=ps->keyEnter();       break;
        case ovkSpace:      t=ps->keySpace();       break;
        default:
            t=ps->key(k->code(), 0);
    }
    
    return processEvent(t);
}

int OVIMAutoPhoneticContext::processEvent(const TEvent &e) {
    int cursor, hstart, hend;
    const char *str;
    switch(e.type) {
        case T_IGNORE:
            if (b->isEmpty()) return 0;
            b->update();
            return 1;
        case T_CLEAR:
            b->clear()->update();
            return 1;
        case T_UPDATE:
            str=ps->composeDisplay();
            cursor=ps->cursor();
            hstart=ps->hiliteStart();
            hend=hstart+ps->hiliteLength();
            if (hstart==hend) hstart=hend=-1;
            b->clear();
            if (str) b->append(str);
            b->update(cursor, hstart, hend);
            return 1;
        case T_COMMIT:
            str=ps->composeDisplay();
            b->clear();
            if (str) b->append(str);
            b->send();
            ps->clear();
            return 1;
        case T_CANDIDATE:
            page=0;
            candi=ps->getCandidateList();
            return updateCandidateWindow();
    }
    murmur ("AutoPhoneticContext: wrong event type %d passed here", e.type);
    return 0;
}

int OVIMAutoPhoneticContext::candidateEvent() {
    char kc=k->code();
    if (kc==ovkEsc || kc==ovkBackspace || kc==ovkDelete) {
        return processEvent(ps->cancelCandidate());
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
        processEvent(ps->chooseCandidate(i+page*perpage,
            candi->item(i+page*perpage)));
        closeCandidateWindow();
    }    
    return 1;
}

int OVIMAutoPhoneticContext::updateCandidateWindow() {
    if (!candi) return 1;
    int candicount=candi->count();
    int perpage=strlen(parent->selectKey());
    int pgstart=page*perpage;
        
    c->clear();
    char dispstr[32];
    
    for (int i=0; i<perpage; i++) {
        if (pgstart+i >= candicount) break;     // stop if idx exceeds candi counts
        sprintf(dispstr, "%c.", parent->selkey[i]);
        c->append(dispstr)->append(candi->item(page*perpage+i)->dataString())
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
    if (candi) {
        // delete candi;
        candi=NULL;
    }
    return 1;        
}

int OVIMAutoPhoneticContext::candidatePageUp() {
    int maxpage=(candi->count()-1) / strlen(parent->selectKey());
    if (!maxpage) s->beep();
    else {
        if (!page) page=maxpage; else page--;
        updateCandidateWindow();
    }
    return 1;
}

int OVIMAutoPhoneticContext::candidatePageDown() {
    int maxpage=(candi->count()-1) / strlen(parent->selectKey());
    if (!maxpage) s->beep();
    else {
        if (page==maxpage) page=0; else page++;
        updateCandidateWindow();
    }
    return 1;
}

