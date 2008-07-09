// OVOFZhWikipidealize.cpp: Wikipedia (zh) filter
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

// The Chinese versioon Wikipedia (zh.wikipedia.org) use 0x00B7 as the
// standard full-width dot, however, most input methods provide 0xFF0E 
// but not 0x00B7. While writing a WikiWord on Wikipedia, I think a filter 
// to convert the character is required, at least I need such a utility.

unsigned short convert(unsigned short s)
{
    if ( s == 0xFF0E) return 0x00B7;
    return s;
}

class OVOFZhWikipidealize : public OVOutputFilter
{
public:
    OVOFZhWikipidealize() { u16buf=NULL; }
    int initialize(OVDictionary *cfg, OVService *srv, const char *modpath) {
        fprintf(stderr, "OVOFZhWikipidealize being initialized, module path=%s\n", modpath);
        return 1;
    }
    const char *identifier() { return "OVOFZhWikipidealize"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
protected:
    unsigned short *u16buf;
};

OV_SINGLE_MODULE_WRAPPER(OVOFZhWikipidealize);

const char *OVOFZhWikipidealize::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "\xE4\xB8\xAD\xE6\x96\x87 Wikipedia \xE6\x96\x87\xE5\xAD\x97\xE6\xA0\xBC\xE5\xBC\x8F\xE9\x81\x8E\xE6\xBF\xBE\xE5\x99\xA8";//"中文Wikipedia文字格式過濾器";
    if (!strcasecmp(locale, "zh_CN")) return "\xE4\xB8\xAD\xE6\x96\x87 Wikipedia \xE6\x96\x87\xE5\xAD\x97\xE6\xA0\xBC\xE5\xBC\x8F\xE8\xBF\x87\xE6\xBB\xA4\xE5\x99\xA8";//"中文Wikipedia文字格式过滤器";
    return "zh.Wikipedia Character Convertor";
}

const char *OVOFZhWikipidealize::process(const char *src, OVService *srv)
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
