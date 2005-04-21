#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
        return 1;
    }
    const char *identifier() { return "OVOFAntiZhuyinwen"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
protected:
    unsigned short *u16buf;
};

OV_SINGLE_MODULE_WRAPPER(OVOFAntiZhuyinwen);

const char *OVOFAntiZhuyinwen::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "注音文退散濾嘴";
    if (!strcasecmp(locale, "zh_CN")) return "注音文退散滤嘴";
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
        if (!strcasecmp(srv->locale(), "zh_TW") || 
            !strcasecmp(srv->locale(), "zh_CN"))
            srv->notify("注音文退散！注音文退散！");
        else
            srv->notify("'Zhuyinwen' found and filtered");
    }

    return srv->UTF16ToUTF8(u16buf, nl);    
}
