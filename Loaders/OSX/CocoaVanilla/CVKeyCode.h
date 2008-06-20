// CVKeyCode.h: CocoaVanilla implementation of OVKeyCode
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

#ifndef __CVKeyCode_h
#define __CVKeyCode_h

#include <Carbon/Carbon.h>
#include <OpenVanilla/OpenVanilla.h>

// the CVSmartMenu extension for CVKeyCode works like this, for example:
//     CVKeyCode k("k", "MOCS");
// means CMD+OPT+CTRL+SHIFT+K; m=CMD, o=OPT, c=CTRL, S=SHIFT (case insensitive)

class CVKeyCode : public OVKeyCode
{
public:
    CVKeyCode(char charcode, UInt32 modifiers);
    virtual int code()       { return c; }
    virtual bool isShift();
    virtual bool isCtrl();
    virtual bool isAlt();
    virtual bool isNum()      { return 0; }  // always so in OS X
    virtual bool isOpt()      { return isAlt(); }
    virtual bool isCommand();
    virtual bool isCapslock();
	
	// extensions for CVSmartMenu
	CVKeyCode(const char *charcode, const char *modifiers);
	CVKeyCode(NSString *s);
	virtual BOOL equalToKey(CVKeyCode* k, BOOL ignorecase=YES);
	virtual UInt8 convertToMenuModifier();
    
    // facility functions for OVPreference
    virtual NSArray *getKeyList();
    virtual NSString *getKeyCodeString();
    virtual NSString *getModifierString();
    virtual NSString *getModifierIconString();
protected:
	void init(const char *charcode, const char *modifiers);
    char c;
    UInt32 m;
};

#endif
