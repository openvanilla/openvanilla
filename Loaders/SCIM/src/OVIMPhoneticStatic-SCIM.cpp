// OVIMPhoneticStatic.cpp
#define OV_DEBUG

#include "OpenVanilla.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "OVPhoneticLib.h"

extern unsigned short ovPhoneticData[];

class OVIMPhoneticStatic;

class OVIMPhoneticContext : public OVInputMethodContext {
public:
    OVIMPhoneticContext(OVIMPhoneticStatic *p);
    virtual void start(OVBuffer*, OVCandidate*, OVService*);
    virtual void clear();
    virtual void end();
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);
    
protected:
    int keyEsc();
    int keyBackspace();
    int keyCompose();
    int keyPrintable();
    int keyNonBPMF();
    int keyCapslock();
    int isPunctuationCombination();
    int punctuationKey();
    int updateCandidateWindow();
    int closeCandidateWindow();
    int commitFirstCandidate();
    int candidateEvent();
    int candidatePageUp();
    int candidatePageDown();
    
    OVKeyCode *k;
    OVBuffer *b;
    OVCandidate *c;
    OVService *s;
    OVIMPhoneticStatic *parent;
    OVPhoneticSyllable syl;
    OVPCandidate *candi;
    int page;
};

class OVIMPhoneticStatic : public OVInputMethod {
public:
    virtual OVInputMethodContext *newContext();
    virtual int initialize(OVDictionary *, OVService *, const char *);
    virtual void update(OVDictionary *, OVService *);
    virtual const char *identifier();
    virtual const char *localizedName(const char *);
    
protected:
    friend class OVIMPhoneticContext;
    int layout;
    char selkey[96];
};

OVInputMethodContext *OVIMPhoneticStatic::newContext() {
    return new OVIMPhoneticContext(this);
}

int OVIMPhoneticStatic::initialize(OVDictionary *cfg, OVService * s, const char *p) {
    update(cfg, s);
    return 1;
}

void OVIMPhoneticStatic::update(OVDictionary *cfg, OVService *) {
    layout=0;       // 0=Standard, 1=ETen layout
    // layout=cfg->getIntegerWithDefault("keyboardLayout", 0);
    // if (layout !=0 && layout !=1) layout=0;
    strcpy(selkey, cfg->getStringWithDefault("selectKey", "123456789"));    
}

const char *OVIMPhoneticStatic::identifier() {
    return "OVIMPhoneticStatic";
}

const char *OVIMPhoneticStatic::localizedName(const char *lc) {
    if (!strcasecmp(lc, "zh_TW")) return "注音輸入法（Static 版）";
    return "Phonetic (Bopomofo, Static version)";
}


OVIMPhoneticContext::OVIMPhoneticContext(OVIMPhoneticStatic *p) {
    parent=p;
}

void OVIMPhoneticContext::start(OVBuffer*, OVCandidate*, OVService* s) {
    syl.clear();
    syl.setLayout(parent->layout);
    candi=NULL;
}

void OVIMPhoneticContext::clear() {
    syl.clear();
}

void OVIMPhoneticContext::end() {
}

int OVIMPhoneticContext::keyEvent(OVKeyCode* pk, OVBuffer* pb, OVCandidate* pc, OVService* ps) {
    k=pk; b=pb; c=pc; s=ps;
    if (candi) return candidateEvent();
    if (isPunctuationCombination() && b->isEmpty()) return punctuationKey();
    if (k->isFunctionKey() && b->isEmpty()) return 0;
    if (k->isCapslock() && b->isEmpty()) return keyCapslock();
    if (k->code()==ovkEsc) return keyEsc();
    if (k->code()==ovkBackspace || k->code()==ovkDelete) return keyBackspace();
    if (!b->isEmpty() && (syl.isComposeKey(k->code()) || k->code()==ovkReturn)) return keyCompose();
    if (isprint(k->code())) return keyPrintable();
    return 0;
}

int OVIMPhoneticContext::keyEsc() {
    if (b->isEmpty()) return 0;     // if buffer is empty, do nothing
    syl.clear();                    // otherwise we clear the syllable
    b->clear()->update();
    return 1;
}

int OVIMPhoneticContext::keyBackspace() {
    if (b->isEmpty()) return 0;
    syl.removeLast();
    b->clear();
    if (!syl.empty()) b->append(syl.compose());
    b->update();
    return 1;
}

int OVIMPhoneticContext::keyPrintable() {
    if (isalpha(k->code()) && k->isShift() && b->isEmpty()) {
        char keystr[2];
        sprintf(keystr, "%c", tolower(k->code()));
        b->clear()->append(keystr)->send();
        return 1;
    }
    if (!syl.addKey(k->code())) {
        if (b->isEmpty()) return keyNonBPMF(); // not a BPMF keycode
        s->beep();
    }
    b->clear()->append(syl.compose())->update();
    return 1;
}

int OVIMPhoneticContext::keyNonBPMF() {
    char keystr[2];
    keystr[0]=k->code(); keystr[1]=0;
    candi=OVPFindCandidateWithCode(ovPhoneticData, OVP_PUNCTUATION_MASK | toupper(k->code()));
    if (!candi) {          // not a punctuation mark
        b->clear()->append(keystr)->send();
        return 1;
    }
    if (candi->count==1) return commitFirstCandidate();
    
    // we need to put the first candidate onto the composing buffer
    // to make some application (e.g. iTerm.app) happy
    b->clear()->append(candi->candidates[0])->update();
    page=0;
    return updateCandidateWindow();
}

