// OVOFDisUniHex.cpp: Decode Unicode
// 2004-2006 By Weizhong Yang
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
#include <string>

using namespace std;

class OVOFDisUniHex : public OVOutputFilter
{
public:
    int initialize(OVDictionary*, OVService*, const char *) {
        return 1;
    }
    const char *identifier() { return "OVOFDisUniHex"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
};

const char *OVOFDisUniHex::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "顯示Unicode字碼";
    if (!strcasecmp(locale, "zh_CN")) return "显示Unicode字码";
    return "Unicode Decoder";
}

const char *OVOFDisUniHex::process(const char *src, OVService *srv)
{
    unsigned short *u16p;
    int l=srv->UTF8ToUTF16(src, &u16p);
    string display;

	for(int j=0; j<l; j++) {
       char msg[512];
       const char *u8;
       unsigned int h=u16p[j];
       
       if (h >= 0xd800 && h <= 0xdc00) {
            int lo;
            u8=srv->UTF16ToUTF8(u16p+j, 2);
            lo=u16p[++j];

            const unsigned int suroff = 0x10000 - (0xD800 << 10) - 0xDC00;
            unsigned int u32= (h << 10) + lo + suroff;
            sprintf(msg, "%s=U+%X (%d)=UTF-16 %X %X", u8, u32, u32, h, lo);
	   }
        else {
           u8=srv->UTF16ToUTF8(u16p+j, 1);
           sprintf(msg, "%s=U+%X (%d)", u8, h, h);
        }

        strcat(msg, "=");
        for (size_t k=0; k<strlen(u8); k++) {
            char u8code[16];
            sprintf(u8code, "%X", (unsigned char)u8[k]);
            strcat(msg, u8code);
            if (k<strlen(u8)-1) strcat(msg, " ");
        }

        if (j < l-1) strcat(msg, "\n");
        display+=msg;
	}
	srv->notify(display.c_str());
	return src;
}

OV_SINGLE_MODULE_WRAPPER(OVOFDisUniHex);
