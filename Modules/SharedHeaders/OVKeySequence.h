// OVKeySequence.h: A minimalist key sequence class
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

#ifndef __OVKeySequence_h
#define __OVKeySequence_h

class OVKeySequence : public OVBase
{
public:
    virtual bool add(char) { return false; }
    virtual OVKeySequence* remove() { return this; }
    virtual OVKeySequence* clear() { return this; }
};

const int ovMaxSeqSimpLen=32;

class OVKeySequenceSimple : public OVKeySequence
{
public:
    inline OVKeySequenceSimple(int m=ovMaxSeqSimpLen)
				: len(0), maxLen(m)
			{ seq[0]=0; }
    virtual bool add(char c) {
        if (len==maxLen) return false;
        seq[len++]=tolower(c);
        seq[len]=0;
        return true;
    }
    
    virtual OVKeySequence* remove() {
        if (len) seq[--len]=0;
        return this;
    }
    
    virtual OVKeySequence* clear() {
        len=0;
        seq[0]=0;
        return this;
    }
    
	virtual int length() {
        return len;
	}
	
	virtual const char *content() {
        return seq;
	}
	
protected:
    int len;
    int maxLen;
    char seq[ovMaxSeqSimpLen];
};

#endif