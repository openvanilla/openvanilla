// OVIMTobacco.cpp

//#define OV_DEBUG

#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
	#include <OpenVanilla/OVLibrary.h>
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OpenVanilla.h"
	#include "OVLibrary.h"
	#include "OVUtility.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef WIN32
	#define strcasecmp stricmp
#endif

#include "OVKeySequence.h"
#include "OVSQLite3.h"

#ifndef WIN32
	#include <sys/syslimits.h>	//for PATH_MAX
#else
	#include <windows.h>
	#define PATH_MAX MAX_PATH
#endif

#include <string>

#include "PredictorSingleton.h"

#include "hsukey.h"

// Some dirty public secrets goes here
SQLite3 *db;
//// Number of names in "tablelist" table
static int IM_TABLES;
//// The values of those names in "tablelist"... with some assumed limitations.
//// Please, not extreme sports here.
char *IM_TABLE_NAMES[32];
// End of secrets

const char *QueryForCommand(SQLite3 *db, const char *command);
const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key);

PredictorSingleton* predictor;

class IMGKeySequence : public OVKeySequenceSimple {
public:
    IMGKeySequence(const char *t) { strcpy(table, t); }
    virtual int isValidKey(char c);
    virtual bool add(char c);
    virtual int isEmpty();
	virtual int length() { return len; }
    virtual const char *compose();
    virtual const char *sequence() { return seq; }

	virtual bool isHsuEndKeyTriggered();

	bool isHsuLayout;

protected:
    virtual const char *query(char c);
    char composebuf[256];
    char table[256];
};

struct IMGCandidate
{
    IMGCandidate();
    ~IMGCandidate();
    unsigned int count;
    char **candidates;
};

class OVIMTobacco;

class OVIMTobaccoContext : public OVInputMethodContext {
public:
    OVIMTobaccoContext(OVIMTobacco *p);
    virtual void start(OVBuffer*, OVCandidate*, OVService*);
    virtual void clear();
    virtual void end();
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);
    
protected:
    int keyCommit();
    int keyEsc();
    int keyRemove();
    int keyMove();
    int keyCompose();
    int keyPrintable();
    int keyNonRadical();
    int keyCapslock();
    int setCandidate();
    int fetchCandidate(const char *);
    int fetchCandidateWithPrefix(const char *prefix, char c);
    int isPunctuationCombination();
    int punctuationKey();
    int setPunctuation(string punctuationCharacters);
    int updateCandidateWindow();
    int closeCandidateWindow();
    int commitFirstCandidate();
    int candidateEvent();
    int candidatePageUp();
    int candidatePageDown();
    
    void freshBuffer();

    OVKeyCode *k;
    OVBuffer *b;
    OVCandidate *c;
    OVService *s;
    OVIMTobacco *parent;
    IMGKeySequence seq;
    IMGCandidate *candi;
    int page;
    
    size_t position;
    bool canNewSequence;
    bool doInsert;
};

class OVIMTobacco : public OVInputMethod {
public:
    OVIMTobacco(char *);
    virtual OVInputMethodContext *newContext();
    virtual int initialize(OVDictionary *, OVService *, const char *);
    virtual void update(OVDictionary *, OVService *);
    virtual const char *identifier();
    virtual const char *localizedName(const char *);
        
    virtual int maxSeqLen() { return cfgMaxSeqLen; }
    virtual bool doBeep() { return cfgBeep; }
    virtual bool doAutoCompose() { return cfgAutoCompose; }
    virtual bool doHitMaxAndCompose() { return cfgHitMaxAndCompose; }
    virtual bool doShiftSelKey() { return cfgDoShiftSelKey; };
    virtual bool doClearSequenceOnError() { return cfgDoClearSequenceOnError; };
    virtual bool doChooseInFrontOfCursor()
    {
        return cfgDoChooseInFrontOfCursor;
    };

protected:
    int isEndKey(char c);
    
    friend class OVIMTobaccoContext;
    int allowwildcard;
    char selkey[96];
    char endkey[96];
    char table[256];
    char idstr[256];
    char tsiDbFilePath[256];

    int cfgMaxSeqLen;
    bool cfgBeep;
    bool cfgAutoCompose;
    bool cfgHitMaxAndCompose;
    bool cfgDoShiftSelKey;
    
