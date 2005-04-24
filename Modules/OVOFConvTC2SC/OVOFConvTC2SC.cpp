// OVOFConvTC2SC.cpp

#include <OpenVanilla/OpenVanilla.h>  
#include <OpenVanilla/OVLibrary.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

const size_t vxTC2SCTableSize=3214;
extern unsigned short vxTC2SCTable[];

struct VXHCData
{
	unsigned short key, value;
};

int VXHCCompare(const void *a, const void *b)
{
	unsigned short x=((const struct VXHCData*)a)->key, y=((const struct VXHCData*)b)->key;
	if (x==y) return 0;
	if (x<y) return -1;
	return 1;
}

unsigned short VXHCFind(unsigned key, unsigned short *table, size_t size)
{
	struct VXHCData k;
	k.key=key;
	struct VXHCData *d=(struct VXHCData*)bsearch(&k, table, size, sizeof(struct VXHCData), VXHCCompare);
	if (!d) return 0;
	return d->value;
}

unsigned short VXUCS2TradToSimpChinese(unsigned short c)
{
	return VXHCFind(c, vxTC2SCTable, vxTC2SCTableSize);
}


class OVOFConvTC2SC : public OVOutputFilter
{
public:
    OVOFConvTC2SC() { u16buf=NULL; }
    int initialize(OVDictionary *cfg, OVService *srv, const char *modpath) {
        fprintf(stderr, "OVOFConvTC2SC being initialized, module path=%s\n", modpath);
        return 1;
    }
    const char *identifier() { return "OVOFConvTC2SC"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
protected:
	unsigned short *u16buf;
};
                       
const char *OVOFConvTC2SC::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "正體中文轉簡體";
    if (!strcasecmp(locale, "zh_CN")) return "繁体中文转简体";
    return "Traditional Chinese to Simpified Chinese";
}

const char *OVOFConvTC2SC::process(const char *src, OVService *srv)
{
    if (u16buf) {
        free(u16buf);
        u16buf=NULL;
    }
    
    unsigned short *u16p;
    int l=srv->UTF8ToUTF16(src, &u16p);
    
    if (!l) return src;
    u16buf=(unsigned short*)calloc(1,l*sizeof(unsigned short));
    memcpy(u16buf, u16p, l*sizeof(unsigned short));
	
	for (int i=0; i<l; i++)
    {
		unsigned short c=VXUCS2TradToSimpChinese(u16buf[i]);
		if (c) u16buf[i]=c; 
    }
	return srv->UTF16ToUTF8(u16buf, l);  
}

OV_SINGLE_MODULE_WRAPPER(OVOFConvTC2SC);