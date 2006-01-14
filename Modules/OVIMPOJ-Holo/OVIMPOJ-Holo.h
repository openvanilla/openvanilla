// OVIMPOJHolo-Holo.h: POJ-Holo input method
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

#ifndef __OVIMPOJHoloHolo_h
#define __OVIMPOJHoloHolo_h

#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
	#include <OpenVanilla/OVUtility.h>
	#include <OpenVanilla/OVLibrary.h>
#else
	#include "OpenVanilla.h"
	#include "OVUtility.h"
	#include "OVLibrary.h"
#endif

#include "OVCIN.h"
#include "OVCandidateList.h"
#include "POJ-Holo.h"

class OVIMPOJHolo;

class OVIMPOJHoloContext : public OVInputMethodContext {
public:
    OVIMPOJHoloContext(OVIMPOJHolo *p, OVCIN *ct);
    ~OVIMPOJHoloContext();
    
    virtual void clear();
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar,
        OVService *srv);
    
protected:
    virtual int candidateEvent(OVKeyCode *key, OVBuffer *buf,
        OVCandidate*textbar, OVService *srv);
    virtual void queryAndCompose(const char *qstr, const char *disp, 
        OVBuffer *buf, OVCandidate *textbar, OVService *srv);

    OVIMPOJHolo *parent;
    POJHoloKeySequence seq;
    OVCandidateList candi;
    OVCIN *cintab;
    vector<string> list;
};


class OVIMPOJHolo : public OVInputMethod {
public:
    OVIMPOJHolo();
    virtual const char* identifier();
    virtual const char* localizedName(const char *cfge);
    virtual int initialize(OVDictionary*, OVService*, const char*);
    virtual void update(OVDictionary*, OVService*);  
    virtual OVInputMethodContext *newContext();

    virtual int isAsciiOutput() { return asciioutput; }
    virtual int getKeyLayout() { return keylayout; }
    virtual int isFullPOJ() { return fullPOJOutput; }
    virtual int isUseDotComposing () { return useDotComposing; }

    
protected:
    int fullPOJOutput;
    int asciioutput;
    int keylayout;
    int useDotComposing;
    OVCIN *cintab;
};

#endif