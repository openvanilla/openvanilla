// OVOFMorseCode.cpp

#include <OpenVanilla/OpenVanilla.h>  
#include <OpenVanilla/OVLibrary.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char *morse[26] = {
".- ","-... ","-.-. ","-.. ",". ","..-. ","--. ",".... ",
".. ",".--- ","-.- ",".-.. ","-- ","-. ","--- ",".--. ",
"--.- ",".-. ","... ","- ","..- ","...- ",".-- ","-..- ",
"-.-- ","--.. "};

char ascii[26] = {
'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
'o','p','q','r','s','t','u','v','w','x','y','z',
};

const char *convert(char code){
	int i;
	for(i = 0; i< 26; i ++){
		if(code == ascii[i])
			return morse[i];
	}
	return "";
}

class OVOFMorseCode : public OVOutputFilter
{
public:
    OVOFMorseCode() { u16buf=NULL; }
    int initialize(OVDictionary *cfg, OVService *srv, const char *modpath) {
        fprintf(stderr, "OVOFMorseCode being initialized, module path=%s\n", modpath);
        return 1;
    }
    const char *identifier() { return "OVOFMorseCode"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
protected:
	unsigned short *u16buf;
};
                       
const char *OVOFMorseCode::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "英數轉換摩斯電碼";
    if (!strcasecmp(locale, "zh_CN")) return "英数转换摩斯电码";
    return "Convert ASCII to Morse Code";
}

const char *OVOFMorseCode::process(const char *src, OVService *srv)
{
    char *back = "";
    unsigned short *u16p;
    int l=srv->UTF8ToUTF16(src, &u16p);
    
    if (!l) return src;
    u16buf=(unsigned short*)calloc(1,l*sizeof(unsigned short));
	
	for (int i=0; i<l; i++)
    {
		if (u16buf[i] > 0x7f) continue; 
 //       char *p=strchr(halfWidthChars, (char)u16buf[i]);
  //      if (p) u16buf[i]=fullWidthChars[(int)(p-halfWidthChars)];
		strcat(back, convert(u16buf[i]));
    }
	return (char *)back;
	//return srv->UTF16ToUTF8(back);
}

OV_SINGLE_MODULE_WRAPPER(OVOFMorseCode);