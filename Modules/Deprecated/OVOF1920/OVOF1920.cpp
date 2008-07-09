// OVOF1920.cpp: Wikipedia (zh) filter
// 2004-2008 By Weizhong Yang
//
// This program is Zonble-ware and adopts Zonble's License
// -- "How could a person who doesn't even believe in law adopt any license?"

#ifndef WIN32
    #include <OpenVanilla/OpenVanilla.h>
    #include <OpenVanilla/OVLibrary.h>
#else
    #include "OpenVanilla.h"
    #include "OVLibrary.h"
    #define strcasecmp stricmp
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned short convert(unsigned short s)
{
    if ( s == 0x7684) return 0x5E95; //的->底
    if ( s == 0x5403) return 0x55AB; //吃->喫
    if ( s == 0x5427) return 0x7F77; //吧->罷
    if ( s == 0x55CE) return 0x9EBC; //嗎->麼
    if ( s == 0x561B) return 0x9EBC; //嘛->麼
    if ( s == 0x53EA) return 0x7947; //只->祇
    if ( s == 0x8702) return 0x882D; //蜂->蠭
    if ( s == 0x88E1) return 0x88CF; //裡->裏
    if ( s == 0x554A) return 0x5466; //啊->呦
    if ( s == 0x500B) return 0x7B87; //個->箇
    if ( s == 0x770B) return 0x7785; //看->瞅	
    return s;
}

class OVOF1920 : public OVOutputFilter
{
public:
    OVOF1920() { u16buf=NULL; }
    int initialize(OVDictionary *cfg, OVService *srv, const char *modpath) {
        fprintf(stderr, "OVOF1920 being initialized, module path=%s\n", modpath);
        return 1;
    }
    const char *identifier() { return "OVOF1920"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
protected:
    unsigned short *u16buf;
};

OV_SINGLE_MODULE_WRAPPER(OVOF1920);

const char *OVOF1920::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "二○年代中文";
	if (!strcasecmp(locale, "zh_CN")) return "二○年代中文";
    return "Chinese in 1920s Style";
}

const char *OVOF1920::process(const char *src, OVService *srv)
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
    
    int nl=0;
    
    for (int i=0; i<l; i++)
    {
		u16buf[nl++] = convert(u16p[i]);
    }

    return srv->UTF16ToUTF8(u16buf, nl);    
}
