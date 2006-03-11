// OVIMPOJHolo.cpp: prototype for POJ IM
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
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

#include "OVIMPOJ-Holo.h"
#include <ctype.h>

#ifdef WIN32
	#include <windows.h>
	#define PATH_MAX MAX_PATH
	#define strcasecmp stricmp
#else
	#include <sys/syslimits.h>	//for PATH_MAX
#endif

OV_SINGLE_MODULE_WRAPPER(OVIMPOJHolo);

OVIMPOJHolo::OVIMPOJHolo() {
    fullPOJOutput=asciioutput=keylayout=0;
    cintab=NULL;
}

const char* OVIMPOJHolo::identifier() {
    return "OVIMPOJ-Holo";
}

const char* OVIMPOJHolo::localizedName(const char *cfge) {
    if (!strcasecmp(cfge, "zh_TW")) return "Holo/台灣閩南語白話字（POJ）漢羅";
    if (!strcasecmp(cfge, "zh_CN")) return "Holo/闽南语白话字（POJ）汉罗";    
    return "Holo Pe̍h-ōe-jī (POJ)";
}

int OVIMPOJHolo::initialize(OVDictionary* cfg, OVService* s, const char* lp) {    
    // update configuration
    update(cfg, s); 
    
    const char *pathsep=s->pathSeparator();
    char cinpath[PATH_MAX];
    sprintf(cinpath, "%s%sOVIMPOJ-Holo%spoj-holo.cin", lp, pathsep, pathsep);
    murmur ("OVIMPOJHolo: loading poj-holo.cin at %s", cinpath);
    cintab=new OVCIN(cinpath);        

    return 1;
}

void OVIMPOJHolo::update(OVDictionary* cfg, OVService*) {
    asciioutput=cfg->getIntegerWithDefault("ASCIIOutput", 0);
    keylayout=cfg->getIntegerWithDefault("keyboardLayout", 0);
    fullPOJOutput=cfg->getIntegerWithDefault("fullPOJOutput", 0);
    useDotComposing=cfg->getIntegerWithDefault("useDotComposing", 1);
}

OVInputMethodContext *OVIMPOJHolo::newContext() {
    return new OVIMPOJHoloContext(this, cintab);
}

OVIMPOJHoloContext::OVIMPOJHoloContext(OVIMPOJHolo *p, OVCIN *ct) {
    parent=p;
    cintab=ct;
}

OVIMPOJHoloContext::~OVIMPOJHoloContext() {
}
    
void OVIMPOJHoloContext::clear() {
    candi.cancel();
    seq.clear();
}

int OVIMPOJHoloContext::keyEvent(OVKeyCode *key, OVBuffer *buf, 
    OVCandidate *textbar, OVService *srv)
{
    if (candi.onDuty()) return candidateEvent(key, buf, textbar, srv);
 
    int ascii=parent->isAsciiOutput();
    int c=key->code();

    //Ignore arrow keys.
    if (c==ovkDown || c==ovkUp || c==ovkLeft || c==ovkRight) {
    	if(!seq.isEmpty()) return 1; else return 0;
    }
    
    // if backspace of delete key is pressed
    if ((c==ovkBackspace || c==ovkDelete) && !buf->isEmpty()) { 
        seq.remove();
        buf->clear()->append(seq.compose(false, parent->isUseDotComposing()))->update();
        return 1;            
    }

    // if enter or compose key is hit
    if ((c==ovkReturn || seq.isComposeKey(c)) && !buf->isEmpty()) {
        if (key->code()!=ovkReturn) seq.add(c);
        seq.finalize();     // normalize & finalize the key
        buf->clear()->append(seq.compose(false, parent->isUseDotComposing()))->update();
    
        queryAndCompose(seq.sequence(), seq.compose(ascii, parent->isUseDotComposing()), buf, textbar, srv);
        seq.clear();        
        return 1;
    }

    // key=[a-z][A-Z], so add to our key sequence
    if (isalpha(c) || (parent->getKeyLayout() && seq.toneMark(c))) {
        seq.add(c, parent->getKeyLayout());
        buf->clear()->append(seq.compose(false, parent->isUseDotComposing()))->update();
        return 1;
    }

    // we find it's not a "valid" character, so we send up the buffer,
    // then tell the app the process the character itself    
    if (!buf->isEmpty()) {
        seq.normalize();
        buf->clear()->append(seq.compose(ascii, parent->isUseDotComposing()))->send();
    }

    seq.clear();        
    return 0;
}

void OVIMPOJHoloContext::queryAndCompose(const char *qstr, const char *disp,
    OVBuffer *buf, OVCandidate *textbar, OVService *srv)
{
    if (parent->isFullPOJ()) {
        buf->send();
        return;
    }
    // we have to "lowercase" every character in the query string
    char p[256], *q=p;
    strcpy (p, qstr);
    while (*q) {
        *q=tolower(*q);
        q++;
    }

    vector<string> l;
    int result=cintab->getWordVectorByChar(p, l);

    if (!result) {
        buf->send();
    }
    else {
        list.clear();
        list.push_back(disp);
        for (size_t i=0; i<l.size(); i++) list.push_back(l[i]);
        candi.prepare(&list, (char*)(cintab->getSelKey()).c_str(), textbar);
    }
}

int OVIMPOJHoloContext::candidateEvent(OVKeyCode *key, OVBuffer *buf, 
    OVCandidate *textbar, OVService *srv)
{
    int c=key->code();
    if (c==ovkEsc ||c==ovkBackspace) {
        textbar->hide()->clear();
        candi.cancel();
        buf->clear()->update();
        return 1;
    }

    if ((c==ovkDown || c==ovkLeft || c=='>') || 
        (!candi.onePage() && c==ovkSpace)) {
        candi.pageDown()->update(textbar);
        return 1;
    }

    if (c==ovkUp || c==ovkRight || c=='<') {
        candi.pageUp()->update(textbar);
        return 1;
    }

    // enter == first candidate
    // space (when candidate list has only one page) == first candidate

    if (c==ovkReturn || 
        (candi.onePage() && key->code()==ovkSpace)
        || key->code()=='-') c=cintab->getSelKey()[0];
            
    string output;
    
    if (candi.select((char)c, output)) {
        buf->clear()->append(output.c_str());
        if (candi.onePage() && key->code()==ovkSpace) buf->append(" ");
        else if (key->code()=='-') buf->append("-");
        buf->send();
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }
    
    srv->beep();
    return 1;
}

