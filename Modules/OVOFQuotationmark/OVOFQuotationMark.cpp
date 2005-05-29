#define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

/*
English: “ ” 
German: „ “
Netherland: ””
Netherland Alternative: „ ”
France: « »
Danemark: » «
Sweden: » »

English: ‘’
German: ‚‛
Netherlan: ’’
Netherland Alternative: ‚ ’
France: ‹ ›
Danemark: › ‹
Sweden: › ›
*/

unsigned short the_double[7][2] = {
	{0x201C, 0x201D,}, 
	{0x201E, 0x201C,},
	{0x201D, 0x201D,},
	{0x201E, 0x201D,},
	{0x00AB, 0x00BB,},
	{0x00BB, 0x00AB,},
	{0x00BB, 0x00BB,},
};

unsigned short the_single[7][2] = {
	{0x2018, 0x2019},
	{0x201A, 0x201B},
	{0x2019, 0x2019},
	{0x201A, 0x2019},
	{0x2039, 0x203A},
	{0x203A, 0x2039},
	{0x203A, 0x203A},
};

class OVOFSmartQuote : public OVOutputFilter {
public:
    OVOFSmartQuote() {u16buf=NULL;dp=0;sp=0;}
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
        return 1;}
    
    virtual const char *identifier() { return "OVOFSmartQuote"; }
    virtual const char *localizedName(const char *locale)
	{
		if (!strcasecmp(locale, "zh_TW")) return "智慧型英文引號";
		if (!strcasecmp(locale, "zh_CN")) return "智慧型英文引号";
		return "Smart Quote - English";
	}
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
		
		int nl=0;
		
		for (int i=0; i<l; i++)
		{
			if ( u16p[i] == '"') {
				u16buf[nl++] = the_double[0][dp];
				dp = (dp+ 1) % 2;
			} else if (u16p[i] == '\''){
				u16buf[nl++] = the_single[0][sp];
				sp = (sp+ 1) % 2;
			} else {
				u16buf[nl++] = u16p[i];
			}
		}
		
		return srv->UTF16ToUTF8(u16buf, nl);  
    }
    
protected:
	unsigned short *u16buf;
    char *buf;
	int dp;
	int sp;
};

//----

class OVOFSmartQuoteGerman : public OVOutputFilter {
public:
    OVOFSmartQuoteGerman() {u16buf=NULL;dp=0;sp=0;}
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
						   const char *modulePath) {
        return 1;}
    
    virtual const char *identifier() { return "OVOFSmartQuoteGerman"; }
    virtual const char *localizedName(const char *locale)
	{
		if (!strcasecmp(locale, "zh_TW")) return "智慧型德文引號";
		if (!strcasecmp(locale, "zh_CN")) return "智慧型德文引号";
		return "Smart Quote - German";
	}
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
		
		int nl=0;
		
		for (int i=0; i<l; i++)
		{
			if ( u16p[i] == '"') {
				u16buf[nl++] = the_double[1][dp];
				dp = (dp+ 1) % 2;
			} else if (u16p[i] == '\''){
				u16buf[nl++] = the_single[1][sp];
				sp = (sp+ 1) % 2;
			} else {
				u16buf[nl++] = u16p[i];
			}
		}
		
		return srv->UTF16ToUTF8(u16buf, nl);  
    }
    
protected:
	unsigned short *u16buf;
    char *buf;
	int dp;
	int sp;
};

//----

class OVOFSmartQuoteFrance : public OVOutputFilter {
public:
    OVOFSmartQuoteFrance() {u16buf=NULL;dp=0;sp=0;}
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
						   const char *modulePath) {
        return 1;}
    
    virtual const char *identifier() { return "OVOFSmartQuoteFrance"; }
    virtual const char *localizedName(const char *locale)
	{
		if (!strcasecmp(locale, "zh_TW")) return "智慧型法文引號";
		if (!strcasecmp(locale, "zh_CN")) return "智慧型法文引号";
		return "Smart Quote - France";
	}
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
		
		int nl=0;
		
		for (int i=0; i<l; i++)
		{
			if ( u16p[i] == '"') {
				u16buf[nl++] = the_double[4][dp];
				dp = (dp+ 1) % 2;
			} else if (u16p[i] == '\''){
				u16buf[nl++] = the_single[4][sp];
				sp = (sp+ 1) % 2;
			} else {
				u16buf[nl++] = u16p[i];
			}
		}
		
		return srv->UTF16ToUTF8(u16buf, nl);  
    }
    
protected:
	unsigned short *u16buf;
    char *buf;
	int dp;
	int sp;
};


extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService* s, const char* p) { 
    return 1; 
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
	switch (idx) {
		case 0: return new OVOFSmartQuote;
		case 1: return new OVOFSmartQuoteGerman;
		case 2: return new OVOFSmartQuoteFrance;
	}
	return NULL;
}

