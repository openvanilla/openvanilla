// AutoPhonetic.h

#ifndef __AutoPhonetic_h
#define __AutoPhoneitc_h

#include "OVPhoneticLib.h"
#include "GrammarUnit.h"

CandidateList *FetchBPMFCandidate(SQLite3 *db, const char *qs) {
    SQLite3Statement *sth=db->prepare("select chr from bpmf where bpmf=?1 order by ord;");
    if (!sth) return NULL;
    sth->bind_text(1, qs);
    int rows=0;
    while (sth->step()==SQLITE_ROW) rows++;
    if (!rows) {
        delete sth;
        return NULL;
    }
    CandidateList* candi=new CandidateList(rows);
    sth->reset();
    int i=0;
    while (sth->step()==SQLITE_ROW) {
        candi->setItem(i++, new CandidateItem(sth->column_text(0)));
    }
    delete sth;
    return candi;
}

CandidateList *FetchTsiCandidate(SQLite3 *db, const char *bpmf, const char *tone) {
    SQLite3Statement *sth;
    if (tone) {
        sth=db->prepare("select tsi from tsi where bpmf=?1 and tone=?2 order by freq desc;");
        if (!sth) return NULL;
        sth->bind_text(1, bpmf);
        sth->bind_text(2, tone);
    }
    else {
        sth=db->prepare("select tsi from tsi where bpmf=?1 order by freq desc;");
        if (!sth) return NULL;
        sth->bind_text(1, bpmf);
    }
    int rows=0;
    while (sth->step()==SQLITE_ROW) rows++;
    if (!rows) {
        delete sth;
        return NULL;
    }
    CandidateList* candi=new CandidateList(rows);
    sth->reset();
    int i=0;
    while (sth->step()==SQLITE_ROW) {
        candi->setItem(i++, new CandidateItem(sth->column_text(0)));
    }
    delete sth;
    return candi;
}

class OVServiceWrapper : public Service {
public:
    OVServiceWrapper(OVService *s) : srv(s) {};
    virtual void notify (const char *msg)  { srv->notify(msg); }
    virtual void beep() { srv->beep(); }
    virtual int u8strlen(const char *str) {
        unsigned int short *u16s;
        int u16len=srv->UTF8ToUTF16(str, &u16s);
        int ul=0;
        for (int i=0; i<u16len; i++) {
            if (u16s[i] >= 0xd800 && u16s[i] <= 0xdfff) i++;
            ul++;
        }
        return ul;
    }
protected:
    OVService *srv;
};

class PhoneticSyllable : public GrammarUnit {
public:
    PhoneticSyllable(Service *s, int l, SQLite3 *d) : GrammarUnit(s), syl(l) { 
        db=d;
        clear();
        strcpy(composed, "");
        strcpy(codestr, "");
    }    
    void setKeyboardLayout(int l) {
        syl.setLayout(l);
    }
    
    virtual const char *composeDisplay() {
        if (strlen(composed)) return composed;
        return syl.compose();
    }
    virtual const char *codeString() {
        if (strlen(codestr)) return codestr;
        return syl.standardLayoutCode();
    }
    virtual const TEvent enterRightBound() { return TEvent(); }
    virtual const TEvent enterLeftBound()  { return TEvent(); } 
    virtual const TEvent clear() { 
        syl.clear();
//      strcpy(composed, "");
//      strcpy(codestr, "");
        return TEvent(T_CLEAR);
    }
    virtual const TEvent key(char c, KeyModifier m) {
        murmur("key=%d", c);
        if (syl.isComposeKey(c) && !syl.empty()) {
            murmur("compose key!");
            if (c!=32) syl.addKey(c);
            CandidateList *l=FetchBPMFCandidate(db, syl.standardLayoutCode());
            if (l) {
                strcpy(composed, l->item(0)->dataString());
                strcpy(codestr, syl.standardLayoutCode());
                delete l;
                syl.clear();
            }
            // compose character, if not found, return T_UPDATE;
            return TEvent(T_COMMIT);
        }
        if (!syl.addKey(c)) srv->beep();
        return TEvent(T_UPDATE);
    }
    virtual const TEvent keyBackspace() {
        if (syl.empty()) return TEvent(T_IGNORE);
        syl.removeLast();
        if (syl.empty()) return TEvent(T_CLEAR);
        return TEvent(T_UPDATE);
    }
    
    virtual const TEvent keySpace() {
        return(key(32, 0));
    }
    
    virtual const TEvent keyEnter()     { return TEvent(); }
    virtual const TEvent keyEsc()       { return clear(); }
    virtual const TEvent keyLeft()      { return TEvent(); }
    virtual const TEvent keyRight()     { return TEvent(); }
    virtual const TEvent keyUp()        { return TEvent(); }
    virtual const TEvent keyDown()      { return TEvent(); }
    virtual const TEvent keyHome()      { return TEvent(); }
    virtual const TEvent keyEnd()       { return TEvent(); }
    virtual const TEvent chooseCandidate(int idx, CandidateItem *item) {
        return TEvent();
    }
    virtual const TEvent cancelCandidate() {
        return TEvent();
    }
    virtual CandidateList *getCandidateList() { return NULL; };
    
