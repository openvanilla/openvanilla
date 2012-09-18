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

#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
#else
	#include "OpenVanilla.h"

	//<comment author='b6s'> Uses OVOSDef.h instead.
	//#include <string.h>
	//#define strcasecmp stricmp
	//</comment>
#endif
#include "OVOSDef.h"

#include "ArrayKeySequence.h"
#include "OVCandidateServiceList.h"
#include "OVCINDataTable.h"
#include <cstring>

namespace OV_Array {
    /*
    enum STATE {
        STATE_WAIT_KEY1 = 0,
        STATE_WAIT_KEY2,
        STATE_WAIT_KEY3,
        STATE_WAIT_CANDIDATE
    };

    enum {
        MAIN_TAB = 0,
        SHORT_TAB = 1,
        SPECIAL_TAB = 2
    };
    const int RET_PASS = 0;
    const int RET_DONE = 1;
    const int RET_CONTINUE = 2;
    */
};


class OVIMArray;
class OVIMArrayContext : public OVInputMethodContext
{
    /*
private:
    OVIMArray* parent;
    OVCINDataTable **tabs;
    ArrayKeySequence keyseq;
    OV_Array::STATE state;
    OVCandidateServiceList candi; */
    std::vector<std::string> candidateStringVector, specialCodeVector;

    //OVTextBuffer* _buf;
    //OVCandidateService* _candibar;
    //OVLoaderService* _srv;

    /*
public:
    OVIMArrayContext(OVIMArray* p, OVCINDataTable** t)
        : parent(p), tabs(t), keyseq(t[OV_Array::MAIN_TAB])
	{
        state = OV_Array::STATE_WAIT_KEY1;
    }
    */

    /*
    virtual int keyEvent(OVKeyCode* , OVTextBuffer* , OVCandidateService* , OVLoaderService* );
    virtual void clear();
    */
private:
    /*
    void changeState(OV_Array::STATE s);
    void changeBackState(OV_Array::STATE s);
    */
    /*
    virtual void updateDisplay(OVTextBuffer*);
    void dispatchStateHandler(OVKeyCode* , OVTextBuffer* , OVCandidateService* , OVLoaderService* );
    int WaitKey1(OVKeyCode* , OVTextBuffer* , OVCandidateService* , OVLoaderService* );
    int WaitKey2(OVKeyCode* , OVTextBuffer* , OVCandidateService* , OVLoaderService* );
    int WaitKey3(OVKeyCode* , OVTextBuffer* , OVCandidateService* , OVLoaderService* );
    int WaitCandidate(OVKeyCode* , OVTextBuffer* , OVCandidateService* , OVLoaderService* );
    int updateCandidate(OVCINDataTable *tab,OVTextBuffer *buf, OVCandidateService *candibar);
    void sendAndReset(const char *, OVTextBuffer* , OVCandidateService* , OVLoaderService* );
    void clearAll(OVTextBuffer* buf, OVCandidateService* candi_bar);
    void clearCandidate(OVCandidateService *candi_bar);
    int selectCandidate(int num, string& out);
    */
    /*
    bool isWSeq(char a, char b) const {  return a == 'w' && isdigit(b);    }
    bool isForceSPSeq()
	{
        return keyseq.length() == 4 && !memcmp(keyseq.getSeq(),",,sp",4);
    }
    */
    /* void queryKeyName(const char *keys, std::string& outKeyNames); */
};

/*
class OVIMArray : public OVInputMethod
{
private:
    char cname[128], ename[128];
    OVCINDataTable *tabs[3];    // main, short-code, special-code
    int cfgAutoSP, cfgForceSP;

public:
    virtual int initialize(OVDictionary *, OVLoaderService*, const char *mp);
    virtual const char* identifier() { return "OVIMArray"; }
    virtual OVInputMethodContext *newContext()
	{ 
        return new OVIMArrayContext(this,tabs); 
    }
    int updateConfig(OVDictionary *conf);
    virtual const char *localizedName(const char *locale)
	{  
        if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN") || !strcasecmp(locale, "zh-Hant") || !strcasecmp(locale, "zh-Hans")) {
            strcpy(cname,"\xE8\xA1\x8C\xE5\x88\x97"); //¦æ¦C
            //if( isForceSP() )   strcat(cname, "(§Ö)");
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
};

*/
#endif
