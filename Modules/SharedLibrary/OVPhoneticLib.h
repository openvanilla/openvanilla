// OVPhoneticLib.h: BPMF syllable toolkit
//
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

#ifndef __OVPhoneticLib_h
#define __OVPhoneticLib_h

enum {
    OVPStandardLayout=0,
    OVPEtenLayout=1
};

class OVPCandidate;

class OVPhoneticSyllable
{
public:
    OVPhoneticSyllable(int layout=OVPStandardLayout);
    
    int layout();
    void setLayout(int layout);
    
    int empty();
    int isComposeKey(char c);
    int addKey(char c);
    int isValidKey(char c);
    int removeLast();
    void clear();
    
    const char *compose();
    const char *standardLayoutCode();
    
protected:
    unsigned short syllable;
    int keylayout;
    friend OVPCandidate* OVPFindCandidate(unsigned short *data, OVPhoneticSyllable *s);
};

struct OVPCandidate
{
    OVPCandidate();
    ~OVPCandidate();
    int count;
    char **candidates;
};

// you're responsible to delete the object returned
OVPCandidate *OVPFindCandidate(unsigned short *data, OVPhoneticSyllable *syl);

enum {
    OVP_PUNCTUATION_MASK = 0xff00,
    OVP_CTRL_OPT_MASK = 0xfe00,
    OVP_PUNCTUATION_LIST = 0xff80
};
OVPCandidate *OVPFindCandidateWithCode(unsigned short *data, unsigned short k);

#endif

