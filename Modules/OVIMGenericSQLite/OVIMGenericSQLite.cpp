// OVIMGenericSQLite.cpp
// currently we only do the table "cj"

#define OV_DEBUG

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/syslimits.h>
#include "../../Headers/OVKeySequence.h"
#include "OVSQLite3.h"

// Some dirty public secrets goes here
SQLite3 *db;
//// Number of names in "tablelist" table
int  IM_TABLES;
//// The values of those names in "tablelist"... with some assumed limitations.
//// Please, not extreme sports here.
char *IM_TABLE_NAMES[32]; 
// End of secrets

const char *QueryForCommand(SQLite3 *db, const char *command);
const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key);

class IMGKeySequence : public OVKeySequenceSimple {
public:
    IMGKeySequence(const char *t) { strcpy(table, t); }
    virtual int isValidKey(char c);
    virtual int add(char c);
    virtual int isEmpty();
    virtual const char *compose();
    virtual const char *sequence() { return seq; }
protected:
    virtual const char *query(char c);
    char composebuf[256];
    char table[256];
};

struct IMGCandidate
{
    IMGCandidate();
    ~IMGCandidate();
    int count;
    char **candidates;
};

class OVIMGenericSQLite;

class OVIMGenericContext : public OVInputMethodContext {
public:
    OVIMGenericContext(OVIMGenericSQLite *p);
    virtual void start(OVBuffer*, OVCandidate*, OVService*);
    virtual void clear();
    virtual void end();
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);
    
protected:
    int keyEsc();
    int keyBackspace();
    int keyCompose();
    int keyPrintable();
    int keyNonRadical();
    int keyCapslock();
    int fetchCandidate(const char *);
    int fetchCandidateWithPrefix(const char *prefix, char c);
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
    OVIMGenericSQLite *parent;
    IMGKeySequence seq;
    IMGCandidate *candi;
    int page;
};

class OVIMGenericSQLite : public OVInputMethod {
public:
    OVIMGenericSQLite(char *);
    virtual OVInputMethodContext *newContext();
    virtual int initialize(OVDictionary *, OVService *, const char *);
    virtual void update(OVDictionary *, OVService *);
    virtual const char *identifier();
    virtual const char *localizedName(const char *);
    
protected:
    int isEndKey(char c);
    
    friend class OVIMGenericContext;
    int allowwildcard;
    char selkey[96];
    char endkey[96];
    char table[256];
    char idstr[256];
};

class OVOFReverseLookupSQLite : public OVOutputFilter
{
public:
    OVOFReverseLookupSQLite();
    virtual const char* identifier();
    virtual const char* localizedName(const char* locale);
    virtual int initialize(OVDictionary*, OVService*, const char*);
    virtual const char* process(const char *src, OVService *srv);
protected:
    char table[256];
    char idstr[256];
    char composebuffer[1024];
};


// OV_SINGLE_MODULE_WRAPPER(OVIMGenericSQLite);

extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;
}
extern "C" int OVInitializeLibrary(OVService*, const char*p) { 
    db=new SQLite3;  // this never gets deleted, but so do we

    char dbfile[PATH_MAX];
    char buf[256];
    int n = 0;

    sprintf(dbfile, "%s/OVIMGenericSQLite/imtables.db", p);
    if (int err=db->open(dbfile)) {
        murmur("SQLite3 error! code=%d", err);
        return 0;
    }
    SQLite3Statement *sth=db->prepare("select name from tablelist;");
    while(sth->step()==SQLITE_ROW) {
	int len;
	sprintf(buf,"%s",sth->column_text(0));
	len = strlen(buf);
	IM_TABLE_NAMES[n] = (char*)calloc(1,len);
	strcpy(IM_TABLE_NAMES[n],buf);
	n++;
    }
    IM_TABLES = n;
    return 1; 
}
extern "C" OVModule *OVGetModuleFromLibrary(int x) {
    if(x < IM_TABLES) {
	return new OVIMGenericSQLite(IM_TABLE_NAMES[x]);
// Temporarily ignore ReverseLookup modules for now.
//        case 1: return new OVOFReverseLookupSQLite;
    }
    return NULL;
}

OVOFReverseLookupSQLite::OVOFReverseLookupSQLite() {
    strcpy(table, "cj");
    strcpy(idstr, "OVOFReverseLookupSQLite-cj");
}

