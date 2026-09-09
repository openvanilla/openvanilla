// OVIMArray.h: The Array Input Method
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

#ifndef _OVIMARRAY_H
#define _OVIMARRAY_H

#include "OpenVanilla.h"
#include "LegacyOpenVanilla.h"
#include "OVOSDef.h"

#include "ArrayKeySequence.h"
#include "OVCandidateList.h"
#include <cstring>

namespace OV_Array {
    enum STATE {
        STATE_WAIT_KEY1 = 0,
        STATE_WAIT_KEY2,
        STATE_WAIT_KEY3,
        STATE_WAIT_CANDIDATE
    };

    enum {
        MAIN_TAB = 0,
        SHORT_TAB = 1,
        SPECIAL_TAB = 2,
        PHRASE_TAB = 3
    };
    const int RET_PASS = 0;
    const int RET_DONE = 1;
    const int RET_CONTINUE = 2;
};


class OVIMArray;
class OVIMArrayContext : public OVInputMethodContext
{
private:
    OVIMArray* parent;
    OpenVanilla::OVCINDataTable **tabs;
    ArrayKeySequence keyseq;
    OV_Array::STATE state;
    OVCandidateList candi;
    std::vector<std::string> candidateStringVector, specialCodeVector;
    string selKeys;
public:
    OVIMArrayContext(OVIMArray* p, OpenVanilla::OVCINDataTable** t)
        : parent(p), tabs(t), keyseq(t[OV_Array::MAIN_TAB])
	{
        state = OV_Array::STATE_WAIT_KEY1;
        selKeys = tabs[OV_Array::MAIN_TAB]->findProperty("selkey");
        if (!selKeys.length()) {
            selKeys = "123456789";
        }
    }
    virtual int keyEvent(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    virtual void clear();
    bool isComposing();
private:
    void changeState(OV_Array::STATE s);
    void changeBackState(OV_Array::STATE s);
    virtual void updateDisplay(OVBuffer*);
    void dispatchStateHandler(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitKey1(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitKey2(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitKey3(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitCandidate(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int updateCandidate(OpenVanilla::OVCINDataTable *tab,OVBuffer *buf, OVCandidate *candibar);
    void showSymbolCandidates(OVBuffer* , OVCandidate* );
    void sendAndReset(const char *, OVBuffer* , OVCandidate* , OVService* );
    void clearAll(OVBuffer* buf, OVCandidate* candi_bar);
    void clearCandidate(OVCandidate *candi_bar);
    int selectCandidate(int num, string& out);
    // Symbol table prefixes: "w" is the original menu, "hg" is the second menu
    // added by the v2026 Array table. A digit typed right after either of them
    // picks a symbol group instead of selecting a candidate.
    bool isSymbolPrefix(const char* seq, int len) const
	{
        if (len == 1) return seq[0] == 'w';
        if (len == 2) return seq[0] == 'h' && seq[1] == 'g';
        return false;
    }
    // True when the whole sequence is a complete symbol code, e.g. "w1", "hg1".
    bool isSymbolSeq(const char* seq, int len) const
	{
        return len > 1 && isdigit(seq[len - 1]) && isSymbolPrefix(seq, len - 1);
    }
    bool isForceSPSeq()
	{
        return keyseq.length() == 4 && !memcmp(keyseq.getSeq(),",,sp",4);
    }
    void queryKeyName(const char *keys, std::string& outKeyNames);
    void commitKeySeq(int table, const char* errorMessage, OVBuffer* buf, OVCandidate* candi_bar, OVService* srv);
};

class OVIMArray : public OVInputMethod
{
private:
    char cname[128], ename[128];
    OpenVanilla::OVCINDataTable *tabs[4]; // main, short-code, special-code, phrase
    int cfgAutoSP, cfgForceSP;
    std::string customMainTablePath;

public:
    OVIMArray()
    {
        tabs[0] = tabs[1] = tabs[2] = tabs[3] = 0;
        cfgAutoSP = 0;
        cfgForceSP = 0;
    }

    virtual int initialize(OVDictionary *, OVService*, const char *mp);
    virtual const char* identifier() { return "OVIMArray"; }

    ~OVIMArray()
    {
        for (int i = 0; i < 4; i++) {
            if (tabs[i]) {
                delete tabs[i];
                tabs[i] = 0;
            }
        }
    }

    // Replaces the main table (array30.cin) with a user-imported table.
    // The bundled main table is still used as a fallback when the custom
    // table is missing or fails to load.
    virtual void setCustomMainTablePath(const char* path)
    {
        customMainTablePath = path ? path : "";
    }
    virtual OVInputMethodContext *newContext()
	{ 
        return new OVIMArrayContext(this, tabs);
    }
    int updateConfig(OVDictionary *conf);
    virtual const char *localizedName(const char *locale)
	{  
        if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN") || !strcasecmp(locale, "zh-Hant") || !strcasecmp(locale, "zh-Hans")) {
            strcpy(cname,"\xE8\xA1\x8C\xE5\x88\x97"); // 行列
            //if( isForceSP() )   strcat(cname, "(Q)");
            return cname;
        }
        else {
            strcpy(ename,"Array");
            //if( isForceSP() )   strcat(ename, "(Q)");
            return ename;
        }
    }
    virtual int isAutoSP() const { return cfgAutoSP; }
    virtual int isForceSP() const { return cfgForceSP; }
    virtual void setForceSP(bool value)
	{ 
        cfgForceSP = value; 
    }

    virtual void setAutoSP(bool value)
    {
        cfgAutoSP = value;
    }
};

#endif
