#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* The Chinese versioon Wikipedia (zh.wikipedia.org) use 0x00B7 as the standard 
full-width dot, however, most input methods provide 0xFF0E but not 0x00B7. While
writing a WikiWord on Wikipedia, I think a filter to convert the character is 
required, at least I need such a utility.*/

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
    if (!strcasecmp(locale, "zh_TW")) return "中文 Wikipedia 文字格式過濾器";
    if (!strcasecmp(locale, "zh_CN")) return "中文 Wikipedia 文字格式过滤器";
    return "Zh-Wikipedia Text Format filter";
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