const char* OVOFReverseLookupSQLite::identifier() {
    return idstr;
}

const char* OVOFReverseLookupSQLite::localizedName(const char* lc) {
    return table;
//    if (!strcasecmp(lc, "zh_TW")) return "反查倉頡字根（SQLite 版）";
//    return "Cangjei lookup (SQLite version)";
}

int OVOFReverseLookupSQLite::initialize(OVDictionary *cfg, OVService * s, const char *p) {
    SQLite3Statement *sth=db->prepare("select name from tablelist;");
    if(sth->step()==SQLITE_ROW) {
	sprintf(table,"%s",sth->column_text(0));
	sprintf(idstr,"OVOFReverseLookupSQLite-%s",sth->column_text(0));
    }
    return 1;
}

const char* OVOFReverseLookupSQLite::process(const char *src, OVService *srv)
{
    unsigned short *u16;
    int u16len=srv->UTF8ToUTF16(src, &u16);
    strcpy(composebuffer, "");
    char sqlbuf[256];
    sprintf(sqlbuf,"select key from %s where value=?1;",table);

    // WE HAVE TO DO SURROGATE CHECK, REMEMBER!
    for (int i=0; i<u16len; i++) {
        // get each codepoint
        const char *u8=srv->UTF16ToUTF8(&(u16[i]), 1);
        
        char buf[256];
        sprintf(buf, "%s=(", u8);
        strcat(composebuffer, buf);
        
        SQLite3Statement *sth=db->prepare(sqlbuf);
        sth->bind_text(1, u8);
        while (sth->step()==SQLITE_ROW) {
            sprintf(buf, "%s, ", sth->column_text(0));
            strcat(composebuffer, buf);
        }
        strcat(composebuffer, ")\n");        
    }
    
    if(strlen(composebuffer)) srv->notify(composebuffer);
    return src;
}


OVIMGenericSQLite::OVIMGenericSQLite(char *name) {
    strcpy(table, name);
    sprintf(idstr,"OVIMGenericSQLite-%s",name);
}

OVInputMethodContext *OVIMGenericSQLite::newContext() {
    return new OVIMGenericContext(this);
}

int OVIMGenericSQLite::initialize(OVDictionary *cfg, OVService * s, const char *p) {
    update(cfg, s);
    return 1;
}

void OVIMGenericSQLite::update(OVDictionary *cfg, OVService *) {
    strcpy(selkey, cfg->getStringWithDefault("selectKey", "123456789"));    
    allowwildcard=cfg->getIntegerWithDefault("wildcard", 1);
    if (allowwildcard !=0 && allowwildcard !=1) allowwildcard=1;
    
    const char *ek=QueryForKey(db, table, "_property_endkey");
    if (ek) {
        strcpy(endkey, ek);
    }
    else {
        strcpy(endkey, "");
    }
    
    murmur("OVIMGenericSQLite: config update! select key=%s, endkey=%s", selkey, endkey);
}

const char *OVIMGenericSQLite::identifier() {
    return idstr;
}

const char *OVIMGenericSQLite::localizedName(const char *lc) {
    static char buf[256];
    sprintf(buf,"%s (SQLite ver.)",table);
    return buf;
//    if (!strcasecmp(lc, "zh_TW")) return "倉頡輸入法（SQLite 版）";
//    return "Cangjei (SQLite version)";
}

int OVIMGenericSQLite::isEndKey(char c) {
    if (strchr(endkey, c)) return 1;
    return 0;
}

OVIMGenericContext::OVIMGenericContext(OVIMGenericSQLite *p) : seq(p->table) {
    parent=p;
}

void OVIMGenericContext::start(OVBuffer*, OVCandidate*, OVService* s) {
    seq.clear();
    candi=NULL;
}

void OVIMGenericContext::clear() {
    seq.clear();
}

void OVIMGenericContext::end() {
}

int OVIMGenericContext::keyEvent(OVKeyCode* pk, OVBuffer* pb, OVCandidate* pc, OVService* ps) {
    k=pk; b=pb; c=pc; s=ps;
    if (candi) return candidateEvent();
    if (isPunctuationCombination()) return punctuationKey();
    if (k->isFunctionKey() && b->isEmpty()) return 0;
    if (k->isCapslock() && b->isEmpty()) return keyCapslock();
    if (k->code()==ovkEsc) return keyEsc();
    if (k->code()==ovkBackspace || k->code()==ovkDelete) return keyBackspace();
    if (!b->isEmpty() && 
        (k->code()==ovkSpace || k->code()==ovkReturn)) return keyCompose();
    if (isprint(k->code())) return keyPrintable();
    return 0;
}

