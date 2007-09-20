// OVIMGeneric.h: Generic Input Method, reads .cin
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

#ifndef __OVIMGeneric_h
#define __OVIMGeneric_h

#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
	#include <OpenVanilla/OVLibrary.h>
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OpenVanilla.h"
	#include "OVLibrary.h"
	#include "OVUtility.h"
/*
    #include <string.h>
	#define strcasecmp stricmp
*/
#endif

#include "OVOSDef.h"

#include "OVCIN.h"
#include "OVKeySequence.h"
#include "OVCandidateList.h"
#include "OVCINInfo.h"
#include <stdlib.h>
#include <string>
#include <vector>


using namespace std;

class GenericKeySequence : public OVKeySequenceSimple
{
public:
    GenericKeySequence(OVCIN* cintab);
    virtual int length() { return len; }
    virtual bool add(char c);
    virtual bool valid(char c);
    virtual string* compose(string* s);
    virtual char* getSeq() { return seq; }
    
protected:
    OVCIN* cinTable;
};

class OVIMGeneric;

class OVGenericContext : public OVInputMethodContext
{
public:
    OVGenericContext(OVIMGeneric* p, OVCIN* tab) : 
        parent(p), keyseq(tab), cintab(tab), autocomposing(false) {}
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);
    virtual void clear();

protected:
    virtual void updateDisplay(OVBuffer *buf);
    virtual int compose(OVBuffer *buf, OVCandidate *textbar, OVService *srv);
    virtual int candidateEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);
    virtual void cancelAutoCompose(OVCandidate *textbar);
    
    OVIMGeneric* parent;
    GenericKeySequence keyseq;
    OVCandidateList candi;
    OVCIN* cintab;
    
    bool autocomposing;
    vector<string> candidateStringVector;
};

class OVIMGeneric : public OVInputMethod
{
public:
    OVIMGeneric(const OVCINInfo& ci);
    virtual ~OVIMGeneric();
    virtual const char* identifier();
    virtual const char* localizedName(const char* locale);
    virtual int initialize(OVDictionary*, OVService*, const char*);
    virtual void update(OVDictionary*, OVService*);
    virtual OVInputMethodContext *newContext();
    
    virtual int maxSeqLen() { return cfgMaxSeqLen; }
    virtual int isBeep() { return cfgBeep; }
    virtual int isAutoCompose() { return cfgAutoCompose; }
    virtual int isHitMaxAndCompose() { return cfgHitMaxAndCompose; }
    virtual bool isShiftSelKey() { return doShiftSelKey; };
    
	virtual char matchOneChar() { return cfgMatchOneChar; }
	virtual char matchZeroOrMoreChar() { return cfgMatchZeroOrMoreChar; }

protected:
    OVCINInfo cininfo;
    OVCIN* cintab;

    string idstr;    
    
    int cfgMaxSeqLen;
    int cfgBeep;
    int cfgAutoCompose;
    int cfgHitMaxAndCompose;

	char cfgMatchOneChar;
	char cfgMatchZeroOrMoreChar;
    
    bool doShiftSelKey;
};

#endif