    virtual const char *BPMFString() {
        strcpy(buf, codestr);
        if (!strlen(buf)) return buf;
        char lc=buf[strlen(buf)-1];
        if (lc=='3' || lc=='4' || lc=='6' || lc=='7') buf[strlen(buf)-1]=0;
        return buf;
    }
    
    virtual const char *toneString() {
        strcpy(buf, codestr);
        if (!strlen(buf)) return buf;
        char lc=buf[strlen(buf)-1];
        switch (lc) {
            case '3':
            case '4':
                break;
            case '6':
                lc='2';
                break;
            case '7':
                lc='5';
                break;
            default:
                lc='1';
        }
        sprintf(buf, "%c", lc);
        return buf;
    }
protected:
    SQLite3 *db;
    OVPhoneticSyllable syl;
    char composed[16];
    char codestr[16];
    char buf[256];
};

const int PP_MaxSyllable=64;

class SyllableList : public AutoPhraseObject {
public:
    SyllableList() {
        cnt=0;
        for (int i=0; i<PP_MaxSyllable+1; i++) syl[i]=NULL;
    }
    ~SyllableList() {
        clear();
    }
    PhoneticSyllable* item(int i) {
        if (i < 0) return NULL;
        if (i > cnt-1) return NULL;
        return syl[i];
    }
    
    void setKeyboardLayout(int l) {
        for (int i=0; i<cnt; i++) syl[i]->setKeyboardLayout(l);
    }
    void clear() {
        for (int i=0; i<cnt; i++) {
            if (syl[i]) delete syl[i];
            syl[i]=NULL;
        }
        cnt=0;
    }
    int count() { return cnt; }
    int append(PhoneticSyllable *s) {
        if (cnt == PP_MaxSyllable) return 0;
        syl[cnt++]=s;
    }
    int append(int pnt, PhoneticSyllable *s) {
        if (cnt == PP_MaxSyllable) return 0;
        if (pnt >= cnt) return append(s);
        for (int i=cnt; i>=pnt+1; i--) syl[i]=syl[i-1];
        syl[pnt]=s;
        cnt++;
    }
    void removeLast() {
        if (!cnt) return;
        delete syl[cnt-1];
        syl[cnt-1]=NULL;
        cnt--;
    }
    
    void remove(int pnt) {
        if (pnt >= cnt-1) { removeLast(); return; }
        if (!cnt) return;
        if (!((0 <= pnt) && (pnt <= cnt-1))) return;
        delete syl[pnt];
        syl[pnt]=NULL;
        for (int i=pnt; i<cnt; i++) syl[i]=syl[i+1];  // syl[MAX+1]=NULL, safe
        cnt--;
    }
    
    const char *toneString() {
        strcpy(buf, "");
        if (!cnt) return buf;
        int i;
        for (i=0; i<cnt-1; i++) {
            const char *t=syl[i]->toneString();
            if (strlen(t)) {
                strcat(buf, t);
                strcat(buf, " ");
            }
        }
        const char *t=syl[i]->toneString();
        if (strlen(t)) strcat(buf, t);
        murmur("tone string=%s", buf);
        return buf;
    }
    
    const char *BPMFString() {
        strcpy(buf, "");
        if (!cnt) return buf;
        int i;
        for (i=0; i<cnt-1; i++) {
            const char *t=syl[i]->BPMFString();
            if (strlen(t)) {
                strcat(buf, t);
                strcat(buf, " ");
            }
        }
        const char *t=syl[i]->BPMFString();
        if (strlen(t)) strcat(buf, t);
        murmur("BPMF string=%s", buf);
        return buf;
    }
    
    const char *displayString() {
        strcpy(buf, "");
        for (int i=0; i<cnt; i++) {
            const char *t=syl[i]->composeDisplay();
            if (strlen(t)) {
                strcat(buf, t);
            }
        }
        murmur("display string=%s", buf);
        return buf;
    }
    
    // void dispatchDisplayString();    // send composed dsp str back to each
    
protected:
    int cnt;
    PhoneticSyllable* syl[PP_MaxSyllable+1];
    char buf[512];
};




class PhoneticPhrase : public GrammarUnit {
public:
    PhoneticPhrase(Service *s, int l, SQLite3 *d) : GrammarUnit(s) {
        db=d;
        layout=l;
        tsicomposed=0;
        focus=-1;
        cursorPos=0;
        candi=NULL;
        clear();
    }
            
    void setKeyboardLayout(int l) {
        layout=l;
        lst.setKeyboardLayout(layout);
    }
    
//  virtual int length()    { return srv->u8strlen(composeDisplay()); }
    virtual int cursor()    { 
        if (cursorPos < 0) cursorPos=0;
        if (cursorPos > lst.count()) cursorPos=lst.count();
        return cursorPos;
    }
    virtual int hiliteStart()   { 
        if (focus != -1) return focus;
        return 0;
    }
    virtual int hiliteLength()  {
        if (focus != -1) return (lst.item(focus))->length();
        return 0 ;
    }
    
