// OVIMEsperanto.cpp: Input Method which applies X-transformation for Esperanto
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

#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
	#include <OpenVanilla/OVLibrary.h>
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OpenVanilla.h"
	#include "OVLibrary.h"
	#include "OVUtility.h"
	#define strcasecmp stricmp
	#define snprintf sprintf_s	
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const size_t ebMaxKeySeq=3;
class KeySeq 
{
	public:
	    KeySeq()
		{
	        reset();
	    }
	    void set(int i)
		{
			x = i;
	    }
	    void reset()
		{
	        x = -1;
	    }
		int value()
		{
			return x;
		}
		int x;	
};

// cx, gx, hx, jx, sx and ux

# define XTRANS 12

char XtransKey[XTRANS] = {'c','g', 'h', 'j', 's', 'u', 'C', 'G', 'H', 'J', 'S', 'U'};
unsigned short XtransChar[XTRANS] = {0x0109,0x011D,0x0125,0x0135,0x015D,0x016D,
									 0x0108,0x011C,0x0124,0x0134,0x015C,0x016C};

short isXtrans(int key)
{
	int i;
	for (i = 0; i < XTRANS; i++) {
		if(key == XtransKey[i])
			return i;
	}
	return -1;
}

class OVIMEsperantoContext : public OVInputMethodContext
{
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*)
	{
        clear();
    }
    virtual void clear()
	{
		keyseq.reset();
    }	
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv)
	{
		if (k->isOpt() || k->isCommand() || k->isCtrl()){return 0;}
		if (k->code() == ovkUp || k->code() == ovkDown || k->code() == ovkLeft || k->code() == ovkRight || k->code()==ovkReturn) {
            if (keyseq.value() > -1) {
                b->send()->clear();
            }
			keyseq.reset();
            return 0;
		}
		if (k->code() == ovkDelete || k->code() == ovkBackspace) {
			if (keyseq.value() > -1) {
				b->clear()->update();
				keyseq.reset();
				return 1;
			}
			return 0;
		}
		
        char s[3];
		int x = isXtrans(k->code());
		snprintf(s, 3, "%c", k->code());
		
		if (keyseq.value() > -1) {
			if (k->code() == 'x' || k->code() == 'X') {
				const char *u8;				
				unsigned short y = XtransChar[keyseq.value()];
				u8 = srv->UTF16ToUTF8(&y, 1);
				b->clear()->append(u8)->send();
			} 
			else {
				b->append(s)->send();
			}
			keyseq.reset();
		}
		else if (x > -1) {
			keyseq.set(x);
			b->append(s)->update();
		}
		else {
			b->append(s)->send();
		}
        return 1;
    }
protected:
	KeySeq keyseq;	
};

class OVIMEsperanto : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMEsperanto"; }
    virtual OVInputMethodContext *newContext() { return new OVIMEsperantoContext; }
    virtual int initialize(OVDictionary *, OVService*, const char *mp)
	{
        return 1;
    }
    virtual const char* localizedName(const char *locale)
	{
		if (!strcasecmp(locale, "ja"))
			return "\xE3\x82\xA8\xE3\x82\xB9\xE3\x83\x9A\xE3\x83\xA9\xE3\x83\xB3\xE3\x83\x88";
        if (!strcasecmp(locale, "zh_TW"))
			return "\xE4\xB8\x96\xE7\x95\x8C\xE8\xAA\x9E";
        if (!strcasecmp(locale, "zh_CN"))
			return "\xE4\xB8\x96\xE7\x95\x8C\xE8\xAF\xAD";
        return "Esperanto";
    }
};

OV_SINGLE_MODULE_WRAPPER(OVIMEsperanto);