    bool cfgDoChooseInFrontOfCursor;
    bool cfgDoClearSequenceOnError;
};

/*
class OVOFReverseLookupSQLite : public OVOutputFilter
{
public:
    OVOFReverseLookupSQLite(char *);
    virtual const char* identifier();
    virtual const char* localizedName(const char* locale);
    virtual int initialize(OVDictionary*, OVService*, const char*);
    virtual const char* process(const char *src, OVService *srv);
protected:
    char table[256];
    char idstr[256];
    char composebuffer[1024];
};
*/

extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;
}
extern "C" int OVInitializeLibrary(OVService*, const char*p) { 
	Watch watch;
	watch.start();

    db=new SQLite3;  // this never gets deleted, but so do we
    char dbfile[PATH_MAX];
    sprintf(dbfile, "%s/OVIMTobacco/imtables.db", p);
    if (int err=db->open(dbfile)) {
        murmur("SQLite3 error! code=%d", err);
        return 0;
    }
    SQLite3Statement *sth=db->prepare("select name from tablelist;");
	//<comment author='b6s'>A workaround of loader singleton issue.
	IM_TABLES = 0;
	//</comment>
    while(sth->step()==SQLITE_ROW) {
		const char *buf = (char*)sth->column_text(0);
		IM_TABLE_NAMES[IM_TABLES] = (char*)calloc(1,strlen(buf)+1);
		strcpy(IM_TABLE_NAMES[IM_TABLES],buf);
		IM_TABLES++;
    }

	watch.stop();
	murmur("%1.3f sec:\tOVInitializeLibrary", watch.getSec());

    return 1;
}
extern "C" OVModule *OVGetModuleFromLibrary(int x) {
    if(x < IM_TABLES) {
	return new OVIMTobacco(IM_TABLE_NAMES[x]);
    }
	/*
	else if (x < IM_TABLES * 2) {
	int n = x - IM_TABLES;
	return new OVOFReverseLookupSQLite(IM_TABLE_NAMES[n]);
    }
	*/
    return NULL;
}

/*
OVOFReverseLookupSQLite::OVOFReverseLookupSQLite(char *name) {
    strcpy(table, name);
    sprintf(idstr,"OVOFReverseLookupSQLite-%s",name);
}

const char* OVOFReverseLookupSQLite::identifier() {
    return idstr;
}

const char* OVOFReverseLookupSQLite::localizedName(const char* lc) {
    static char buf[256];
    const char *name;
    if(!strcasecmp(lc,"zh_TW")) {
	name=QueryForKey(db, table, "_property_cname");
    } else {
	name=QueryForKey(db, table, "_property_ename");
    }
    sprintf(buf,"%s Lookup (SQLite)",name);
    return buf;
}

int OVOFReverseLookupSQLite::initialize(OVDictionary *cfg, OVService * s, const char *p) {
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
*/

OVIMTobacco::OVIMTobacco(char *name) {
    strcpy(table, name);
    sprintf(idstr,"OVIMTobacco-%s",name);
}

OVInputMethodContext *OVIMTobacco::newContext() {
    return new OVIMTobaccoContext(this);
}

int OVIMTobacco::initialize(OVDictionary *cfg, OVService * s, const char *p) {
    sprintf(tsiDbFilePath, "%sOVIMTobacco/tsi.db", p);

	murmur("initial predictor(%s)", tsiDbFilePath);

	Watch watch;
	watch.start();
    predictor = PredictorSingleton::getInstance(tsiDbFilePath);

    update(cfg, s);
	watch.stop();
	murmur("%1.3f sec:\tPredictorSingleton::getInstance(tsiDbFilePath)", watch.getSec());

	murmur("initialized");    
    return 1;        
}

