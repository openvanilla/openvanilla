// OVIMRomanNew.cpp: New Roman Input Method, supports hunspell.
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

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

#include "OVOSDef.h"
/*
#ifdef WIN32
	#define strcasecmp stricmp
#endif
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
//#include "OVSQLite3.h"

#include "OVIMRomanNew.h"
#ifdef OSX_INCLUDE
        #include <Carbon/Carbon.h>
#endif

/*
SQLite3 *db;

const char *QueryForCommand(SQLite3 *db, const char *command);
const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key);
*/

void OVIMRomanNewContext::start(OVBuffer*, OVCandidate*, OVService* s)
{
	clear();
	
#ifdef OSX_INCLUDE
    char modulepath[PATH_MAX];
    CFBundleRef bundle = CFBundleGetBundleWithIdentifier(CFSTR("org.openvanilla.module.ovimromannewstatic"));
    if (!bundle) return;

    CFURLRef url = CFBundleCopyResourcesDirectoryURL(bundle);
    if (!url) return;

    CFURLGetFileSystemRepresentation(url, TRUE, (UInt8*)modulepath, sizeof(modulepath)-1);
	string affPath =
		string(modulepath) + "/" + parent->dict + ".aff";
	string dictPath =
		string(modulepath) + "/" + parent->dict + ".dic";
		
#else	
	string affPath =
		parent->modulePath + string(s->pathSeparator()) + parent->dict + ".aff";
	string dictPath =
		parent->modulePath + string(s->pathSeparator()) + parent->dict + ".dic";
#endif
	murmur(affPath.c_str());
	murmur(dictPath.c_str());	
	hunspellChecker
		= hunspell_initialize(
			const_cast<char*>(affPath.c_str()),
			const_cast<char*>(dictPath.c_str())
			);
}

