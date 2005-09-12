// OVOFReverseLookup.cpp 

#define OV_DEBUG
#ifndef WIN32
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#else
#include "OpenVanilla.h"
#include "OVLibrary.h"
#include "OVUtility.h"
#define strcasecmp stricmp
#endif
#include <stdlib.h>
#include "OVCandidateList.h"
#include "OVOFReverseLookup.h"
#include "OVCIN.h"
#include <string>
#include <iostream>

using namespace std;

// functions that help load all .cin --------------------------------
CinList cinlist;

// OpenVanilla Loadable IM interface functions -------------------------------

extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;
}
extern "C" int OVInitializeLibrary(OVService*, const char*p) { 
    cinlist.load((char*)p);
    return 1; 
}
extern "C" OVModule *OVGetModuleFromLibrary(int x) {
    if (x >= cinlist.index) return NULL;
    return new OVOFReverseLookup(cinlist.cinpath, cinlist.list[x].filename, cinlist.list[x].ename, cinlist.list[x].cname);
}


OVOFReverseLookup::OVOFReverseLookup(char *lpath, char *cfile, char *en, char *cn) {
    char cinfilename[PATH_MAX];
    strcpy(cinfile, cfile);
    strcpy (cinfilename, lpath);
    if (cinfilename[strlen(cinfilename)-1]!='/') strcat(cinfilename, "/");
    strcat(cinfilename, cinfile);
    cintab=new OVCIN(cinfilename);

    sprintf(ename, "%s lookup", en ? en : cfile);
    sprintf(cname, "查詢%s字根", cn ? cn : cfile);
    sprintf(idbuf, "OVOFReverseLookup-%s", en ? en : cfile);
}

OVOFReverseLookup::~OVOFReverseLookup() {
    if (cintab) delete cintab;
}

const char* OVOFReverseLookup::identifier() {
    return idbuf;
}

const char* OVOFReverseLookup::localizedName(const char* locale) {
    if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN")) {
        return cname;
    }        
    return ename;
}

int OVOFReverseLookup::initialize(OVDictionary* global, OVService*, const char*) {
    if (!cintab) return 0;
    murmur("OVOFReverseLookup: initializing %s", identifier());
    return 1;
}

const char *OVOFReverseLookup::process(const char *src, OVService *srv) {
    unsigned short *u16;
    int u16len=srv->UTF8ToUTF16(src, &u16);
    
    string result;
    
    // WE HAVE TO DO SURROGATE CHECK, REMEMBER!
    for (int i=0; i<u16len; i++) {
        // get each codepoint
        const char *u8=srv->UTF16ToUTF8(&(u16[i]), 1);
        char buf[512];
        
        vector<string> lookupvector, key;
        string code;
        string seperator(" ");

        // look up each "candidate"
        int size=cintab->getWordVectorByChar(u8, lookupvector);
        for (int j=0; j<size; j++) {
            const char *lvstr=lookupvector[j].c_str();
            
            // compose the "character parts"
            for (size_t k=0; k<strlen(lvstr); k++) {
                char kbuf[2];
                sprintf(kbuf, "%c", tolower(lvstr[k]));
                string kstr(kbuf);
    
                vector<string> outStringVectorRef;
                if(cintab->getCharVectorByKey(kstr, outStringVectorRef) > 0)
                    code+=outStringVectorRef[0];
                else
                    code+=kstr;
            }
        
            if (j!=size-1) code += seperator;
        }
    
        if (size) {
            // if( size > 1 ) code = "\n" + code;
            sprintf(buf, "%s: %s", u8, code.c_str());
            string bstr(buf);
            if (result.size()) result += "\n";
            result += bstr;
    	}
    }
    
    if(strlen(result.c_str())) srv->notify(result.c_str());
    return src;
}
    
/*        
int OVXcinContext::compose(OVBuffer *buf, OVCandidate *textbar, OVService *srv)
{
    if (!keyseq.length()) return 0;

	int size =
		cintab->getWordVectorByChar(keyseq.getSeq(), candidateStringVector);

    if (size == 0)
    {
        if (parent->isBeep()) srv->beep();
        return 1;
    }

    if (size ==1 && !autocomposing)
    {
        buf->clear()->append(candidateStringVector[0].c_str())->send();
        keyseq.clear();
        return 1;
    }

    if (!autocomposing)
    {    
        buf->clear()->append(candidateStringVector[0].c_str())->update();
        keyseq.clear();
    }
	
	string currentSelKey = cintab->getSelKey();
	if(parent->isShiftSelKey())
		currentSelKey = " " + currentSelKey;

    candi.prepare(&candidateStringVector,
				  const_cast<char*>(currentSelKey.c_str()), textbar);

    return 1;
}

int OVXcinContext::candidateEvent(OVKeyCode *key, OVBuffer *buf, 
    OVCandidate *textbar, OVService *srv)
{
    if (key->code() == ovkEsc || key->code() == ovkBackspace)
    {
        textbar->hide()->clear();
        candi.cancel();
        buf->clear()->update();
        return 1;
    }

    if (key->code() == ovkDown || key->code() == ovkRight ||
        (!candi.onePage() && key->code()==ovkSpace))
    {
        candi.pageDown()->update(textbar);
        return 1;
    }

    if (key->code() == ovkUp || key->code() == ovkLeft)
    {
        candi.pageUp()->update(textbar);
        return 1;
    }

    // enter == first candidate
    // space (when candidate list has only one page) == first candidate
    char c=key->code();
    if (key->code() == ovkReturn || 
        (candi.onePage() && key->code()==ovkSpace)) c=candi.getSelKey()[0];

    string output;
    if (candi.select(c, output)) {
        buf->clear()->append(output.c_str())->send();
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }

	string inKey;
	inKey.push_back(c);
    if (cintab->isValidKey(inKey) || cintab->isEndKey(c)) {
        string output;
        candi.select(candi.getSelKey()[0], output);

        buf->clear()->append(output.c_str())->send();
		keyseq.add(c);
		updateDisplay(buf);
		candi.cancel();
		textbar->hide()->clear();
		if(cintab->isEndKey(c))
			compose(buf, textbar, srv);
		
		return 1;			
    }

    if (parent->isBeep()) srv->beep();

    return 1;
}
*/
