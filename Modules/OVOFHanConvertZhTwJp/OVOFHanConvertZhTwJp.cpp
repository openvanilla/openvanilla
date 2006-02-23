#define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

// externally defined in ZhTwToJpKanji.c
extern size_t ZhTwToJpKanjiTableSize;
extern unsigned short ZhTwToJpKanjiTable[];

// Linear search.
unsigned short ZhTwToJpKanji(unsigned short c) {
    extern size_t ZhTwToJpKanjiTableSize;
    extern unsigned short ZhTwToJpKanjiTable[];
    unsigned int i;
    for ( i = 0; i < ZhTwToJpKanjiTableSize; i += 2 ) {
        if(ZhTwToJpKanjiTable[i] == c) {
            return ZhTwToJpKanjiTable[i+1];
        }
    }
    return c;
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