void OVIMTobacco::update(OVDictionary *cfg, OVService *) {
    const char *warningBeep="warningBeep";
    const char *autoCompose="autoCompose";
    const char *maxSeqLen="maxKeySequenceLength";
    const char *hitMax="hitMaxAndCompose";

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

    cfgMaxSeqLen = cfg->getInteger(maxSeqLen);

    cfgBeep =
        cfg->getIntegerWithDefault(warningBeep, 1) == 0 ? false : true;
    cfgAutoCompose =
        cfg->getIntegerWithDefault(autoCompose, 0) == 0 ? false : true;
    cfgHitMaxAndCompose =
        cfg->getIntegerWithDefault(hitMax, 0) == 0 ? false : true;
    cfgDoShiftSelKey =
        cfg->getIntegerWithDefault("shiftSelectionKey", 0) == 0 ? false : true;
    
    cfgDoChooseInFrontOfCursor =
        cfg->getIntegerWithDefault("chooseInFrontOfCursor", 1)
            == 0 ? false : true;
    cfgDoClearSequenceOnError =
        cfg->getIntegerWithDefault("clearSequenceOnError", 1)
            == 0 ? false : true;
}

const char *OVIMTobacco::identifier() {
    return idstr;
}

const char *OVIMTobacco::localizedName(const char *lc) {
    static char buf[256];
    const char *name;
    if(!strcasecmp(lc,"zh_TW")) {
	name=QueryForKey(db, table, "_property_cname");
    } else {
	name=QueryForKey(db, table, "_property_ename");
    }
    sprintf(buf,"%s",name);
    return buf;
}

int OVIMTobacco::isEndKey(char c) {
    if (strchr(endkey, c)) return 1;
    return 0;
}

OVIMTobaccoContext::OVIMTobaccoContext(OVIMTobacco *p) : seq(p->table) {
    parent=p;
}

void OVIMTobaccoContext::start(OVBuffer* buf, OVCandidate*, OVService* s) {
	clear();
	b = buf;

	const char* ename = QueryForKey(db, parent->table, "_property_ename");
    	string inputMethodId(ename);
	predictor->setInputMethodId(inputMethodId);
}

void OVIMTobaccoContext::clear() {
	seq.clear();
	b=NULL;
	candi=NULL;
    
	position = 0;
	canNewSequence = false;
	doInsert = false;

	predictor->clearAll();
}

void OVIMTobaccoContext::end() {
	if(b) {
		if(!b->isEmpty()) b->send();
	}
	clear();
}

int OVIMTobaccoContext::keyEvent(OVKeyCode* pk, OVBuffer* pb, OVCandidate* pc, OVService* ps) {
    k=pk; b=pb; c=pc; s=ps;
    if (candi) return candidateEvent();
    if (isPunctuationCombination()) return punctuationKey();
    if (k->isFunctionKey() && b->isEmpty()) return 0;
    if (k->isCapslock()) {
		if(!b->isEmpty())
			keyCommit();

		return keyCapslock();
	}
    if (k->code()==ovkEsc) return keyEsc();
    if (k->code()==ovkBackspace || k->code()==ovkDelete) return keyRemove();
	if (k->code()==ovkSpace && !seq.isEmpty()) return keyCompose();
    if ((k->code()==ovkSpace || k->code()==ovkDown)
		&& !b->isEmpty() && seq.isEmpty())
		return setCandidate();
    if (k->code()==ovkReturn) return keyCommit();
    if (k->code()==ovkLeft || k->code()==ovkRight) return keyMove();
    if (isprint(k->code())) return keyPrintable();
    return 0;
}

int OVIMTobaccoContext::keyMove() {
	size_t currentBufferLength = predictor->tokenVector.size();
    if((seq.isEmpty() || parent->doAutoCompose()) && !b->isEmpty()
		&& position >= 0 && position <= currentBufferLength) {
        /// dirty hack for autoCompose mode... orz
        if(parent->doAutoCompose()) {
            if(!seq.isEmpty()) {
                seq.clear();
                position++;
            }
        }
    
        if(k->code()==ovkLeft) {
            if(position > 0)
                position--;
            else
                return 0;
        }
        else {
            if(position < predictor->tokenVector.size())
                position++;
            else
                return 0;
        }
        
        b->update((int)position, (int)position, (int)position);
        doInsert = true;

        return 1;
    }
    else
        return 0;
}

int OVIMTobaccoContext::keyCommit() {
    if (b->isEmpty()) return 0;
    if (!seq.isEmpty()) {
        if (keyCompose()) {
            b->send();
			clear();
            return 1;        
        }
        else
            return 0;
    }
    else
    {
        b->send();
		clear();
        return 1;
    }
}

