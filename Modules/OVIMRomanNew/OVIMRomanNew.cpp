// OVIMRomanNew.cpp
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

#define OV_DEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "OVSQLite3.h"


int is_punc(char i){
	if(i >= '!' && i <= '@'){
		return 1;
	}
	if(i >= '[' && i <= '\''){
		return 1;
	}
	if(i >= '{' && i <= '~'){
		return 1;
	}
	return 0;
}

const size_t ebMaxKeySeq=40;
class KeySeq {
public:
    KeySeq() {
        clear();
    }
    void add(char c) {
        if (len == ebMaxKeySeq) return;
        buf[len++]=c;
        buf[len]=0;
    }
    void remove() {
        if (!len) return;
        buf[--len]=0;
    }
    void clear() {
        len=0; buf[0]=0;
    }
    char buf[ebMaxKeySeq];
    size_t len;
};

struct IMGCandidate
{
    IMGCandidate();
    ~IMGCandidate();
    unsigned int count;
    char **candidates;
};

SQLite3 *db;
IMGCandidate *candi;

const char *QueryForCommand(SQLite3 *db, const char *command);
const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key);


int showcandi(char* buf, OVCandidate* i) { //Show candidate list
    char cmd[256];
    char realqs[256];
    sprintf(cmd, "select value from %s where key like ?1 limit 20;", buf);
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
    
    if (candi){
        int candicount=candi->count;
        char dispstr[32];
        for (int j=0; j< 10; j++) {
            if (j >= candicount) break;     // stop if idx exceeds candi counts
            sprintf(dispstr, "%d.", j+1);
            i->append(dispstr)->append(candi->candidates[j])->append("\n");
        }
        i->update();
        if (!i->onScreen()) i->show();
        return 1;
    }
    return 0;
}


class OVIMRomanNewContext : public OVInputMethodContext
{
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*) {
        clear();
    }
    virtual void clear() {
		keyseq.clear();
    } 
    
    virtual int initialize(OVDictionary* l, OVService* s, const char* modulePath) {
		keyseq.clear();
        db=new SQLite3;  // this never gets deleted, but so do we
        char dbfile[128];
        sprintf(dbfile, "%s/OVIM/dict.db", modulePath);
        if (int err=db->open(dbfile)) {
            murmur("SQLite3 error! code=%d", err);
            return 0;
        }
        return 1;
    }

    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv) {
        
        // we only send printable characters so that they can pass thru 
        // full-width character filter (if enabled)
		
		if (k->code()==ovkSpace || k->code()==ovkReturn || is_punc(k->code())) {
            if (!(strlen(keyseq.buf))) return 0;   // empty buffer, do nothing
			if(k->code()!=ovkReturn) keyseq.add(k->code());
			b->clear()->append(keyseq.buf)->send();
			keyseq.clear();
            return 1;   // key processed
		}
		if (k->code()==ovkDelete || k->code()==ovkBackspace) {
			if(!strlen(keyseq.buf)) return 0;
			keyseq.remove();
			b->clear()->append(keyseq.buf)->update();
			return 1;
		}
		
		if (!isprint(k->code()) || k->isFunctionKey()) return 0;
		
		if(strlen(keyseq.buf) >= ebMaxKeySeq) return 1;
		
		if (isprint(k->code())){
			char s[2];
			sprintf(s, "%c", k->code());
			keyseq.add(k->code());
			if(keyseq.buf) {
                showcandi(keyseq.buf, i);
			}
			b->clear()->append(keyseq.buf)->update();
			return 1;
		}
		return 0;
    }
protected:
	KeySeq keyseq;
};

class OVIMRomanNew : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMRomanNew"; }
    virtual OVInputMethodContext *newContext() { return new OVIMRomanNewContext; }
    virtual int initialize(OVDictionary *, OVService*, const char *mp) {
        return 1;
    }
    virtual const char* localizedName(const char *locale) {
        if (!strcasecmp(locale, "zh_TW")) return "新英數";
        if (!strcasecmp(locale, "zh_CN")) return "新英数";
        return "New Roman (alphanumeric)";
    }
};

OV_SINGLE_MODULE_WRAPPER(OVIMRomanNew);

