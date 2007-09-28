// OVOFAntiZhuyinwen: Anti-Zhuyinwen filter
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

#import <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#import "AZYWController.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

AZYWController *ac;

unsigned short zhuyinsymbols[41]=
{
    0x3105, 0x3106, 0x3107, 0x3108, 0x3109, 0x310a, 0x310b, 0x310c, 0x310d, 0x310e, 
    0x310f, 0x3110, 0x3111, 0x3112, 0x3113, 0x3114, 0x3115, 0x3116, 0x3117, 0x3118, 
    0x3119, 0x3127, 0x3128, 0x3129, 0x311a, 0x311b, 0x311c, 0x311d, 0x311e, 0x311f, 
    0x3120, 0x3121, 0x3122, 0x3123, 0x3124, 0x3125, 0x3126, 0x02ca, 0x02c7, 0x02cb, 
    0x02d9
};

int isZhuyin(unsigned short s)
{
    for (int i=0; i<41; i++) if (s==zhuyinsymbols[i]) return 1;
    return 0;
}

class OVOFAntiZhuyinwen : public OVOutputFilter
{
public:
    OVOFAntiZhuyinwen() { u16buf=NULL; }
    int initialize(OVDictionary *cfg, OVService *srv, const char *modpath) {
        fprintf(stderr, "OVOFAntiZhuyinwen being initialized, module path=%s\n", modpath);
		NSApplicationLoad();
		ac=[[AZYWController alloc] initWithWindowNibName:@"OVOFAntiZhuyinwen"];
		if (ac) {
			NSLog(@"Controller loaded!");
		}
		[[ac window] orderOut: nil];	
        return 1;
    }
    const char *identifier() { return "OVOFAntiZhuyinwen"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
protected:		
    unsigned short *u16buf;
};

const char *OVOFAntiZhuyinwen::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "\xE6\xB3\xA8\xE9\x9F\xB3\xE6\x96\x87\xE9\x80\x80\xE6\x95\xA3\xE6\xBF\xBE\xE5\x98\xB4";//"注音文退散濾嘴";
    if (!strcasecmp(locale, "zh_CN")) return "\xE6\xB3\xA8\xE9\x9F\xB3\xE6\x96\x87\xE9\x80\x80\xE6\x95\xA3\xE6\xBB\xA4\xE5\x98\xB4";//"注音文退散滤嘴";
    return "Anti-Zhuyinwen filter";
}

const char *OVOFAntiZhuyinwen::process(const char *src, OVService *srv)
{
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
    
    int nl=0, haszhuyin=0;
    
    for (int i=0; i<l; i++)
    {
        if (!isZhuyin(u16p[i])) u16buf[nl++]=u16p[i];
        else haszhuyin=1;
    }
    
    if (haszhuyin)
    {
		[ac show];
    }

    return srv->UTF16ToUTF8(u16buf, nl);    
}

OV_SINGLE_MODULE_WRAPPER(OVOFAntiZhuyinwen);