    virtual const char *composeDisplay() {
        if (tsicomposed && strlen(composed)) return composed;
        return lst.displayString();        
    }
    virtual const char *codeString() {
        return "";          // we don't need it at this level
    }
    virtual const TEvent enterRightBound() { return TEvent(); }
    virtual const TEvent enterLeftBound()  { return TEvent(); } 
    virtual const TEvent clear() { 
        focus=-1;
        cursorPos=0;
        lst.clear();
        strcpy(composed, "");
        return TEvent(T_CLEAR);
    }
    virtual const TEvent update() {
        if (!lst.count()) return TEvent(T_CLEAR);
        char bpmf[256], tone[256];
        strcpy(bpmf, lst.BPMFString());
        strcpy(tone, lst.toneString());
        murmur("update! bpmf=%s, tone=%s", bpmf, tone);
        if (candi) delete candi;

        // candi=FetchTsiCandidate(db, bpmf, tone);
        
        // test TONE TOLERANCE
        if (lst.count() == 1) 
            candi=FetchTsiCandidate(db, bpmf, tone);
        else
            candi=FetchTsiCandidate(db, bpmf, NULL);
        if (candi) {
            for (int i=0; i<candi->count(); i++) 
                murmur("candiate %d = %s", i, candi->item(i)->dataString());
            tsicomposed=1;
            strcpy(composed, candi->item(0)->dataString());
        } else {
            tsicomposed=0;
            strcpy(composed, "");
        }
        return TEvent(T_UPDATE);
    }
    virtual const TEvent processSyllableEvent(const TEvent &e) {
        switch(e.type) {
            case T_IGNORE:
            case T_UPDATE:
                return TEvent(T_UPDATE);
            case T_COMMIT:
                cursorPos=focus+1;
                focus=-1;
                break;
            case T_CLEAR:
                lst.remove(focus);
                focus=-1;
                cursor();
                break;
        }
        return update();
    }
    virtual const TEvent key(char c, KeyModifier m) {
        if (focus == -1) {
            PhoneticSyllable *ps=new PhoneticSyllable(srv, layout, db);
            lst.append(cursorPos, ps);
            focus=cursorPos;
        }
        murmur("key=%d, focus=%d, cursorPos=%d", c, focus, cursorPos);
        PhoneticSyllable *syl=lst.item(focus);
        if (!syl) {
            murmur("NULL!");
        }
        tsicomposed=0;
        return processSyllableEvent(syl->key(c, m));
    }
    virtual const TEvent keyBackspace() {
        if (focus != -1) {
            return processSyllableEvent((lst.item(focus))->keyBackspace());
        }
        if (!lst.count()) return TEvent(T_IGNORE);
        if (!cursorPos) {
            srv->beep();
            return TEvent(T_UPDATE);
        }
        lst.remove(--cursorPos);
        if (!lst.count()) return TEvent(T_CLEAR);
        return update();
    }
    
    virtual const TEvent keySpace() {
        if (focus != -1) {
            return processSyllableEvent((lst.item(focus))->keySpace());
        }
        srv->beep();
        return update();
    }
    
    virtual const TEvent keyEnter()     {
        if (!lst.count()) return TEvent(T_IGNORE);
        murmur("commit! bpmfstr='%s'", lst.BPMFString());
        murmur("tonestr='%s'", lst.toneString());
        murmur("displaystr='%s'",composeDisplay());
        return TEvent(T_COMMIT);
    }
    virtual const TEvent keyEsc()       { return clear(); }
    virtual const TEvent keyLeft()      { 
        if (!lst.count()) return TEvent(T_IGNORE);
        if (focus != -1) {
            srv->beep();
        } else {
            cursorPos--;
            if (cursorPos<0) cursorPos=0;
        }
        return TEvent(T_UPDATE);
    }
    virtual const TEvent keyRight()     { 
        if (!lst.count()) return TEvent(T_IGNORE);
        if (focus != -1) {
            srv->beep();
        } else {
            cursorPos++;
            if (cursorPos>lst.count()) cursorPos=lst.count();
        }
        return TEvent(T_UPDATE);        
    }
    virtual const TEvent keyUp()        { return TEvent(); }
    virtual const TEvent keyDown()      { 
        if (!lst.count()) return TEvent(T_IGNORE);
        if (!candi) return TEvent(T_IGNORE);
        return TEvent(T_CANDIDATE);
    }
    virtual const TEvent keyHome()      { return TEvent(); }
    virtual const TEvent keyEnd()       { return TEvent(); }
    virtual const TEvent chooseCandidate(int idx, CandidateItem *item) {
        tsicomposed=1;
        strcpy(composed, item->dataString());
        return TEvent(T_UPDATE);
    }
    virtual const TEvent cancelCandidate() {
        return TEvent(T_UPDATE);
    }
    virtual CandidateList *getCandidateList() { 
        return candi;
    }

protected:
    SQLite3 *db;
    int layout;
    int focus, cursorPos;
    SyllableList lst;
    int tsicomposed;
    char composed[256];
    CandidateList *candi;
};

#endif
