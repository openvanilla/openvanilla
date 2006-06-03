// OVIMRomanNew.cpp
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

#define OV_DEBUG
#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
	#include <OpenVanilla/OVLibrary.h>
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OpenVanilla.h"
	#include "OVLibrary.h"
	#include "OVUtility.h"
#endif
	
#ifdef WIN32
	#define strcasecmp stricmp
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
//#include "OVSQLite3.h"

//#include "LuceneSearch.h"
#include "aspell.h"

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
    size_t count() { return candidates.size(); }
    void clear() { candidates.clear(); }
    void add(const string& s) { candidates.push_back(s); }
    const char *item(size_t i) { return candidates[i].c_str(); }
    vector<string>& vectorInstance() { return candidates; }
protected:    
    vector<string> candidates;
};

/*
SQLite3 *db;

const char *QueryForCommand(SQLite3 *db, const char *command);
const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key);
*/

string modulePath;

AspellCanHaveError* aspell_possible_err = 0;
AspellSpeller* aspell_checker = 0;
AspellConfig* aspell_config = 0;

class OVIMRomanNewContext : public OVInputMethodContext
{
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*)
	{
		clear();

		aspell_config = new_aspell_config();
		aspell_config_replace(aspell_config, "lang", "en_US");		

		aspell_possible_err = new_aspell_speller(aspell_config);
		if (aspell_error_number(aspell_possible_err) != 0)
			puts(aspell_error_message(aspell_possible_err));
		else
			aspell_checker = to_aspell_speller(aspell_possible_err);
	}
    virtual void clear() { keyseq.clear();} 

	virtual void end() { delete_aspell_speller(aspell_checker); }
    virtual int keyEvent(
        OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* s)    
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
            if(k->code()==ovkTab){
                if(!candi.item(temp+pagenumber*10)) return 0;
                b->clear()->append(keyseq.buf)->append(candi.item(temp+pagenumber*10) + keyseq.len)->update();
                if(temp++ > 8) temp = 0;
                if(temp+pagenumber*10 > candi.count()) temp = 0;
                return 1;
            }
            if(is_selkey(k->code())){
		        murmur("SelectKey Pressed: %c",k->code());
                int n = (k->code() - '1' + 10) % 10;
                if(n+pagenumber*10 >= candi.count()) return 0;
				const char* correctedWord = candi.item(n+pagenumber*10);

				aspell_speller_store_replacement(
					aspell_checker, static_cast<const char*>(keyseq.buf), -1, correctedWord, -1);

                b->clear()->append(correctedWord)->append(" ")->send();
		    	if (i->onScreen()) i->hide();
		    	keyseq.clear();
		    	return closeCandidateWindow(i);
		    }
		    
            if (k->code()==ovkSpace ||
                k->code()==ovkReturn ||
                is_punc(k->code()))
            {
                if (!(strlen(keyseq.buf))) return 0; // empty buffer, do nothing            

    			b->append(" ")->send();
                keyseq.clear();
	       		return closeCandidateWindow(i);
    		}

			if(!b->isEmpty() && isprint(k->code())) {
				b->append(" ")->send();
				keyseq.clear();
				keyseq.add(k->code());
				b->clear()->append(keyseq.buf)->update();
				return closeCandidateWindow(i);
			}
		}

		if(is_selkey(k->code())){
		    murmur("SelectKey Pressed: %c",k->code());
            int n = (k->code() - '1' + 10) % 10;
			const char* correctedWord = candi.item(n+pagenumber*10) + keyseq.len;

			aspell_speller_store_replacement(
				aspell_checker, static_cast<const char*>(keyseq.buf), -1, correctedWord, -1);

            b->clear()->append(keyseq.buf)->append(correctedWord)->append(" ")->send();

			if (i->onScreen()) i->hide();
			keyseq.clear();
			return closeCandidateWindow(i);
		}

		if (k->code()==ovkSpace || k->code()==ovkReturn || is_punc(k->code())) {
            if (!(strlen(keyseq.buf))) return 0;   // empty buffer, do nothing            

            if(keyseq.buf) {
                pagenumber = 0;
                if(!isEnglish(keyseq.buf) &&
					//spellCheckerByLuceneFuzzySearch(keyseq.buf))
					spellCheckerByAspell(keyseq.buf))
                {
					showcandi(i);
					return 1;
                }

				if (!(strlen(keyseq.buf))) return 0;
				keyseq.clear();
				keyseq.add(k->code());
				b->append(keyseq.buf)->send();
				keyseq.clear();
          		return closeCandidateWindow(i);
			} else {
    			b->send();
                return 0;
			}
		}

		if (k->code()==ovkDelete || k->code()==ovkBackspace) {
			if(!strlen(keyseq.buf)) { closeCandidateWindow(i); return 0;}
			keyseq.remove();
            if(keyseq.len && i->onScreen()) {
                pagenumber = 0;
                if(!isEnglish(keyseq.buf) &&
					spellCheckerByAspell(keyseq.buf))
				{
					showcandi(i);
					return 1;
                }
				else {
					closeCandidateWindow(i);
				}
            } else {
                closeCandidateWindow(i);
            }
			b->clear()->append(keyseq.buf)->update();
			return 1;
		}
		
		if(k->code()==ovkTab){
            if(keyseq.buf) {
                pagenumber = 0;
                if(!isEnglish(keyseq.buf) &&
					spellCheckerByAspell(keyseq.buf))
				{
					showcandi(i);
					return 1;
                }
				else {
					closeCandidateWindow(i);
				}
			} else {
                return 0;
			}
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
            if(keyseq.buf && i->onScreen()) {
                pagenumber = 0;
                temp = 0;
                if(!isEnglish(keyseq.buf) &&
					spellCheckerByAspell(keyseq.buf))
				{
					showcandi(i);
					return 1;
                }
				else {
					closeCandidateWindow(i);
				}
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
    //int updatepagetotal(char* buf);
    //int spellCheckerBySQLiteSoundex(char* buf);
    //int spellCheckerByLuceneFuzzySearch(char* buf);
	size_t spellCheckerByAspell(char* buf);
    bool isEnglish(char* buf);

protected:
	KeySeq keyseq;
    IMGCandidate candi;
    size_t pagenumber;
    size_t pagetotal;
    size_t temp;    
};

bool OVIMRomanNewContext::isEnglish(char* buf) {
	return aspell_speller_check(aspell_checker, static_cast<const char*>(buf), -1) != 0 ? true : false;
}

size_t OVIMRomanNewContext::spellCheckerByAspell(char* buf)
{
    pagenumber=0;
    pagetotal=0;
    candi.clear();

     const AspellWordList * suggestions =
		 aspell_speller_suggest(aspell_checker, static_cast<const char*>(buf), -1);
     AspellStringEnumeration * aspell_elements = aspell_word_list_elements(suggestions);
     const char * word;
     while ( (word = aspell_string_enumeration_next(aspell_elements)) != NULL )
		 candi.add(string(word));

     delete_aspell_string_enumeration(aspell_elements);

	 pagetotal=candi.count()/10;

	 return candi.count();
}

/*
bool OVIMRomanNewContext::isEnglish(char* buf) {
    char cmd[256];

	string word(buf);
	for(int i = 0; i < word.length(); i++)
		word[i] = tolower(word[i]);

	sprintf(cmd, "select count(key) from dict where key = '%s';", word.c_str());
    SQLite3Statement *sth=db->prepare(cmd);
    if (!sth) return false;

    int amount = 0;
    if(sth->step()==SQLITE_ROW)
        amount = sth->column_int(0);
    delete sth;
    
    if(amount > 0)  return true;
    else            return false;
}

int OVIMRomanNewContext::spellCheckerByLuceneFuzzySearch(char* buf)
{
    pagenumber=0;
    pagetotal=0;
    candi.clear();
    
    std::string query(buf);
    query += "~";
    LuceneSearch::run(query, modulePath, candi.vectorInstance());

	pagetotal=candi.count()/10;
    
    return candi.count();
}

int OVIMRomanNewContext::spellCheckerBySQLiteSoundex(char* buf){
    pagenumber=0;
    pagetotal=0;
    candi.clear();

    char cmd[256];
    sprintf(cmd, "select key from dict where soundex(key) = soundex('%s');", buf);
    SQLite3Statement *sth=db->prepare(cmd);
    if (!sth) return 0;

    int row = 0;
    while (sth->step()==SQLITE_ROW) row++;
    
    if(row > 0) {
        sth->reset();
        while (sth->step()==SQLITE_ROW) {
            candi.add(string(sth->column_text(0)));
        }
        delete sth;
        pagetotal=candi.count()/10;
        return 1;
    }
    else {
        delete sth;
        return 0;
    }
}

int OVIMRomanNewContext:: updatepagetotal(char* buf){
    pagenumber=0;
    pagetotal=0;
    candi.clear();
    if (strlen(buf) < 3) return 0;

    char cmd[256];
    sprintf(cmd, "select * from dict where key like '%s%%' order by freq desc;", buf);
    SQLite3Statement *sth=db->prepare(cmd);
    if (!sth) return 0;

    while (sth->step()==SQLITE_ROW) {
        candi.add(string(sth->column_text(0)));
    }
    pagetotal=candi.count()/10;
    return 1;
}
*/

int OVIMRomanNewContext::showcandi(OVCandidate* i) {
    if (!candi.count()) {
        if (i->onScreen()) i->hide();
        return 1;
    }

    char dispstr[32];    
    const char *selkey="1234567890";
    i->clear();
    
    size_t total=candi.count();
    for (size_t j=0; j<10; j++) {
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
    virtual int initialize(OVDictionary* l, OVService* s, const char* mp)
    {
        string idString(identifier());
        modulePath = mp + idString + "/";
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
	/*
    db=new SQLite3;  // this never gets deleted, but so do we
    char dbfile[128];
    sprintf(dbfile, "%sOVIMRomanNew/dict.db", p);
    murmur("DBPath: %s",dbfile);
    if (int err=db->open(dbfile)) {
        murmur("SQLite3 error! code=%d", err);
        return 0;
    }
	*/

    return 1;
}
extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    return (idx==0) ? new OVIMRomanNew : NULL;
}