int OVIMRomanNewContext::keyEvent(
	OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* s)
{
	if(!candi.isEmpty()) {
		if(k->code()==ovkEsc)
			return closeCandidateWindow(i);
		if (k->code()==ovkLeft || k->code()==ovkUp) {
			if(pagenumber > 0)
				pagenumber--;
			else
				pagenumber = pagetotal;
			return showcandi(i);
		}
		if (k->code()==ovkRight || k->code()==ovkDown) {
			if(pagenumber == pagetotal)
				pagenumber = 0;
			else
				pagenumber++;
			return showcandi(i);
		}
		if(k->code()==ovkTab){
			if(!candi.item(temp+pagenumber*10))
				return 0;
			b->clear()
				->append(keyseq.buf)
				->append(candi.item(temp+pagenumber*10) + keyseq.len)
				->update();
			if(temp++ > 8)
				temp = 0;
			if(temp+pagenumber*10 > candi.count())
				temp = 0;
			return 1;
		}
		if(is_selkey(k->code())) {
			murmur("SelectKey Pressed: %c",k->code());
			int n = (k->code() - '1' + 10) % 10;
			if(n+pagenumber*10 >= candi.count())
				return 0;
			const char* correctedWord = candi.item(n+pagenumber*10);

			b->clear()->append(correctedWord)->append(" ")->send();
			if (i->onScreen())
				i->hide();
			keyseq.clear();
			return closeCandidateWindow(i);
		}
		if (k->code()==ovkSpace || k->code()==ovkReturn || is_punc(k->code()))
		{
			if (!(strlen(keyseq.buf)))
				return 0; // empty buffer, do nothing

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

	if(k->code() == ovkEsc) {
		b->clear()->update();
		clear();
		return 1;
	}
	if (k->code() == ovkReturn)
	{
		if(!(strlen(keyseq.buf)))
			return 0;

		if (keyseq.buf)
		{
			if(!(strlen(keyseq.buf)))
				return 0;

			keyseq.clear();
			//keyseq.add(k->code());
			b->append(keyseq.buf)->send();
			keyseq.clear();
			return 1;
		}
	}
	if (k->code()==ovkSpace || is_punc(k->code())) {
		if (!(strlen(keyseq.buf))) return 0;   // empty buffer, do nothing            

		if(keyseq.buf) {
			pagenumber = 0;
			if(!isEnglish(keyseq.buf) &&
				spellCheckerByHunspell(keyseq.buf))
			{
				showcandi(i);
				return 1;
			}

			if (!(strlen(keyseq.buf)))
				return 0;
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
				spellCheckerByHunspell(keyseq.buf))
			{
				showcandi(i);
				return 1;
			} else {
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
				spellCheckerByHunspell(keyseq.buf))
			{
				showcandi(i);
				return 1;
			} else {
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
				spellCheckerByHunspell(keyseq.buf))
			{
				showcandi(i);
				return 1;
			} else {
				closeCandidateWindow(i);
			}
		}
		b->clear()->append(keyseq.buf)->update();
		return 1;
	}
	return 0;
}

bool OVIMRomanNewContext::isEnglish(char* buf) {
	return hunspell_spell((Hunspell*)hunspellChecker, (buf)) != 0 ? true : false;

}

size_t OVIMRomanNewContext::spellCheckerByHunspell(char* buf)
{
    //char* result;
	char** pString_array;
	int numSuggestWord = -1;

    pagenumber=0;
    pagetotal=0;
    candi.clear();
   		
	numSuggestWord =
#ifdef WIN32
		hunspell_suggest((Hunspell*)hunspellChecker, (buf), &pString_array); 
#else
		hunspell_suggest((Hunspell*)hunspellChecker, &pString_array, (buf));
#endif
	if(numSuggestWord > 0)
	{
		for(int i= 0; i <numSuggestWord; i++)
		{
			candi.add(string(*(pString_array + i)));
		}
	}
	//initial the intoDoubleQ is false.
	//bool intoDoubleQ = false;
	//char word[15];
	//memset(word, 0, sizeof(word));
	//do
	//{
	//	if(&result[i] == '"')
	//	{
	//		if(intoDoubleQ == true)
	//		{
	//			//word[j] = '@';
	//			//word[j+1] = 0;
	//	 candi.add(string(word));

	//			j = 0;
	//		}
	//		intoDoubleQ = !intoDoubleQ;


	//	}
	//	else if(result[i] == ',')
	//	{
	//		//do nothing
	//	}
	//	else
	//	{
	//		word[j] = result[i];
	//		j++;
	//	}

	//	i++;
	//	murmur("RomanNew Suggest array [I] : %d", i);
	//}while(result[i] != '\0');

	//const AspellWordList * suggestions =
	//	 aspell_speller_suggest(aspell_checker, static_cast<const char*>(buf), -1);
 //    AspellStringEnumeration * aspell_elements = aspell_word_list_elements(suggestions);
   //  const char * word;
   //  while ( (word = aspell_string_enumeration_next(aspell_elements)) != NULL )
		 //candi.add(string(word));

   //  delete_aspell_string_enumeration(aspell_elements);

	//original
	//candi.add(string(result));

	//Jaimie Test
	//candi.add("gi");
	//candi.add("gi2");
     //candi.count() = 10;
	 pagetotal=candi.count()/10;
	 
	 return candi.count();
}
//size_t OVIMRomanNewContext::spellCheckerByAspell(char* buf)
//{
//    pagenumber=0;
//    pagetotal=0;
//    candi.clear();
//
//     const AspellWordList * suggestions =
//		 aspell_speller_suggest(aspell_checker, static_cast<const char*>(buf), -1);
//     AspellStringEnumeration * aspell_elements = aspell_word_list_elements(suggestions);
//     const char * word;
//     while ( (word = aspell_string_enumeration_next(aspell_elements)) != NULL )
//		 candi.add(string(word));
//
//     delete_aspell_string_enumeration(aspell_elements);
//
//	 pagetotal=candi.count()/10;
//
//	 return candi.count();
//}

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

    char dispstr[128];
    const char *selkey="1234567890";
    i->clear();
    
    size_t total=candi.count();
    for (size_t j=0; j<10; j++) {
        if (j+pagenumber*10 >= total) break;
        sprintf(dispstr, "%c.", selkey[j]);
#ifdef WIN32
        i->append(dispstr)->append(candi.item(j+pagenumber*10))->append("\t");
#else
        i->append(dispstr)->append(candi.item(j+pagenumber*10))->append("\n");
#endif
    }
    
    sprintf(dispstr, "(%ld/%ld)", pagenumber + 1, pagetotal + 1);
    i->append(dispstr)->update()->show();

	isCandiOnDuty = true;

    return 1;
}