int OVIMPhoneticContext::isPunctuationCombination() {
    // only accept CTRL-1 or CTRL-0
    if (k->isCtrl() && !k->isOpt() && !k->isCommand() &&
        (k->code()=='1' || k->code()=='0')) return 1;
    // only accept CTRL-OPT-[printable]
    if (k->isCtrl() && k->isOpt() && !k->isCommand() && !k->isShift() && 
        ((k->code() >=1 && k->code() <=26) || isprint(k->code()))) return 1;
    return 0;
}

int OVIMPhoneticContext::punctuationKey() {
    char kc=k->code();
    if ((kc=='0' || kc=='1') && !k->isOpt())
        candi=OVPFindCandidateWithCode(ovPhoneticData, OVP_PUNCTUATION_LIST);
    else {
        if (kc >= 1 && kc <= 26) kc+='a'-1;
        candi=OVPFindCandidateWithCode(ovPhoneticData, OVP_CTRL_OPT_MASK | toupper(kc));
    }
    if (!candi) return 0;       // we send back the combination key
    if (candi->count==1) return commitFirstCandidate();
    
    // we need to put the first candidate onto the composing buffer
    // to make some application (e.g. iTerm.app) happy
    b->clear()->append(candi->candidates[0])->update();
    page=0;
    return updateCandidateWindow();    
}

int OVIMPhoneticContext::keyCapslock() {
    char keystr[2];
    keystr[1]=0;
    if (isprint(k->code())) {
        if (k->isShift()) keystr[0]=toupper(k->code());
        else keystr[0]=tolower(k->code());
        b->clear()->append(keystr)->send();
        return 1;
    }
    return 0;
}

int OVIMPhoneticContext::keyCompose() {
    if (k->code()!=ovkSpace && k->code()!=ovkReturn) syl.addKey(k->code());
    
    // we don't update the compose key (tone key) at this stage,
    // because it often causes display nauseousness
    // b->clear()->append(syl.compose())->update();
    
    candi=OVPFindCandidate(ovPhoneticData, &syl);
    if (!candi) {
        b->clear()->append(syl.compose())->update();      // update here
        s->beep();
        return 1;
    }

    if (candi->count==1) return commitFirstCandidate();
    
    // we display first candidate instead
    b->clear()->append(candi->candidates[0])->update();
    page=0;
    return updateCandidateWindow();
}


int OVIMPhoneticContext::candidateEvent() {
    char kc=k->code();
    if (kc==ovkEsc || kc==ovkBackspace || kc==ovkDelete) {  //ESC/BKSP/DELETE cancels candi window
        clear();
        b->clear()->update();
        return closeCandidateWindow();
    }
    
    if (kc==ovkSpace || kc==ovkRight || kc==ovkDown || kc==ovkPageDown || kc =='>')
        return candidatePageDown(); 
    if (kc==ovkLeft || kc==ovkUp || kc==ovkPageUp || kc=='<')
        return candidatePageUp();

    int perpage=strlen(parent->selkey);
    int i=0, l=perpage, nextsyl=0;
    for (i=0; i<perpage; i++) if(parent->selkey[i]==kc) break;
    if (i==l) {         // not a valid candidate key
        if (kc==ovkReturn) i=0;
        if (syl.isValidKey(kc)) { i=0; nextsyl=1; }
    }
    if (i==l) {
        s->beep();
        b->update();    // we do this to make some applications happy
    }
    else {
        b->clear()->append(candi->candidates[i + page*perpage])->send();
        closeCandidateWindow();
        if (nextsyl) {
            syl.clear();
            syl.addKey(kc);
            b->clear()->append(syl.compose())->update();
        }
    }    
    return 1;
}

int OVIMPhoneticContext::updateCandidateWindow() {
    if (!candi) return 1;
    int candicount=candi->count;
    int perpage=strlen(parent->selkey);
    int pgstart=page*perpage;
        
    c->clear();
    char dispstr[32];
    
    for (int i=0; i<perpage; i++) {
        if (pgstart+i >= candicount) break;     // stop if idx exceeds candi counts
        sprintf(dispstr, "%c.", parent->selkey[i]);
        c->append(dispstr)->append(candi->candidates[page*perpage+i])->append(" ");
    }
    // add current page number
    sprintf(dispstr, "(%d/%d)", page+1, (candicount-1)/perpage +1);
    c->append(dispstr);
    c->update();
    if (!c->onScreen()) c->show();
    b->update();        // we do this to make some application happy
    return 1;
}

int OVIMPhoneticContext::closeCandidateWindow() {
    syl.clear();
    if (c->onScreen()) c->hide()->clear()->update();
    if (candi) {
        delete candi;
        candi=NULL;
    }
    return 1;        
}

int OVIMPhoneticContext::commitFirstCandidate() {
    if (!candi) return 1;
    b->clear()->append(candi->candidates[0])->send();
    return closeCandidateWindow();        
}

int OVIMPhoneticContext::candidatePageUp() {
    int maxpage=(candi->count-1) / strlen(parent->selkey);
    if (!maxpage) s->beep();
    else {
        if (!page) page=maxpage; else page--;
        updateCandidateWindow();
    }
    return 1;
}

int OVIMPhoneticContext::candidatePageDown() {
    int maxpage=(candi->count-1) / strlen(parent->selkey);
    if (!maxpage) s->beep();
    else {
        if (page==maxpage) page=0; else page++;
        updateCandidateWindow();
    }
    return 1;
}

