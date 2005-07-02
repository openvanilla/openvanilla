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

int is_selkey(char i){
	if(i >= '0' && i <= '9'){ return 1; }
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
IMGCandidate::IMGCandidate(){    count=0;    candidates=NULL;}IMGCandidate::~IMGCandidate(){    if (!count) return;    for (unsigned int i=0; i<count; i++) delete candidates[i];    delete[] candidates;}

SQLite3 *db;

const char *QueryForCommand(SQLite3 *db, const char *command);
const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key);

class OVIMRomanNewContext : public OVInputMethodContext
{
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*) { clear(); }
    virtual void clear() { keyseq.clear();} 
    
    virtual int initialize(OVDictionary* l, OVService* s, const char* modulePath) {
		keyseq.clear();
        return 1;
    }
        int closeCandidateWindow(OVCandidate* c) {        if (c->onScreen()) c->hide()->clear()->update();        if (candi) {            delete candi;            candi=NULL;        }        return 1;            }

    int showcandi(char* buf, OVCandidate* i);

    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv) {
        
        // we only send printable characters so that they can pass thru 
        // full-width character filter (if enabled)
		/*
		if (k->code()==ovkLeft) {
		  if(pagenumber > 0) {pagenumber--;}
		  if(keyseq.buf) { showcandi(keyseq.buf, i); }
		}
		if (k->code()==ovkRight) {
		  if(pagenumber < pagetotal) {pagenumber++;}
		  if(keyseq.buf) { showcandi(keyseq.buf, i); }
		}*/

		if(is_selkey(k->code())){
		    murmur("SelectKey Pressed: %c",k->code());
            int n = (k->code() - '1' + 10) % 10;
            b->clear()->append(candi->candidates[n])->send();
			if (i->onScreen()) i->hide();
			keyseq.clear();
			return closeCandidateWindow(i);
		}

		if (k->code()==ovkSpace || k->code()==ovkReturn || is_punc(k->code())) {
            if (!(strlen(keyseq.buf))) return 0;   // empty buffer, do nothing
			if(k->code()!=ovkReturn) keyseq.add(k->code());
			b->clear()->append(keyseq.buf)->send();
			keyseq.clear();
			return closeCandidateWindow(i);
		}

		if (k->code()==ovkDelete || k->code()==ovkBackspace) {
			if(!strlen(keyseq.buf)) return 0;
			keyseq.remove();
            if(keyseq.buf) { showcandi(keyseq.buf, i); }
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
            murmur("FOO A");
			b->clear()->append(keyseq.buf)->update();
            murmur("FOO B");
			return 1;
		}
		return 0;
    }


protected:
	KeySeq keyseq;
    IMGCandidate *candi;
    int pagenumber;
    int pagetotal;
};

int OVIMRomanNewContext::showcandi(char* buf, OVCandidate* i) { //Show candidate list
    char cmd[256];
    // sprintf(cmd, "select key from dict where key like '%s%%' limit %d,10;",buf, pagenumber*10 );
    sprintf(cmd, "select key from dict where key like '%s%%' limit 0,10;",buf);
    murmur("executing command=%s", cmd);

    SQLite3Statement *sth=db->prepare(cmd);
    if (!sth) return 0;
    int rows=0;
    while (sth->step()==SQLITE_ROW) {
        rows++;
    }
    if(!rows) { delete sth; return 0;}

    candi = new IMGCandidate;
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
    if (!rows) { return 0; }

    if (candi){
        char selkey[11] = "1234567890";
        int candicount=candi->count;
        char dispstr[32];
        i->clear();
        for (int j=0; j< 10; j++) {
            if (j >= candicount) break;     // stop if idx exceeds candi counts
            sprintf(dispstr, "%c.", selkey[j]);
            i->append(dispstr)->append(candi->candidates[j])->append("\n");
        }
        i->update();
        if (!i->onScreen()) i->show();
    }
    return 1;
}

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

extern "C" unsigned int OVGetLibraryVersion() { return OV_VERSION; }extern "C" int OVInitializeLibrary(OVService*, const char* p) {
    db=new SQLite3;  // this never gets deleted, but so do we
    char dbfile[128];
    sprintf(dbfile, "%sOVIMRomanNew/dict.db", p);
    murmur("DBPath: %s",dbfile);
    if (int err=db->open(dbfile)) {
        murmur("SQLite3 error! code=%d", err);
        return 0;
    }
    return 1;
}extern "C" OVModule *OVGetModuleFromLibrary(int idx) {    return (idx==0) ? new OVIMRomanNew : NULL;}