int OVIMTobaccoContext::keyEsc() {
    if (seq.isEmpty()) return 0;     // if buffer is empty, do nothing
    seq.clear();                    // otherwise we clear the syllable
    freshBuffer();
    return 1;
}

int OVIMTobaccoContext::keyRemove() {
    if (b->isEmpty()) return 0;
    if (seq.isEmpty() || parent->doAutoCompose()) {
        /// dirty hack for autoCompose mode... orz
        if(parent->doAutoCompose()) {
            if(!seq.isEmpty()) {
                seq.clear();
                position++;
            }
        }
        
        if (k->code() == ovkDelete)
        {
            murmur("do delete");
            if (position < predictor->tokenVector.size())
                predictor->removeWord(position, true);
            else
                return 0;
        }
        else
        {
            murmur("do backspace");
            if (position > 0) {
                predictor->removeWord(position, false);
                position--;
            }
            else
                return 0;
        }
    }
    else
        seq.remove();

    doInsert = true;
    freshBuffer();

    return 1;
}

int OVIMTobaccoContext::keyPrintable() {
    if (isalpha(k->code()) && k->isShift() && b->isEmpty() && seq.isEmpty()) {
        char keystr[2];
        sprintf(keystr, "%c", tolower(k->code()));
        b->clear()->append(keystr)->send();
        return 1;
    }
    
    // wildcard hack: currently we don't allow ? * as the first character
	/* Conflict to punctuation.
    if (parent->allowwildcard && b->isEmpty() && 
        (k->code()=='?' || k->code()=='*')) return keyNonRadical();
	*/
 
    if (!seq.add(k->code())) {
        if (b->isEmpty()) return keyNonRadical();
        s->beep();
    }
    if (!seq.isEmpty()) {
		seq.isHsuLayout = false;
		const char* ename = parent->localizedName("en");
		if (strcmp("PhoneticHsu", ename) == 0) {
			seq.isHsuLayout = true;
			if(seq.isHsuEndKeyTriggered())
				return keyCompose();
		}
		else if (parent->isEndKey(k->code())) {
            freshBuffer();
            return keyCompose();
        }
        else if (parent->doAutoCompose()) {
            return keyCompose();
        }
    }
    freshBuffer();
    return 1;
}

void OVIMTobaccoContext::freshBuffer() {
/// BAD UTF-8 Chinese character processing here, again...

	size_t currentBufferLength = predictor->tokenVector.size();
	if(strlen(seq.sequence()) > 0 && position < currentBufferLength)
    {
        murmur("should be here with [%s], [%s], (%d)",
            seq.compose(), predictor->composedString.c_str(), position);
		string::size_type len = predictor->composedString.length();
        string leftString, rightString;
		if(!predictor->composedString.empty()) {
            leftString = 
                predictor->composedString.substr(0, (position)*3);
            rightString = 
                predictor->
                    composedString.substr((position)*3, len-(position)*3);
        }
		else {
            leftString = rightString = "";
			murmur("left[%s], right[%s]", leftString.c_str(), rightString.c_str());
		}

		b->clear()
			->append(leftString.c_str())
			->append(seq.compose())
			->append(rightString.c_str())
			->update((int)position, (int)position, (int)position + (int)seq.length());
    }
    else
	{
		b->clear();
		if(predictor->composedString.length() > 0)
			b->append(predictor->composedString.c_str());
		if(strlen(seq.sequence()) > 0)
			b->append(seq.compose());
		b->update((int)position, (int)position, (int)position + (int)seq.length());
	}
}

