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
#include <string>
#include <vector>
#include "OVSQLite3.h"

using namespace std;

int is_punc(char i){
	if(i >= '!' && i <= '@')  return 1;
	if(i >= '[' && i <= '\'') return 1;
	if(i >= '{' && i <= '~') return 1;
	return 0;
}

int is_selkey(char i){
	if(i >= '0' && i <= '9') return 1;
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

class IMGCandidate
{
public:
    int count() { return candidates.size(); }
    void clear() { candidates.clear(); }
    void add(const string& s) { candidates.push_back(s); }
    const char *item(size_t i) { return candidates[i].c_str(); }
protected:    
    vector<string> candidates;
};

SQLite3 *db;

const char *QueryForCommand(SQLite3 *db, const char *command);
const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key);

class OVIMRomanNewContext : public OVInputMethodContext
{
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*) { clear(); }
    virtual void clear() { keyseq.clear();} 
    
    virtual int initialize(OVDictionary* l, OVService* s, 
        const char* modulePath) { return 1; }
    
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService*)    
    {
        if(candi.count()) {
            if (k->code()==ovkLeft || k->code()==ovkUp) {
                if(pagenumber > 0) pagenumber--;
                return showcandi(i);
    		}
    		if (k->code()==ovkRight || k->code()==ovkDown) {
                if(pagenumber < pagetotal + 1) pagenumber++;
                return showcandi(i);
    		}
		}

		if(is_selkey(k->code())){
		    murmur("SelectKey Pressed: %c",k->code());
            int n = (k->code() - '1' + 10) % 10;
            b->clear()->append(keyseq.buf)->append(candi.item(n+pagenumber*10) + keyseq.len)->append(" ")->send();
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
			if(!strlen(keyseq.buf)) { closeCandidateWindow(i); return 0;}
			keyseq.remove();
            if(keyseq.len) { 
                updatepagetotal(keyseq.buf);
                showcandi(i);
            } else {
                closeCandidateWindow(i);
            }
			b->clear()->append(keyseq.buf)->update();
			return 1;
		}
		
		if (!isprint(k->code()) || k->isFunctionKey()) {
		   closeCandidateWindow(i);
		   return 0;
        }
		
		if(strlen(keyseq.buf) >= ebMaxKeySeq) return 1;
		
		if (isprint(k->code())){
			char s[2];
			sprintf(s, "%c", k->code());
			keyseq.add(k->code());
			if(keyseq.buf) {
                pagenumber = 0;
                updatepagetotal(keyseq.buf);
                showcandi(i);
			}
			b->clear()->append(keyseq.buf)->update();
			return 1;
		}
		return 0;
    }

protected:
    int closeCandidateWindow(OVCandidate* c) {
        if (c->onScreen()) c->hide()->clear()->update();
        candi.clear();
        return 1;        
    }

    int showcandi(OVCandidate* i);
    int updatepagetotal(char* buf);

protected:
	KeySeq keyseq;
    IMGCandidate candi;
    int pagenumber;
    int pagetotal;
};

int OVIMRomanNewContext:: updatepagetotal(char* buf){
    pagenumber=0;
    pagetotal=0;
    candi.clear();
    if (strlen(buf) < 3) return 0;

    char cmd[256];
    sprintf(cmd, "select * from dict where key like '%s%%';", buf);
    SQLite3Statement *sth=db->prepare(cmd);
    if (!sth) return 0;

    while (sth->step()==SQLITE_ROW) {
        candi.add(string(sth->column_text(0)));
    }
    pagetotal=candi.count()/10;
    return 1;
}

int OVIMRomanNewContext::showcandi(OVCandidate* i) {
    if (!candi.count()) {
        if (i->onScreen()) i->hide();
        return 1;
    }

    char dispstr[32];    
    const char *selkey="1234567890";
    i->clear();
    
    int total=candi.count();
    for (int j=0; j<10; j++) {
        if (j+pagenumber*10 >= total) break;
        sprintf(dispstr, "%c.", selkey[j]);
        i->append(dispstr)->append(candi.item(j+pagenumber*10))->append("\n");
    }
    
    sprintf(dispstr, "(%d/%d)", pagenumber + 1, pagetotal + 1);
    i->append(dispstr)->update()->show();    
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

extern "C" unsigned int OVGetLibraryVersion() { return OV_VERSION; }
extern "C" int OVInitializeLibrary(OVService*, const char* p) {
    db=new SQLite3;  // this never gets deleted, but so do we
    char dbfile[128];
    sprintf(dbfile, "%sOVIMRomanNew/dict.db", p);
    murmur("DBPath: %s",dbfile);
    if (int err=db->open(dbfile)) {
        murmur("SQLite3 error! code=%d", err);
        return 0;
    }
    return 1;
}
extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    return (idx==0) ? new OVIMRomanNew : NULL;
}
