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
	char hex[1024], dec[1024], buf[1024], s[2048];
	for(int j =0; j < l; j++){
	   unsigned short x = u16p[j];
	   const char *u8 = srv->UTF16ToUTF8(&(u16p[j]), 1);
	   sprintf(buf, "%s=(U+%X) ", u8, x);
	   strcat(hex, buf);
	   sprintf(buf, "%s=(%d) ", u8, x);
	   strcat(dec, buf);
	}
	sprintf(s, "Hex: %s\nDec: %s", hex, dec);
	srv->notify((char *)s);
    }
    //return src;
    return srv->UTF16ToUTF8(u16p, l);
}

OV_SINGLE_MODULE_WRAPPER(OVOFDisUniHex);