int OVIMTobaccoContext::keyNonRadical() {
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

int OVIMTobaccoContext::isPunctuationCombination() {
    // only accept CTRL-1 or CTRL-0
    if (k->isCtrl() && !k->isOpt() && !k->isCommand() &&
        (k->code()=='1' || k->code()=='0')) return 1;
    // only accept CTRL-OPT(ALT)-[printable]
    if (k->isCtrl() && (k->isOpt() || k->isAlt()) && !k->isShift() && 
        ((k->code() >=1 && k->code() <=26) || isprint(k->code()))) return 1;
    return 0;
}

int OVIMTobaccoContext::punctuationKey() {
    int count;
    char kc=k->code();
    if ((kc=='0' || kc=='1') && !k->isOpt())
        count=fetchCandidate("_punctuation_list");
    else {
        if (kc >= 1 && kc <= 26) kc+='a'-1;
        count=fetchCandidateWithPrefix("_ctrl_opt_", kc);
    }
    if (!count) return 0;       // we send back the combination key
    string punctuationCharacters("_ctrl_opt_" + kc);
    if (count > 0) return setPunctuation(punctuationCharacters);
    
    return 0;
}

int OVIMTobaccoContext::setPunctuation(string punctuationCharacters) {
    if (!candi) return 0;
    
    string punctuationString(candi->candidates[0]);
    predictor->setFixedToken(
        punctuationCharacters, punctuationString, position);
    seq.clear();
    position++;
    freshBuffer();

    delete candi;
    candi=NULL;
    /// candidate selecting does not work for punctuations currently...
    
    return 1;
}

int OVIMTobaccoContext::keyCapslock() {
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

int OVIMTobaccoContext::keyCompose() {
    murmur("key composing... [%s], (%d)", seq.compose(), position);
    std::string characterString(seq.compose());
    if(predictor->setTokenVector(
        characterString, position, parent->doAutoCompose()))
    {
        if(!parent->doAutoCompose()) {
            position++;
            doInsert = true;
            seq.clear();
        }
        canNewSequence = true;
        doInsert = false;
        freshBuffer();
        
        return 1;
    }
    else if(parent->doAutoCompose()) {
        if(strlen(seq.sequence()) > 1 && canNewSequence) {
            canNewSequence = false;
            seq.clear();
            seq.add(k->code());
            murmur("test here[%s]", seq.sequence());
            position++;
            doInsert = true;
        }
        freshBuffer();

        return 0;
    }
    else {
        s->beep();
        if(parent->doClearSequenceOnError()) seq.clear();
        freshBuffer();

        return 1;
    }
}


int OVIMTobaccoContext::fetchCandidateWithPrefix(const char *prefix, char c) {
    char keystr[64];
    sprintf(keystr, "%s%c", prefix, c);
    return fetchCandidate(keystr);
}


int OVIMTobaccoContext::fetchCandidate(const char *qs) {
    page=0;
    if (candi) {
        delete candi;
        candi=NULL;
    }
    
    int wildcard=0;
    char realqs[256];
    strcpy(realqs, qs);
    if (parent->allowwildcard && strlen(seq.sequence())>1) {
        size_t ql=strlen(realqs);
        for (size_t qi=0; qi<ql; qi++) {
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

int OVIMTobaccoContext::setCandidate() {
    murmur("start to set candidate at (%d)", position);
    page=0;
    if (candi) {
        delete candi;
        candi=NULL;
    }

    size_t choosingIndex = -1;
    if(parent->doChooseInFrontOfCursor()) {
        if(position == 0)
            choosingIndex = 0;
        else
            choosingIndex = position - 1;
    }
    else
    {
        if(position == predictor->tokenVector.size())
            choosingIndex = position - 1;
        else
            choosingIndex = position;
    }
        
    predictor->setCandidateVector(choosingIndex);
    /// Currently use "post" choosing mode only
    size_t rows = predictor->candidateVector.size();
    murmur("got %d rows", rows);
    if(!rows) return 1; //zonble
    candi=new IMGCandidate;
    candi->count=0;
    candi->candidates=new char* [rows];
    for(size_t i = 0; i < rows; i++)
        candi->candidates[candi->count++] =
            const_cast<char*>(predictor->candidateVector[i].word.c_str());

    return updateCandidateWindow();
}

int OVIMTobaccoContext::candidateEvent() {
    char kc=k->code();
    char *localSelKey;

    if (kc==ovkEsc || kc==ovkBackspace || kc==ovkDelete) {
    /// ESC/BKSP/DELETE cancels candi window
        seq.clear();
        return closeCandidateWindow();
    }

    if (parent->doShiftSelKey()) {
	localSelKey = (char*)calloc(1,strlen(parent->selkey) + 2);
	sprintf(localSelKey," %s",parent->selkey);
    } else {
	localSelKey = parent->selkey;
    }

    if ((kc==ovkSpace && !parent->doShiftSelKey())
	|| kc==ovkRight || kc==ovkDown || kc==ovkPageDown || kc =='>')
        return candidatePageDown(); 
    if (kc==ovkLeft || kc==ovkUp || kc==ovkPageUp || kc=='<')
        return candidatePageUp();

    size_t perpage=strlen(localSelKey);
    size_t i=0, l=perpage, nextsyl=0;
    for (i=0; i<perpage; i++) if(localSelKey[i]==kc) break;
    if (i==l) {         // not a valid candidate key
        if (kc==ovkReturn) i=0;
        if (seq.isValidKey(kc)) { i=0; nextsyl=1; }
    }
    if (i==l) {
        s->beep();
        b->update((int)position, (int)position, (int)position);    // we do this to make some applications happy
    }
    else {
        size_t choosingIndex;
        if(parent->doChooseInFrontOfCursor()) {
            if(position == 0)
                choosingIndex = 0;
            else
                choosingIndex = position - 1;
        }
        else
        {
            if(position == predictor->tokenVector.size())
                choosingIndex = position - 1;
            else
                choosingIndex = position;
        }
        murmur("set selected candidate(%d, %d)",
            choosingIndex, i + page*perpage);
        predictor->setSelectedCandidate(choosingIndex, i + page*perpage);
        b->clear()->append(predictor->composedString.c_str()); 
        closeCandidateWindow();
        if (nextsyl) {
            seq.add(kc);
            b->append(seq.compose());
			b->update((int)position, (int)position - 1, (int)position);
        }
		else
			b->update((int)position, (int)position, (int)position);
        
    }    
    return 1;
}

int OVIMTobaccoContext::updateCandidateWindow() {
    if (!candi) return 1;
    size_t candicount=candi->count;
    size_t perpage=strlen(parent->selkey);
    size_t pgstart=page*perpage;
        
    c->clear();
    char dispstr[32];
    
    for (size_t i=0; i<perpage; i++) {
        if (pgstart+i >= candicount) break;     // stop if idx exceeds candi counts
        sprintf(dispstr, "%c.", parent->selkey[i]);
        c->append(dispstr)->append(candi->candidates[page*perpage+i])->append(" ");
    }
    // add current page number
    sprintf(dispstr, "(%d/%d)", page+1, (candicount-1)/perpage +1);
    c->append(dispstr);
    c->update();
    if (!c->onScreen()) c->show();
    b->update((int)position, (int)position, (int)position);
    
    return 1;
}

int OVIMTobaccoContext::closeCandidateWindow() {
    seq.clear();
    if (c->onScreen()) c->hide()->clear()->update();
    if (candi) {
		//< b6s: this crashed win32 version.
        //delete candi;
        candi=NULL;
    }
    return 1;
}

int OVIMTobaccoContext::commitFirstCandidate() {
    if (!candi) return 1;
    b->clear()->append(candi->candidates[0])->send();
    return closeCandidateWindow();
}

int OVIMTobaccoContext::candidatePageUp() {
    int maxpage=(candi->count-1) / strlen(parent->selkey);
    if (!maxpage) s->beep();
    else {
        if (!page) page=maxpage; else page--;
        updateCandidateWindow();
    }
    return 1;
}

int OVIMTobaccoContext::candidatePageDown() {
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

bool IMGKeySequence::add(char c) {
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

	if(isHsuLayout) {
		string bpmfStr(composebuf);
		if(hsuKeyToBpmf(seq, bpmfStr)) {
			strcpy(composebuf, "");
			strcpy(composebuf, bpmfStr.c_str());
		}
	}
	return composebuf;
}

bool IMGKeySequence::isHsuEndKeyTriggered() {
	size_t lastIndex = strlen(seq) - 1;
	if(lastIndex > 0 && lastIndex < 4) {
		switch (seq[lastIndex]) {
			case 'd':
			case 'f':
			case 'j':
			case 's':
				return true;
			default:
				break;
		}
	}
	return false;
}

IMGCandidate::IMGCandidate()
{
    count=0;
    candidates=NULL;
}

IMGCandidate::~IMGCandidate()
{
    if (!count) return;
    for (unsigned int i=0; i<count; i++) delete candidates[i];
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