int OVIMGenericContext::keyEsc() {
    if (b->isEmpty()) return 0;     // if buffer is empty, do nothing
    seq.clear();                    // otherwise we clear the syllable
    b->clear()->update();
    return 1;
}

int OVIMGenericContext::keyBackspace() {
    if (b->isEmpty()) return 0;
    seq.remove();
    b->clear();
    if (!seq.isEmpty()) b->append(seq.compose());
    b->update();
    return 1;
}

int OVIMGenericContext::keyPrintable() {
    if (isalpha(k->code()) && k->isShift() && b->isEmpty()) {
        char keystr[2];
        sprintf(keystr, "%c", tolower(k->code()));
        b->clear()->append(keystr)->send();
        return 1;
    }
    
    // wildcard hack: currently we don't allow ? * as the first character
    if (parent->allowwildcard && b->isEmpty() && 
        (k->code()=='?' || k->code()=='*')) return keyNonRadical();
        
    if (!seq.add(k->code())) {
        if (b->isEmpty()) return keyNonRadical(); // not a Radical keycode
        s->beep();
    }
    if (parent->isEndKey(k->code())) return keyCompose();
    b->clear()->append(seq.compose())->update();
    return 1;
}

int OVIMGenericContext::keyNonRadical() {
    char keystr[2];
    keystr[0]=k->code(); keystr[1]=0;
    int count=fetchCandidateWithPrefix("_punctuation_", k->code());
    if (!count) {          // not a punctuation mark
        b->clear()->append(keystr)->send();
        return 1;
    }
    if (count==1) return commitFirstCandidate();
    return updateCandidateWindow();
}

int OVIMGenericContext::isPunctuationCombination() {
    // only accept CTRL-1 or CTRL-0
    if (k->isCtrl() && !k->isOpt() && !k->isCommand() &&
        (k->code()=='1' || k->code()=='0')) return 1;
    // only accept CTRL-OPT-[printable]
    if (k->isCtrl() && k->isOpt() && !k->isCommand() && !k->isShift() && 
        ((k->code() >=1 && k->code() <=26) || isprint(k->code()))) return 1;
    return 0;
}

int OVIMGenericContext::punctuationKey() {
    int count;
    char kc=k->code();
    if ((kc=='0' || kc=='1') && !k->isOpt())
        count=fetchCandidate("_punctuation_list");
    else {
        if (kc >= 1 && kc <= 26) kc+='a'-1;
        count=fetchCandidateWithPrefix("_ctrl_opt_", kc);
    }
    if (!count) return 0;       // we send back the combination key
    if (count==1) return commitFirstCandidate();
    return updateCandidateWindow();    
}

