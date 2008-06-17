// OVOFWuzhetien.cpp: Wikipedia (zh) filter
// 2007 By Weizhong Yang
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

unsigned int ConversionTable[26][2] = {
	{0x7167, 0x66CC},  // 照
	{0x5929, 0xD840DC11}, // 天
	{0x5730, 0x57CA},  // 地
	{0x65E5, 0xD841DC7A}, // 日
	{0x6708, 0x56DD},  // 月
	{0x661F, 0x3007},  // 星
	{0x541B, 0xD840DC48}, // 君
	{0x81E3, 0xD849DE11}, // 臣
	{0x9664, 0xD840DC3A}, // 除
	{0x8F09, 0xD845DD40}, // 載
	{0x8F7D, 0xD845DD40}, // 载
	{0x5E74, 0xD842DC66}, // 年
	{0x6B63, 0xD841DE7A}, // 正
	{0x4EBA, 0xD852DFD4}, // 人
	{0x5E7C, 0xD848DE17}, // 幼
	{0x521D, 0xD845DD08}, // 初
	{0x570B, 0x5700},  // 國
	{0x56FD, 0x5700},  // 国
	{0x751F, 0xD842DD35}, // 生
	{0x61C9, 0xD842DE4D}, // 應
	{0x5E94, 0xD842DE4D}, // 应
	{0x8056, 0xD863DCA2}, // 聖
	{0x5723, 0xD863DCA2}, // 圣
	{0x8B49, 0xD852DE89}, // 證
	{0x8BC1, 0xD852DE89}, // 证
	{0x6388, 0xD856DC93} // 授	
};	

unsigned int convert(unsigned short s) {
	unsigned short i;
	for(i = 0; i < 26; i++) {
		if ( s == ConversionTable[i][0]) {
			return ConversionTable[i][1];
		}
	}
    return s;
}

class OVOFWuzhetien : public OVOutputFilter
{
public:
    OVOFWuzhetien() { u16buf=NULL; }
    int initialize(OVDictionary *cfg, OVService *srv, const char *modpath) {
        fprintf(stderr, "OVOFWuzhetien being initialized, module path=%s\n", modpath);
        return 1;
    }
    const char *identifier() { return "OVOFWuzhetien"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
protected:
    unsigned short *u16buf;
};

OV_SINGLE_MODULE_WRAPPER(OVOFWuzhetien);

const char *OVOFWuzhetien::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "武則天文字過濾器";
    if (!strcasecmp(locale, "zh_CN")) return "武则天文字过滤器";
    return "Wuzhetien Filter";
}

const char *OVOFWuzhetien::process(const char *src, OVService *srv)
{
    if (u16buf) {
        free(u16buf);
        u16buf=NULL;
    }
    
    unsigned short *u16p;
    int l;
    l=srv->UTF8ToUTF16(src, &u16p);
    
    if (!l) return src;
    u16buf=(unsigned short*)calloc(1,l*sizeof(unsigned short)*2);
    memcpy(u16buf, u16p, l*sizeof(unsigned short));
    
    size_t u16ptr = 0;
    
    for (int i=0; i<l; i++)
    {
        unsigned result = convert(u16p[i]);
        
        if (result > 0xffff) {
           // got a surrogate pair, feed in the first then the second
           u16buf[u16ptr++] = (result & 0xffff0000) >> 16;
           u16buf[u16ptr++] = (result & 0x0000ffff);
        }
	    else {
	       u16buf[u16ptr++] = result;
	    }	
    }

    return srv->UTF16ToUTF8(u16buf, u16ptr);    
}
