// OVCandidateList.h: A simple candidate list class
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

#ifndef __OVCandidateList_h
#define __OVCandidateList_h

#ifndef WIN32
    #include <OpenVanilla/OpenVanilla.h>  
    #include <OpenVanilla/OVUtility.h>
#else
    #include "OpenVanilla.h"
    #include "OVUtility.h"
#endif

#include <string>
#include <vector>

using namespace std;

class OVCandidateList : public OVBase
{
public:
    OVCandidateList()
    {
        onduty=0;
    }
    
    void prepare(vector<string>* l, char* skey, OVCandidate *textbar);
    int onDuty() { return onduty; }
    int onePage() { return (count <= perpage); }
    void cancel() { onduty=0; }
    void update(OVCandidate *textbar);
    OVCandidateList* pageUp();
    OVCandidateList* pageDown();
    bool select(char inKey, string& outStringRef);
	const char* getSelKey() { return selkey; }
protected:
    int onduty;
    char selkey[32];
    int count;
    int perpage;
    int pos;
    vector<string>* list;
};

#endif
