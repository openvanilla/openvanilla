// OVKPPhraseTools.h: Phrase management tool
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

#ifndef __OVKPPhraseTool_h
#define __OVKPPhraseTool_h

// #define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <string>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include "OVSQLite3.h"
#include "PTKeySequence.h"
#include "PTCandidateList.h"

using namespace std;

#define PT_DEFKEY       '~'
#define PT_CATCHKEY     '['
#define PT_DBNAME       "userphrase-0.7.2.0.db"
#define PT_DBTBLCREATE  "create table phrase(key, value, context, time, freq);"
#define PT_DBIDXCREATE  "create index phrase_index_key on phrase(key);"

enum {
    PTS_WAIT=0,
    PTS_MODE_ACTIVATED=1,
    PTS_MODE_COMMAND=2,
    PTS_MODE_CANDIDATE=3
};

class OVKPPhraseTools;

class OVKPPhraseToolsContext : public OVInputMethodContext {
public:
    OVKPPhraseToolsContext(OVKPPhraseTools *p);
    virtual void start(OVBuffer*, OVCandidate*, OVService*);
    virtual void clear();
    virtual void end();
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);

protected:
    virtual bool clearBufCandi();   // clear buffer and candidate, always true
    virtual bool composeAndUpdateBuffer();  // always returns true
    virtual bool state_activated();
    virtual bool state_command();
    virtual bool state_candidate();
    virtual bool parse_command();
    
    virtual bool command_add(const string& k);
    virtual bool command_search(const string& k, bool wcard=false);
    
    // displays end message (mode ended)
    virtual bool cancelmsg(bool returnvalue);
    virtual bool cancelmsg(int newstate, bool returnvalue);

    OVKPPhraseTools *parent;
    
    int state;
    PTKeySequence keyseq;
    
    bool wildcard;
    vector<string>candilist;
    vector<string>valuelist;
    PTCandidateList candictrl;
    
    OVBuffer *b;
    OVKeyCode *k;
    OVCandidate *c;
    OVService *s;
};

class OVKPPhraseTools : public OVInputMethod {
public:
    OVKPPhraseTools(bool &cflag, PTKeySequence &wseq, SQLite3 *pdb);
    
    virtual const char *moduleType() { return "OVKeyPreprocessor"; }
    virtual const char *identifier() { return "OVKPPhraseTools"; }    
    virtual const char *localizedName(const char*);
    virtual int initialize(OVDictionary*, OVService*, const char*);
    virtual void update(OVDictionary*, OVService*);
    virtual OVInputMethodContext* newContext();
    
protected:
    bool &recordingMode;
    PTKeySequence &recSeq;
    SQLite3 *db;
    
    int actkey;     // key to invoke the tools
    int catchkey;   // (after phrase tools on) key to start text recorder
    string selkey;  // selection key (e.g. 1234567890)
    
    friend class OVKPPhraseToolsContext;
};

class OVOFTextRecorder : public OVOutputFilter {
public:
    OVOFTextRecorder(bool &cflag, PTKeySequence& wseq);

    virtual const char *identifier() { return "OVOFTextRecorder"; }
    virtual const char *localizedName(const char*);
    virtual const char *process(const char*, OVService*);
    
protected:
    bool &recordingMode;
    PTKeySequence &recSeq;
};

#endif
