// OVIMRoman.cpp: Input Method for ASCII Characters
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

#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
	#include <OpenVanilla/OVLibrary.h>
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OpenVanilla.h"
	#include "OVLibrary.h"
	#include "OVUtility.h"
	#define strcasecmp stricmp
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>

class OVIMRomanContext : public OVInputMethodContext
{
public:
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv) {
        if (!isprint(k->code()) || k->isFunctionKey()) return 0;
        
        // we only send printable characters so that they can pass thru 
        // full-width character filter (if enabled)
        char s[2];
        sprintf(s, "%c", k->code());
        b->append(s)->send();
        return 1;
    }
};

class OVIMRoman : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMRoman"; }
    virtual OVInputMethodContext *newContext() { return new OVIMRomanContext; }
    virtual int initialize(OVDictionary *, OVService*, const char *mp) {
        return 1;
    }
    virtual const char* localizedName(const char *locale) {
        if (!strcasecmp(locale, "zh_TW")) return "英數";
        if (!strcasecmp(locale, "zh_CN")) return "英数";
        return "Roman (alphanumeric)";
    }
};

OV_SINGLE_MODULE_WRAPPER(OVIMRoman);