int OVIMGenericContext::keyCapslock() {
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

int OVIMGenericContext::keyCompose() {
    b->clear()->append(seq.compose())->update();
    
    int count=fetchCandidate(seq.sequence());
    if (!count) {
        s->beep();
        return 1;
    }

    if (count==1) return commitFirstCandidate();
    return updateCandidateWindow();
}


int OVIMGenericContext::fetchCandidateWithPrefix(const char *prefix, char c) {
    char keystr[64];
    sprintf(keystr, "%s%c", prefix, c);
    return fetchCandidate(keystr);
}


int OVIMGenericContext::fetchCandidate(const char *qs) {
    page=0;
    if (candi) {
        delete candi;
        candi=NULL;
    }
    
    int wildcard=0;
    char realqs[256];
    strcpy(realqs, qs);
    if (parent->allowwildcard && strlen(seq.sequence())>1) {
        int ql=strlen(realqs);
        for (int qi=0; qi<ql; qi++) {
            if (realqs[qi]=='?') { realqs[qi]='_'; wildcard=1; }
            if (realqs[qi]=='*') { realqs[qi]='%'; wildcard=1; }
        }
    }
    
    // note in wildcard we don't use order by
    char cmd[256];
    if (!wildcard)
        sprintf(cmd, "select value from %s where key=?1 order by ord;", parent->table);
    else
        sprintf(cmd, "select value from %s where key like ?1;", parent->table);
    murmur("executing command=%s", cmd);

    SQLite3Statement *sth=db->prepare(cmd);
    if (!sth) return 0;
    sth->bind_text(1, realqs);
    
    int rows=0;
    while (sth->step()==SQLITE_ROW) rows++;
    murmur("query string=%s, number of candidates=%d", realqs, rows);
    if (!rows) {
        delete sth;
        return 0;
    }
    
    candi=new IMGCandidate;
    candi->count=0;
    candi->candidates=new char* [rows];
    sth->reset();
    while (sth->step()==SQLITE_ROW) {
        const char *v=sth->column_text(0);
        char *s=(char*)calloc(1, strlen(v)+1);
        strcpy(s, v);
        candi->candidates[candi->count++]=s;
    }
    delete sth;
    return rows;
}

int OVIMGenericContext::candidateEvent() {
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
        if (seq.isValidKey(kc)) { i=0; nextsyl=1; }
    }
    if (i==l) {
        s->beep();
        b->update();    // we do this to make some applications happy
    }
    else {
        b->clear()->append(candi->candidates[i + page*perpage])->send();
        closeCandidateWindow();
        if (nextsyl) {
            seq.add(kc);
            b->append(seq.compose())->update();
        }
    }    
    return 1;
}

int OVIMGenericContext::updateCandidateWindow() {
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

int OVIMGenericContext::closeCandidateWindow() {
    seq.clear();
    if (c->onScreen()) c->hide()->clear()->update();
    if (candi) {
        delete candi;
        candi=NULL;
    }
    return 1;        
}

int OVIMGenericContext::commitFirstCandidate() {
    if (!candi) return 1;
    b->clear()->append(candi->candidates[0])->send();
    return closeCandidateWindow();        
}

int OVIMGenericContext::candidatePageUp() {
    int maxpage=(candi->count-1) / strlen(parent->selkey);
    if (!maxpage) s->beep();
    else {
        if (!page) page=maxpage; else page--;
        updateCandidateWindow();
    }
    return 1;
}

int OVIMGenericContext::candidatePageDown() {
    int maxpage=(candi->count-1) / strlen(parent->selkey);
    if (!maxpage) s->beep();
    else {
        if (page==maxpage) page=0; else page++;
        updateCandidateWindow();
    }
    return 1;
}

int IMGKeySequence::isValidKey(char c) {
    if (query(c)) return 1;
    return 0;
}

int IMGKeySequence::add(char c) {
    if (query(c)) return OVKeySequenceSimple::add(tolower(c));
    return 0;
}

int IMGKeySequence::isEmpty() {
    if (len) return 0;
    return 1;
}
const char *IMGKeySequence::query(char c) {
    char keystr[32];
    sprintf(keystr, "_key_%c", tolower(c));
    return QueryForKey(db, table, keystr);
}

const char *IMGKeySequence::compose() {
    strcpy(composebuf, "");
    for (int i=0; i<len; i++) {
        const char *s=query(seq[i]);
        if (s) strcat(composebuf, s);
    }
    return composebuf;
}


IMGCandidate::IMGCandidate()
{
    count=0;
    candidates=NULL;
}

IMGCandidate::~IMGCandidate()
{
    if (!count) return;
    for (int i=0; i<count; i++) delete candidates[i];
    delete[] candidates;
}


const char *QueryForCommand(SQLite3 *db, const char *command) {
    char *r=NULL;
    static char valuebuffer[256];

    murmur("executing SQL command: %s", command);

    SQLite3Statement *sth=db->prepare(command);
    if (sth) {
        if (sth->step()==SQLITE_ROW) {
            strcpy(valuebuffer, sth->column_text(0));
            murmur("return value=%s", valuebuffer);
            r=valuebuffer;
        }
        delete sth;
    }
    return r;
}

const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key) {
    char cmd[256];
    char keyescaped[128];
    const char *kp=key;
    char *ep=keyescaped;
    while (*kp) {
        if (*kp=='\'') { *ep++='\''; *ep++='\''; }
        else *ep++=*kp;
        kp++;
    }
    *ep=0;
    sprintf(cmd, "select value from %s where key='%s';", tbl, keyescaped);
    return QueryForCommand(db, cmd);
}


