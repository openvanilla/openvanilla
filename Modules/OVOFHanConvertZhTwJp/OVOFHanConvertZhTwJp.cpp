#define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

// Size copied from ZhTwToJpKanji.c
const size_t ZhTwToJpKanjiTableSize = 8195;
extern unsigned short ZhTwToJpKanjiTable[];

// VXHC* copied from OVOFHanConvert.cpp
struct VXHCData {
	unsigned short key, value;
};

int VXHCCompare(const void *a, const void *b) {
	unsigned short x=((const struct VXHCData*)a)->key, y=((const struct VXHCData*)b)->key;
	if (x==y) return 0;
	if (x<y) return -1;
	return 1;
}

unsigned short VXHCFind(unsigned key, unsigned short *table, size_t size) {
	struct VXHCData k;
	k.key=key;
	struct VXHCData *d=(struct VXHCData*)bsearch(&k, table, size, sizeof(struct VXHCData), VXHCCompare);
	if (!d) return 0;
	return d->value;
}

// Convert between utf16
unsigned short ZhTwToJpKanji(unsigned short c) {
    return VXHCFind(c, ZhTwToJpKanjiTable, ZhTwToJpKanjiTableSize);
}

// OVOF interface
class OVOFHanConvertZhTwJp : public OVOutputFilter {
public:
    OVOFHanConvertZhTwJp() {
        u16buf=NULL;
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
        return 1;
    }
    
    virtual const char *identifier() { return "OVOFHanConvertZhTwJp"; }
    virtual const char *process (const char *src, OVService *srv) {
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
    
        for (int i=0; i<l; i++) {
            unsigned short c=ZhTwToJpKanji(u16buf[i]);
            if (c) u16buf[i]=c; 
        }
	return srv->UTF16ToUTF8(u16buf, l);    
    }

protected:
    unsigned short *u16buf;
};

OV_SINGLE_MODULE_WRAPPER(OVOFHanConvertZhTwJp);

