// OVIMXcin.cpp

// #define OVDEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLoadable.h>
#include <OpenVanilla/OVUtility.h>
#include <stdlib.h>
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
        cinlist.list[x].ename, cinlist.list[x].cname, cinlist.list[x].encoding);
}

extern "C" void OVLoadableDeleteIM(OVInputMethod *im)
{
    delete im;
}

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

OVIMXcin::OVIMXcin(char *lpath, char *cfile, char *en, char *cn,
    OVEncoding enc)
{
    strcpy(loadpath, lpath);
    strcpy(cinfile, cfile);
    cintab=NULL;
   
    cnameencoding=enc;
    sprintf(ename, "OV xcin %s", en ? en : cfile);
    sprintf(cname, "OV xcin %s", cn ? cn : cfile);
}

OVIMXcin::~OVIMXcin()
{
    if (cintab) delete cintab;
}

int OVIMXcin::identifier(char *s)
{
    char idbuf[256];
    strcpy(idbuf, "OVIMXcin-");
    strcat(idbuf, cinfile);
    return strlen(strcpy(s, idbuf));
}

int OVIMXcin::name(char *locale, void *s, OVEncoding *enc)
{
    *enc=ovEncodingUTF8;
    if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN"))
    {
        *enc=cnameencoding;
        return strlen(strcpy((char*)s, cname));
    }
        
    return strlen(strcpy((char*)s, ename));
}

int OVIMXcin::initialize(OVDictionary* global, OVDictionary* local, OVService*, char*)
{
    if (cintab) return 0;
    
    //const char *sk="shiftSelectionKey";
	//<comment authur='b6s'>move to OVIMXcin::update()</comment>
    const char *encoding="encoding";
    char buf[256];
    OVEncoding enc=ovEncodingUTF8;
    //if (!local->keyExist(sk)) local->setInt(sk, 0);
	//<comment authur='b6s'>move to OVIMXcin::update()</comment>
//  if (!local->keyExist(encoding)) local->setString(encoding, "big5");

    update(global, local);  // run-time configurable settings    
    local->getString(encoding, buf);
    enc=OVEncodingMapper(buf);
	
    char cinfilename[PATH_MAX];
    strcpy (cinfilename, loadpath);
    if (cinfilename[strlen(cinfilename)-1]!='/') strcat(cinfilename, "/");
    strcat(cinfilename, cinfile);
    cintab=new OVCIN(cinfilename);
		
    return 1;
}

int OVIMXcin::update(OVDictionary* global, OVDictionary* local)
{
    const char *warningBeep="warningBeep";
    const char *autoCompose="autoCompose";
    const char *maxSeqLen="maxKeySequenceLength";
    const char *hitMax="hitMaxAndCompose";
	const char *sk="shiftSelectionKey";

    if (!global->keyExist(warningBeep)) global->setInt(warningBeep, 1);
    if (!local->keyExist(maxSeqLen)) local->setInt(maxSeqLen, 5);
    if (!local->keyExist(autoCompose)) local->setInt(autoCompose, 0);
    if (!local->keyExist(hitMax)) local->setInt(hitMax, 0);
	if (!local->keyExist(sk)) local->setInt(sk, 0);

    cfgMaxSeqLen=local->getInt(maxSeqLen);
    cfgBeep=global->getInt(warningBeep);
    cfgAutoCompose=local->getInt(autoCompose);
    cfgHitMaxAndCompose=local->getInt(hitMax);
	
	if(local->getInt(sk) == 0)
		doShiftSelKey = false;
	else
		doShiftSelKey = true;

    return 1;
}

int OVIMXcin::terminate(OVDictionary*, OVDictionary*, OVService*)
{
    delete cintab;
    cintab=NULL;
    return 1;
}

OVIMContext *OVIMXcin::newContext()
{
    return new OVXcinContext(this, cintab);
}

int OVXcinContext::activate(OVService *)
{
    return 1;
    
}

int OVXcinContext::deactivate(OVService *)
{
    candi.cancel();
    keyseq.clear();
    return 1;
}

void OVXcinContext::updateDisplay(OVBuffer *buf)
{
    buf->clear();
    if (keyseq.length())
    {
        string *ms= new string;
        keyseq.compose(ms);
        buf->append((void*)const_cast<char*>(ms->c_str()),
					parent->getCNameEncoding());
        delete ms;
    }
    buf->update();
}


int OVXcinContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, 
    OVService *srv)
{
    if (candi.onDuty())
    {
        if (!autocomposing) return candidateEvent(key, buf, textbar, srv);

		if (key->isCode(4, ovkDown, ovkLeft, ovkUp, ovkRight) ||
			(!candi.onePage() && key->code()==ovkSpace))
			return candidateEvent(key, buf, textbar, srv);
        
		string output;
        if (candi.select(key->code(), output))
        {			
            buf->clear()->
                append((void*)const_cast<char*>(output.c_str()),
					   parent->getCNameEncoding())->send();
            keyseq.clear();
            cancelAutoCompose(textbar);
            return 1;
        }
    }

    if (!keyseq.length() && !key->isPrintable()) return 0;

    if (key->code()==ovkEscape)
    {
        cancelAutoCompose(textbar);
        buf->clear()->update();
        keyseq.clear();
        return 1;
    }

    if (key->isCode(2, ovkDelete, ovkBackspace))
    {
        keyseq.remove();
        updateDisplay(buf);
        if (!keyseq.length() && autocomposing) cancelAutoCompose(textbar);
        
        // if autocomposing is on
        if (keyseq.length() && parent->isAutoCompose())
        {
			string inKey(keyseq.getSeq());
            if (cintab->isValidKey(inKey))
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
    if (key->isPrintable() && (key->isCapslock() /*|| key->isShift()*/))
    {
        if (key->isCapslock())
        {
            if (key->isShift()) buf->appendChar(key->upper());
            else buf->appendChar(key->lower());
        }
        //else if (key->isShift()) buf->appendChar(key->lower());
        cancelAutoCompose(textbar);
        keyseq.clear();
        buf->send();
        return 1;
    }

    if (key->isPrintable() && keyseq.valid(static_cast<char>(key->code())) &&
		/*!key->isShift() &&*/ !key->isCapslock())
    {
        if (keyseq.length() == parent->maxSeqLen())
        {
            updateDisplay(buf);
            if (parent->isBeep()) srv->beep();
            return 1;
        }
        
        keyseq.add(key->code());
        if (keyseq.length() == parent->maxSeqLen() && parent->isHitMaxAndCompose())
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
            if (cintab->getWordVectorByChar(keyseq.getSeq(), candidateStringVector))
            {
                autocomposing=1;
                compose(buf, textbar, srv);
            }
            else if (candi.onDuty()) cancelAutoCompose(textbar);
        }
        
        return 1;
    }

    cancelAutoCompose(textbar);
    keyseq.clear();
    if (buf->length()) buf->send();
    
    return 0;
}

void OVXcinContext::cancelAutoCompose(OVTextBar *textbar)
{
    autocomposing=0;
    candi.cancel();
    textbar->hide()->clear();
}
    
        
int OVXcinContext::compose(OVBuffer *buf, OVTextBar *textbar, OVService *srv)
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
        buf->clear()->append((void*)const_cast<char*>
                             (candidateStringVector[0].c_str()),
							 parent->getCNameEncoding())
                    ->send();
        keyseq.clear();
        return 1;
    }

    if (!autocomposing)
    {    
        buf->clear()->append((void*)const_cast<char*>
                             (candidateStringVector[0].c_str()),
							 parent->getCNameEncoding())
                    ->update();
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
    OVTextBar *textbar, OVService *srv)
{
    if (key->isCode(2, ovkEscape, ovkBackspace))
    {
        textbar->hide()->clear();
        candi.cancel();
        buf->clear()->update();
        return 1;
    }

    if (key->isCode(2, ovkDown, ovkRight) ||
        (!candi.onePage() && key->code()==ovkSpace))
    {
        candi.pageDown()->update(textbar);
        return 1;
    }

    if (key->isCode(2, ovkUp, ovkLeft))
    {
        candi.pageUp()->update(textbar);
        return 1;
    }

    // enter == first candidate
    // space (when candidate list has only one page) == first candidate
    char c=key->code();
    if (key->isCode(2, ovkReturn, ovkMacEnter) || 
        (candi.onePage() && key->code()==ovkSpace)) c=candi.getSelKey()[0];

    string output;
    if (candi.select(c, output)) {
        buf->clear()->append((void*)const_cast<char*>(output.c_str()),
                             parent->getCNameEncoding())
					->send();
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }

	string inKey;
	inKey.push_back(c);
    if (cintab->isValidKey(inKey) || cintab->isEndKey(c)) {
        string output;
        candi.select(candi.getSelKey()[0], output);

		buf->clear()->append((void*)const_cast<char*>(output.c_str()),
							 parent->getCNameEncoding())
					->send();
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