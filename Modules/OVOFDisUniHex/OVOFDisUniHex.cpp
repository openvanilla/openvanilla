#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

class OVOFDisUniHex : public OVOutputFilter
{
public:
    OVOFDisUniHex() {}
    int initialize(OVDictionary *cfg, OVService *srv, const char *modpath) {
        fprintf(stderr, "OVOFDisUniHex being initialized, module path=%s\n", modpath);
        return 1;
    }
    const char *identifier() { return "OVOFDisUniHex"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
};

const char *OVOFDisUniHex::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "顯示 Unicode 字碼";
    if (!strcasecmp(locale, "zh_CN")) return "显示 Unicode 字码";
    return "Display Unicode Hex";
}

const char *OVOFDisUniHex::process(const char *src, OVService *srv)
{
    unsigned short *u16p;
    int l;
    l=srv->UTF8ToUTF16(src, &u16p);
    
    if (l) {
		unsigned short x = u16p[l-1];
		char s[200];
		sprintf(s, "Unicode\nHex: U+%X\nDec: %d", x, x);
		srv->notify((char *)s);
    }
    return srv->UTF16ToUTF8(u16p, l);
}

OV_SINGLE_MODULE_WRAPPER(OVOFDisUniHex);
