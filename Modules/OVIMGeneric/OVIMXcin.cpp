// OVIMXcin.cpp

#define OV_DEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <stdlib.h>
#include "OVCandidateList.h"
#include "OVIMXcin.h"
#include "OVCIN.h"
#include "XcinCinList.h"
#include <string>
#include <iostream>

using namespace std;

//OVLOADABLEOBJCWRAPPER;

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
    return new OVIMXcin(cinlist.cinpath, cinlist.list[x].filename, cinlist.list[x].ename, cinlist.list[x].cname);
}

/*
extern "C" int OVLoadableAvailableIMCount(char* p)
{
    cinlist.load(p);
    return cinlist.index;
}


extern "C" unsigned int OVLoadableVersion()
{
    return ovVersion;
}


extern "C" OVInputMethod* OVLoadableNewIM(int x)
{
    return new OVIMXcin(cinlist.cinpath, cinlist.list[x].filename,
        cinlist.list[x].ename, cinlist.list[x].cname);
}

extern "C" void OVLoadableDeleteIM(OVInputMethod *im)
{
    delete im;
}

*/

XcinKeySequence::XcinKeySequence(OVCIN* cintab)
{
    cinTable=cintab;
}
    
int XcinKeySequence::valid(char c)
{
	string inKey;
	inKey.push_back(c);
    if (!cinTable->isValidKey(inKey)) return 0;
    return 1;
}
    
int XcinKeySequence::add(char c)
{
    if (valid(c) == 0) return 0;
    return OVKeySequenceSimple::add(c);
}
    
string *XcinKeySequence::compose(string *s)
{
    for (int i=0; i<len; i++)
    {
		string inKey;
		inKey.push_back(seq[i]);
		vector<string> outStringVectorRef;
        if (cinTable->getCharVectorByKey(inKey, outStringVectorRef) > 0)
			s->append(outStringVectorRef[0]);
    }
    return s;
}

OVIMXcin::OVIMXcin(char *lpath, char *cfile, char *en, char *cn)
{
    char cinfilename[PATH_MAX];
    strcpy(cinfile, cfile);
    strcpy (cinfilename, lpath);
    if (cinfilename[strlen(cinfilename)-1]!='/') strcat(cinfilename, "/");
    strcat(cinfilename, cinfile);
    cintab=new OVCIN(cinfilename);

    sprintf(ename, "%s", en ? en : cfile);
    sprintf(cname, "%s", cn ? cn : cfile);
    sprintf(idbuf, "OVIMGeneric-%s", en ? en : cfile);
}

OVIMXcin::~OVIMXcin()
{
    if (cintab) delete cintab;
}

const char* OVIMXcin::identifier()
{
    return idbuf;
}

const char* OVIMXcin::localizedName(const char* locale)
{
    if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN"))
    {
        return cname;
    }
        
    return ename;
}

int OVIMXcin::initialize(OVDictionary* global, OVService* srv, const char*)
{
    if (!cintab) return 0;
    murmur("OVIMXcin: initializing %s", identifier());
    update(global, srv);
    return 1;
}

void OVIMXcin::update(OVDictionary* global, OVService*)
{
    const char *warningBeep="warningBeep";
    const char *autoCompose="autoCompose";
    const char *maxSeqLen="maxKeySequenceLength";
    const char *hitMax="hitMaxAndCompose";
	const char *sk="shiftSelectionKey";

    if (!global->keyExist(warningBeep)) global->setInteger(warningBeep, 1);
    if (!global->keyExist(maxSeqLen)) global->setInteger(maxSeqLen, 5);
    if (!global->keyExist(autoCompose)) global->setInteger(autoCompose, 0);
    if (!global->keyExist(hitMax)) global->setInteger(hitMax, 0);
	if (!global->keyExist(sk)) global->setInteger(sk, 0);

    cfgMaxSeqLen=global->getInteger(maxSeqLen);
    cfgBeep=global->getInteger(warningBeep);
    cfgAutoCompose=global->getInteger(autoCompose);
    cfgHitMaxAndCompose=global->getInteger(hitMax);
	
	if(global->getInteger(sk) == 0)
		doShiftSelKey = false;
	else
		doShiftSelKey = true;
}

OVInputMethodContext *OVIMXcin::newContext()
{
    return new OVXcinContext(this, cintab);
}

void OVXcinContext::updateDisplay(OVBuffer *buf)
{
    buf->clear();
    if (keyseq.length())
    {
        string *ms= new string;
        keyseq.compose(ms);
        buf->append(ms->c_str());
        delete ms;
    }
    buf->update();
}


int OVXcinContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, 
    OVService *srv)
{
    if (candi.onDuty())
    {
        if (!autocomposing) return candidateEvent(key, buf, textbar, srv);

		if (key->code() == ovkDown ||
            key->code() == ovkLeft ||
            key->code() == ovkUp ||
            key->code() == ovkRight ||
			(!candi.onePage() && key->code()==ovkSpace))
			return candidateEvent(key, buf, textbar, srv);
        
		string output;
        if (candi.select(key->code(), output))
        {
            buf->clear()->append(output.c_str())->send();
            keyseq.clear();
            cancelAutoCompose(textbar);
            return 1;
        }
    }

    if (!keyseq.length() && !isprint(key->code())) return 0;

    if (key->code()==ovkEsc)
    {
        cancelAutoCompose(textbar);
        buf->clear()->update();
        keyseq.clear();
        return 1;
    }

    if (key->code() == ovkDelete || key->code() == ovkBackspace)
    {
        keyseq.remove();
        updateDisplay(buf);
        if (!keyseq.length() && autocomposing) cancelAutoCompose(textbar);
        
        // if autocomposing is on
        if (keyseq.length() && parent->isAutoCompose())
        {
			if (cintab->getWordVectorByChar(keyseq.getSeq(),
			candidateStringVector))
            {
                autocomposing=1;
                compose(buf, textbar, srv);
            }
            else if (candi.onDuty()) cancelAutoCompose(textbar);
        }

        return 1;
    }

    if (keyseq.length() && key->code()==ovkSpace)
    {
        // there the candiate window has only one page, space key
        // will send out the first candidate
        if (autocomposing && candi.onDuty())
        {
            keyseq.clear();
            autocomposing=0;
            cancelAutoCompose(textbar);
            return candidateEvent(key, buf, textbar, srv);
        }

        autocomposing=0;        
        return compose(buf, textbar, srv);
    }
    
    // we send back any CTRL/OPT/CMD key combination 
    // <comment author='lukhnos'>In OV 0.7 this part will be processed by
    // pre-IM key filters</comment>
    if (key->isOpt() || key->isCommand() || key->isCtrl())
    {
        cancelAutoCompose(textbar);
        buf->clear()->update();
        keyseq.clear();
        return 0;
    }
    
    
    // shift and capslock processing
	// <comment author='b6s'>Shift processing is disabled.</comment>
    if (isprint(key->code()) && (key->isCapslock() /*|| key->isShift()*/))
    {
        if (key->isCapslock())
        {
            char cbuf[2];
            if (key->isShift())
                sprintf(cbuf, "%c", toupper(key->code()));
            else
                sprintf(cbuf, "%c", tolower(key->code()));

            buf->append(cbuf);
        }
        //else if (key->isShift()) buf->appendChar(key->lower());
        cancelAutoCompose(textbar);
        keyseq.clear();
        buf->send();
        return 1;
    }

    if (isprint(key->code()) && keyseq.valid(static_cast<char>(key->code())) &&
		/*!key->isShift() &&*/ !key->isCapslock())
    {
        if (keyseq.length() == parent->maxSeqLen())
        {
            updateDisplay(buf);
            if (parent->isBeep()) srv->beep();
            return 1;
        }
        
        keyseq.add(key->code());
        if (keyseq.length() == parent->maxSeqLen() &&
        parent->isHitMaxAndCompose())
        {
            autocomposing=0;
            cancelAutoCompose(textbar);
            return compose(buf, textbar, srv);
        }
		
        updateDisplay(buf);
        if (cintab->isEndKey(static_cast<char>(key->code())))
        {
            autocomposing=0;
            cancelAutoCompose(textbar);
            return compose(buf, textbar, srv);
        }
        
        // if autocomposing is on
        if (parent->isAutoCompose())
        {
            if (cintab->getWordVectorByChar(keyseq.getSeq(),
            candidateStringVector))
            {
                autocomposing=1;
                compose(buf, textbar, srv);
            }
            else if (candi.onDuty()) cancelAutoCompose(textbar);
        }
		        
        return 1;
    }
	// <comment author='b6s'>For undefined key events, do nothing.
	else {
		/*
		cancelAutoCompose(textbar);
		keyseq.clear();
		if (buf->length()) buf->send();
		*/
		
		return 0;
	}
	// </comment>	
}

void OVXcinContext::cancelAutoCompose(OVCandidate *textbar)
{
    autocomposing=0;
    candi.cancel();
    textbar->hide()->clear();
}
    
        
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
