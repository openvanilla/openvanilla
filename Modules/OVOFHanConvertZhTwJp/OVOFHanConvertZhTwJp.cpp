// OVOFHanConvertZhTwJp.cpp: Traditional Chinese->Japan Kanji Output Filter
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

#undef OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

// externally defined in ZhTwToJpKanji.c
extern size_t ZhTwToJpKanjiTableSize;
extern unsigned short ZhTwToJpKanjiTable[];

// Linear search.
unsigned short ZhTwToJpKanji(unsigned short c) {
    extern size_t ZhTwToJpKanjiTableSize;
    extern unsigned short ZhTwToJpKanjiTable[];
    unsigned int i;
    for ( i = 0; i < ZhTwToJpKanjiTableSize; i += 2 ) {
        if(ZhTwToJpKanjiTable[i] == c) {
            return ZhTwToJpKanjiTable[i+1];
        }
    }
    return c;
}

// OVOF interface
class OVOFHanConvertZhTwJp : public OVOutputFilter {
public:
    OVOFHanConvertZhTwJp() {
        u16buf=NULL;
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
        return 1;
    }
    
    virtual const char *identifier() { return "OVOFHanConvertZhTwJp"; }
    virtual const char *localizedName(const char *locale) {
        if (!strcasecmp(locale, "zh_TW")) return "繁體中文轉日文漢字";
        if (!strcasecmp(locale, "zh_CN")) return "繁体中文转日文汉字";
        return "Traditional Chinese to Japanese Kanji";
    }
    virtual const char *process (const char *src, OVService *srv) {
        if (u16buf) {
            free(u16buf);
            u16buf=NULL;
        }
    
        unsigned short *u16p;
        int l;
        l=srv->UTF8ToUTF16(src, &u16p);

        if (!l) return src;
        u16buf=(unsigned short*)calloc(1,l*sizeof(unsigned short));
        memcpy(u16buf, u16p, l*sizeof(unsigned short));
    
        for (int i=0; i<l; i++) {
            unsigned short c=ZhTwToJpKanji(u16buf[i]);
            if (c) u16buf[i]=c; 
        }
	return srv->UTF16ToUTF8(u16buf, l);    
    }

protected:
    unsigned short *u16buf;
};

OV_SINGLE_MODULE_WRAPPER(OVOFHanConvertZhTwJp);

