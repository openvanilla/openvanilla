// OVOFFullWidthCharacter.cpp

#include "OVOFFullWidthCharacter.h"
#include <OpenVanilla/OVLibrary.h>
#include <strings.h>
#include <ctype.h>

OVSINGLEMODULEWRAPPER(OVOFFullWidthCharacter);

unsigned short fullWidthChars[95]=
{
    0x3000, 0xff01, 0x201d, 0xff03, 0xff04, 0xff05, 0xff06, 0x2019, 0xff08, 0xff09, 
    0xff0a, 0xff0b, 0xff0c, 0xff0d, 0xff0e, 0xff0f, 0xff10, 0xff11, 0xff12, 0xff13, 
    0xff14, 0xff15, 0xff16, 0xff17, 0xff18, 0xff19, 0xff1a, 0xff1b, 0xff1c, 0xff1d, 
    0xff1e, 0xff1f, 0xff20, 0xff21, 0xff22, 0xff23, 0xff24, 0xff25, 0xff26, 0xff27, 
    0xff28, 0xff29, 0xff2a, 0xff2b, 0xff2c, 0xff2d, 0xff2e, 0xff2f, 0xff30, 0xff31, 
    0xff32, 0xff33, 0xff34, 0xff35, 0xff36, 0xff37, 0xff38, 0xff39, 0xff3a, 0x3014, 
    0xff3c, 0x3015, 0xff3e, 0xff3f, 0x2018, 0xff41, 0xff42, 0xff43, 0xff44, 0xff45, 
    0xff46, 0xff47, 0xff48, 0xff49, 0xff4a, 0xff4b, 0xff4c, 0xff4d, 0xff4e, 0xff4f, 
    0xff50, 0xff51, 0xff52, 0xff53, 0xff54, 0xff55, 0xff56, 0xff57, 0xff58, 0xff59, 
    0xff5a, 0xff5b, 0xff5c, 0xff5d, 0xff5e
};

char *halfWidthChars=" !\"#$%&'()*+,-./0123456789:;<=>?"
                     "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                     "abcdefghijklmnopqrstuvwxyz{|}~";
                       
const char *OVOFFullWidthCharacter::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "全形英數字";
    if (!strcasecmp(locale, "zh_CN")) return "全角英数字";
    return "Full width character";
}

const char *OVOFFullWidthCharacter::process(const char *src, OVService *srv)
{
    const char *utf16t=srv->nativeUTF16EncodingType();
    unsigned short *utf16buf=new unsigned short [strlen(src)];
    int l=srv->fromUTF8(utf16t, src, utf16buf);
    
    for (int i=0; i<l; i++)
    {
        if (utf16buf[i] > 0x7f) continue; 
        char *p=strchr(halfWidthChars, (char)utf16buf[i]);
        if (p) utf16buf[i]=fullWidthChars[(int)(p-halfWidthChars)];
    }
    
    const char *r=srv->toUTF8(utf16t, utf16buf, l);
    delete[] utf16buf;
    return r;
}
