// OVIMGeneric.cpp: Generic Input Method, reads .cin
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

// #define OV_DEBUG

#include "OVIMGeneric.h"
#include "CIN-Defaults.h"

using namespace std;

OVCINList *cinlist=NULL;

extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService *s, const char *libpath) {
    if (cinlist) {
        // already initializde
        return false;
    }

    const char *pathsep=s->pathSeparator();
        
    cinlist = new OVCINList(pathsep);
    if (!cinlist) return false;

    // will be something like this on OS X:
    //     ~/Library/OpenVanilla/version/UserSpace/OVIMGeneric/
    string userpath=s->userSpacePath("OVIMGeneric");

    // will be something like this on OS X:
    //     /Library/OpenVanilla/version/Modules/OVIMGeneric/
    string datapath=string(libpath) + string(pathsep) + string("OVIMGeneric");
        
    murmur("OVIMGeneric initializing");
   
    Watch watch;
    int loaded=0;

    watch.start(); 
    loaded += cinlist->load(userpath.c_str(), ".cin");
    watch.stop();

    murmur("Loaded modules from %s in %1.3f",
        userpath.c_str(), watch.getSec());

    watch.start(); 
    loaded += cinlist->load(datapath.c_str(), ".cin");
    watch.stop();

    murmur("Loaded modules from %s in %1.3f",
        datapath.c_str(), watch.getSec());

    if (!loaded) {
        murmur ("OVIMGeneric: nothing loaded, init failed");
        return false;
    }

    return true;
}
extern "C" OVModule *OVGetModuleFromLibrary(int x) {
    if ((size_t)x >= cinlist->count()) return NULL;
       
    return new OVIMGeneric(cinlist->cinInfo((size_t)x));
}

GenericKeySequence::GenericKeySequence(OVCIN* cintab) {
    cinTable=cintab;
}
    
bool GenericKeySequence::valid(char c) {
	string inKey;
	inKey.push_back(c);
    if (!cinTable->isValidKey(inKey)) return false;
    return true;
}
    
bool GenericKeySequence::add(char c) {
    if (valid(c) == 0) return false;
    return OVKeySequenceSimple::add(c);
}
    
string *GenericKeySequence::compose(string *s)
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

OVIMGeneric::OVIMGeneric(const OVCINInfo& ci) : cininfo(ci), cintab(NULL) {
    idstr = "OVIMGeneric-" + cininfo.shortfilename;
}

OVIMGeneric::~OVIMGeneric() {
    if (cintab) delete cintab;
}

const char* OVIMGeneric::identifier()
{
    return idstr.c_str();
}

const char* OVIMGeneric::localizedName(const char* locale)
{
    if (!strcasecmp(locale, "zh_TW")) return cininfo.tcname.c_str();
    if (!strcasecmp(locale, "zh_CN")) return cininfo.scname.c_str();
    return cininfo.ename.c_str();
}

int OVIMGeneric::initialize(OVDictionary* global, OVService* srv, const char*)
{
    Watch watch;
    watch.start();
    if (!cintab) {
        cintab=new OVCIN(cininfo.longfilename.c_str());
     }
    watch.stop();

    murmur("OVIMGeneric: initializing %s in %1.3f sec",
        identifier(), watch.getSec());
    update(global, srv);
    return 1;
}

void OVIMGeneric::update(OVDictionary* global, OVService*)
{
    CINSetDefaults(cininfo.shortfilename.c_str(), global);
    
    cfgMaxSeqLen=global->getInteger(CIN_MAXSEQLEN);
    cfgBeep=global->getInteger(CIN_WARNINGBEEP);
    cfgAutoCompose=global->getInteger(CIN_AUTOCOMPOSE);
    cfgHitMaxAndCompose=global->getInteger(CIN_HITMAX);
	
	if(!global->getInteger(CIN_SHIFTSELECTIONKEY))
	   doShiftSelKey = false;
	else
		doShiftSelKey = true;
		
	const char *cfgstr;
	cfgstr = global->getStringWithDefault(CIN_MATCHONECHAR, "");
	cfgMatchOneChar = cfgstr[0];
	
	cfgstr = global->getStringWithDefault(CIN_MATCHZEROORMORECHAR, "");
	cfgMatchZeroOrMoreChar = cfgstr[0];
}

OVInputMethodContext *OVIMGeneric::newContext()
{
    return new OVGenericContext(this, cintab);
}

void OVGenericContext::updateDisplay(OVBuffer *buf)
{
    buf->clear();
    // murmur("UpdateDisplay");
    if (keyseq.length())
    {
        string *ms= new string;
        keyseq.compose(ms);
        buf->append(ms->c_str());
        delete ms;
    }
    buf->update();
}

void OVGenericContext::clear() {
    keyseq.clear();
    autocomposing=false;
    candi.cancel();
}

int OVGenericContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, 
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
			if (cintab->getWordVectorByCharWithWildcardSupport(keyseq.getSeq(), candidateStringVector, parent->matchOneChar(), parent->matchZeroOrMoreChar()))
            {
                autocomposing=true;
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
            autocomposing=false;
            cancelAutoCompose(textbar);
            return candidateEvent(key, buf, textbar, srv);
        }

        autocomposing=false;        
        return compose(buf, textbar, srv);
    }
    
    // we send back any CTRL/OPT/CMD key combination 
    // <comment author='lukhnos'>In OV 0.7 this part will be processed by
    // pre-IM key filters</comment>
    if (key->isOpt() || key->isCommand() || key->isCtrl())
    {
        if (!buf->isEmpty()) {
            cancelAutoCompose(textbar);
            buf->clear()->update();
            keyseq.clear();
        }
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
    	if (keyseq.length() > 0 &&
    		// prevent to the exception of parent->maxSeqLen() == 0
    		keyseq.length() == parent->maxSeqLen())
    	{
            updateDisplay(buf);
			srv->notify("\xE5\x9B\xA7");
            if (parent->isBeep())
				srv->beep();
            return 1;
    	}
    	
        keyseq.add(key->code());
		// murmur("add %d", key->code());
        if (keyseq.length() == parent->maxSeqLen() &&
        parent->isHitMaxAndCompose())
        {
            autocomposing=false;
            cancelAutoCompose(textbar);
            return compose(buf, textbar, srv);
        }
		
        updateDisplay(buf);
        if (cintab->isEndKey(static_cast<char>(key->code())))
        {
            autocomposing=false;
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
    
    if (!buf->isEmpty()) {
		srv->notify("\xE5\x9B\xA7");
        if (parent->isBeep())
			srv->beep();
        return 1;
    }
    
    if (isprint(key->code())) {
        char sb[2];
        sprintf(sb, "%c", key->code());
        buf->append(sb)->send();
        return 1;
    }
    
    return 0;
}

void OVGenericContext::cancelAutoCompose(OVCandidate *textbar)
{
    autocomposing=false;
    candi.cancel();
    textbar->hide()->clear();
}
    
        
int OVGenericContext::compose(OVBuffer *buf, OVCandidate *textbar, OVService *srv)
{
    if (!keyseq.length()) return 0;

	size_t size =
		cintab->getWordVectorByCharWithWildcardSupport(keyseq.getSeq(), candidateStringVector, parent->matchOneChar(), parent->matchZeroOrMoreChar());

    if (size == 0)
    {
		srv->notify("\xE5\x9B\xA7");
        if (parent->isBeep())
			srv->beep();
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

int OVGenericContext::candidateEvent(OVKeyCode *key, OVBuffer *buf, 
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

	srv->notify("\xE5\x9B\xA7");
    if (parent->isBeep())
		srv->beep();

    return 1;
}
