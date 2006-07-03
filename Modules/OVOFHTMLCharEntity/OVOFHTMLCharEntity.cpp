// OVOFHTMLCharEntity.cpp: Converts characters to HTML entities
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

#ifndef WIN32
    #include <OpenVanilla/OpenVanilla.h>
    #include <OpenVanilla/OVLibrary.h>
    #include <OpenVanilla/OVUtility.h>
#else
    #include "OpenVanilla.h"
    #include "OVLibrary.h"
    #include "OVutility.h"
    #define strcasecmp stricmp
#endif
#include <map>
#include <string>

using namespace std;

static char *entityTable[] = {
    "&", "&amp;",
    ">", "&gt;",
    "<", "&lt;",
    "\"", "&quot;",
    "'", "&apos;",
    NULL
};

class OVOFHTMLCharEntity : public OVOutputFilter {
private:
    typedef map<string, const char*> EntityMap;
    EntityMap entityMap;
    string buf;
public:
    OVOFHTMLCharEntity() {}
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
        for(int i=0; entityTable[i] ; i+=2){
            entityMap.insert( make_pair( entityTable[i], entityTable[i+1]) );
        }
        return 1;
    }

    
    virtual const char *identifier() { return "OVOFHTMLCharEntity"; }

    virtual const char *localizedName(const char *locale)
    {       
	    if (!strcasecmp(locale, "zh_TW")) return "字碼轉換 HTML Entity";
	    if (!strcasecmp(locale, "zh_CN")) return "字码转换成 HTML Entity";
	    return "Convert Characters to HTML Entities";
    } 

    virtual const char *process (const char *src, OVService *srv) {
        int len = strlen(src);
        buf.clear();
        for(int i=0;i<len;i++){
            EntityMap::const_iterator it;
            string ch;
            ch.push_back(src[i]);
            if( (it = entityMap.find(ch)) != entityMap.end() )
                buf.append(it->second) ;
            else
                buf.append(ch);
        }
        return buf.c_str();
    }
    
};

 OV_SINGLE_MODULE_WRAPPER(OVOFHTMLCharEntity);
