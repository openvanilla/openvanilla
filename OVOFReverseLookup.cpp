// OVOFReverseLookup.cpp: Reverse lookup module for .cin files
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
//#define OV_DEBUG

#include "OVOFReverseLookup.h"
#if !defined(WIN32) && !defined(WINCE)
    #include <OpenVanilla/OVLibrary.h>
    #include <OpenVanilla/OVUtility.h>
#else
    #include "OVLibrary.h"
    #include "OVUtility.h"
#endif

#include "OVOSDef.h"

#include <cstdlib>

using namespace std;

extern SQLite3 *db;

OVOFReverseLookup::OVOFReverseLookup(const OVCINSQInfo &ci) : cininfo(ci), cintab(0) {
    idstr = "OVOFReverseLookup-" + cininfo.shortfilename;
}

OVOFReverseLookup::~OVOFReverseLookup() {
    if (cintab) delete cintab;
}

const char* OVOFReverseLookup::identifier() {
    return idstr.c_str();
}

const char* OVOFReverseLookup::localizedName(const char* locale) {

	
	if (!strcasecmp(locale, "zh_TW")) localizedname = cininfo.tcname + string("-\xE5\x8F\x8d\xE6\x9F\xA5"); 
    else if (!strcasecmp(locale, "zh_CN")) localizedname = cininfo.scname + string("-\xE5\x8F\x8d\xE6\x9F\xA5");
    else localizedname = cininfo.ename + string("-Reverse Look Up");

	return localizedname.c_str();

}

int OVOFReverseLookup::initialize(OVDictionary* global, OVService*, const char*) {
    if (!cintab) {
	    murmur("OVOFRevereseLookup: initializing %s", identifier());
        cintab=new OVCINSQ(cininfo, db);
     }
	else {
		murmur("Already initialized");
	}
    return 1;
}

const char *OVOFReverseLookup::process(const char *src, OVService *srv) {
    unsigned short *u16;
    int u16len=srv->UTF8ToUTF16(src, &u16);
    
    string result;
    
    for (int i=0; i<u16len; i++) {
        // get each codepoint -- and do surrogate check
        
        const char *u8;
        if (u16[i] >= 0xd800 && u16[i] <= 0xdbff) {
            u8=srv->UTF16ToUTF8(&(u16[i]), 2);
            i++;
        }
        else {
            u8=srv->UTF16ToUTF8(&(u16[i]), 1);
        }
        
        if (u8==NULL) {
			/*
            if (!strcasecmp(srv->locale(), "zh_TW")) {
                srv->notify("?�查失�?：Unicode字碼?�誤");
            }
            else if (!strcasecmp(srv->locale(), "zh_CN")) {
                srv->notify("?�查失败：Unicode字�??�误");
            }
            else {
                srv->notify("Look-up failed: Bad Unicode codepoint");
            }*/
            return src;
        }
        
        char buf[512];
        
        vector<string> lookupvector, key;
        string code;
        string seperator(", ");

        // look up each "candidate"
        size_t size=cintab->getCharVectorByWord(u8, lookupvector);
        for (size_t j=0; j<size; j++) {
            const char *lvstr=lookupvector[j].c_str();
            
            // compose the "character parts"
            for (size_t k=0; k<strlen(lvstr); k++) {
                char kbuf[2];
                sprintf(kbuf, "%c", tolower(lvstr[k]));
                string kstr(kbuf);
    
                vector<string> outStringVectorRef;
                if(cintab->getCharVectorByKey(kstr, outStringVectorRef) > 0)
                    code+=outStringVectorRef[0];
                else
                    code+=kstr;
            }
        
            if (j!=size-1) code += seperator;
        }
    
        if (size) {
            // if( size > 1 ) code = "\n" + code;
            sprintf(buf, "%s: %s", u8, code.c_str());
            string bstr(buf);
            if (result.size()) result += "\n";
            result += bstr;
    	}
    }
	murmur("%s",result.c_str());
    if(strlen(result.c_str())) srv->notify(result.c_str());
    return src;
}

