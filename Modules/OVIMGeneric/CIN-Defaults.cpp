// CIN-Defaults.cpp: Setting default values for common .cin's
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

#include "CIN-Defaults.h"
#include <stdlib.h>
#include <strings.h>

#ifndef WIN32
#else
	#define strcasecmp stricmp
#endif

bool CINSetConfig(const char *sn, OVDictionary *d, const char *tn, int ln, 
    int ac=0, int hit=0, int ssk=0, int bp=1);

bool CINSetConfig(const char *sn, OVDictionary *d, const char *tn, int ln, 
    int ac, int hit, int ssk, int bp)
{
    if (sn && tn) {
        if (strcasecmp(sn, tn)) return false;
    }

    #define CFGSET(var, key) \
        if (var!=-1) { if (!d->keyExist(key)) d->setInteger(key, var); }

    CFGSET(ln, CIN_MAXSEQLEN);
    CFGSET(ac, CIN_AUTOCOMPOSE);
    CFGSET(hit, CIN_HITMAX);
    CFGSET(ssk, CIN_SHIFTSELECTIONKEY);
    CFGSET(bp, CIN_WARNINGBEEP);
    #undef CFGSET
    return true;
}

bool CINSetDefaults(const char *shortname, OVDictionary *dict) {
    #define S CINSetConfig(shortname, dict
    #define R ) return true
    
    if (S, "biaoyin.cin", 7) R;
    if (S, "bpmf-symbol.cin", 1, 0, 1) R; // maxlen=1, hitmax&compose
    if (S, "cj-ext.cin", 5) R;            // all cj* maxlen=5
    if (S, "cj-j.cin", 5) R;
    if (S, "cj.cin", 5) R;
    if (S, "cj5.cin", 5) R;
    if (S, "corner.cin", 4, 0, 1) R;      // maxlen=4, hitmax&compose
    if (S, "daibuun.cin", 12) R;
    if (S, "dayi3.cin", 4, 0, 0, 1) R;    // maxlen=4, shift sel key
    if (S, "ehq-symbols.cin", 10, 1, 0) R; // maxlen=10, autocompose
    if (S, "jinjin.cin", 10) R;
    if (S, "jtcj.cin", 7) R;
    if (S, "jyutping.cin", 7) R;
    if (S, "klingon.cin", 1, 0, 1) R;     // maxlen=1, autocompose, hitmax&cmpse.
    if (S, "kk.cin", 2, 0, 1) R;          // maxlen=2, hitmax&cmpse.
    if (S, "liu57.cin", 4, 1, 1) R;       // maxlen=4, autocompose, hitmax&cmpse.
    if (S, "pictograph.cin", 4, 1, 1) R;
    if (S, "pinyin.cin", 50, 1) R;        // maxlen=50, autocompose
    if (S, "pinyinbig5.cin", 7) R;
    if (S, "qcj.cin", 2, 0, 1) R;         // qcj=simplex
    if (S, "scj7.cin", 5) R;              // scj7=cj
    if (S, "shuangpin.cin", 4, 0, 1) R;   // maxlen=4, hitmax&compose
    if (S, "simplex-ext.cin", 2, 0, 1) R; // maxlen=2, hitmax&compose
    if (S, "simplex.cin", 2, 0, 1) R;
    if (S, "tcj.cin", 5) R;               // tcj=cj
    if (S, "telecode.cin", 5, 1, 1) R;    // maxlen=5, autocompose, hitmax&cmpse.
    if (S, "wu.cin", 14) R;
    if (S, "wubizixing.cin", 4, 0, 1) R;  // maxlen=4, hitmax&cmpse;
    if (S, "wus.cin", 13) R;
    if (S, "wut.cin", 13) R;
    
    CINSetConfig(NULL, dict, NULL, 7);
    return true;

    #undef S
    #undef R
}

