// OVIMRomanNew.h: New Roman Input Method, supports hunspell.
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

#ifndef OVIMRomanNew_h
#define OVIMRomanNew_h

#ifdef WIN32
	#include "hunspelldll.h"
#else
	#include "hunspell.hxx"
	#include "hunspell.h"
	#define Hunspell Hunhandle
	#define hunspell_initialize (void*)Hunspell_create
	#define hunspell_uninitialize Hunspell_destroy
	#define hunspell_spell Hunspell_spell
	#define hunspell_suggest Hunspell_suggest
#endif

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
const int ovMaxCandiLen = 32;
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

class OVIMRomanNew;

class OVIMRomanNewContext : public OVInputMethodContext
{
public:
	OVIMRomanNewContext(OVIMRomanNew* ovIM): parent(ovIM) {}

    virtual void start(OVBuffer*, OVCandidate*, OVService* s);

    virtual void clear() { keyseq.clear();} 

	virtual void end() { hunspell_uninitialize((Hunspell*) hunspellChecker); }

    virtual int keyEvent(
        OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* s);

protected:
    int closeCandidateWindow(OVCandidate* c) {
        if (c->onScreen()) c->hide()->clear()->update();
        candi.clear();
        return 1;        
    }

    int showcandi(OVCandidate* i);
	size_t spellCheckerByHunspell(char* buf);
    bool isEnglish(char* buf);

protected:
	KeySeq keyseq;
    IMGCandidate candi;
    size_t pagenumber;
    size_t pagetotal;
    size_t temp;

	void* hunspellChecker;

	OVIMRomanNew* parent;
};

class OVIMRomanNew : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMRomanNew"; }
    virtual OVInputMethodContext *newContext()
	{
		return new OVIMRomanNewContext(this);
	}

    virtual int initialize(OVDictionary* l, OVService* s, const char* libPath)
    {        
		modulePath =
			string(libPath) + string(identifier()) + string(s->pathSeparator());

        return 1; 
    }

    virtual const char* localizedName(const char *locale) {
		if (!strcasecmp(locale, "zh_TW")) return "ZH_TW";
        if (!strcasecmp(locale, "zh_CN")) return "ZH_CN";
        return "New Roman (alphanumeric)";
    }

	string modulePath;
};

extern "C" unsigned int OVGetLibraryVersion() { return OV_VERSION; }
extern "C" int OVInitializeLibrary(OVService* s, const char* libPath) {
	//string dataPath = libPath + s->pathSeparator() + "OVIMRomanNew";

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

#endif
