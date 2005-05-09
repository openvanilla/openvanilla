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
    OVOFMorseCode() {
        buf=NULL;
        lastlen=0;
    }
    int initialize(OVDictionary *cfg, OVService *srv, const char *modpath) {
        return 1;
    }
    const char *identifier() { return "OVOFMorseCode"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
protected:
    char *buf;
    size_t lastlen;
};
                       
const char *OVOFMorseCode::localizedName(const char *locale)
{
    if (!strcasecmp(locale, "zh_TW")) return "英數轉換摩斯電碼";
    if (!strcasecmp(locale, "zh_CN")) return "英数转换摩斯电码";
    return "Convert ASCII to Morse Code";
}

const char *OVOFMorseCode::process(const char *src, OVService *srv)
{
    if (!src || !strlen(src)) return src;
    if (lastlen < strlen(src)) {
        if (buf) free(buf);
        lastlen=strlen(src);
        buf=(char*)calloc(1, lastlen*4+1);  // a char may become 4 chars
    }

    strcpy(buf, "");
    
	for (size_t i=0; i<strlen(src); i++)
    {
        char c=toupper(src[i]);     // it's a UTF-8 string, so always safe
        if (c >= 'A' && c <='Z') {
            strcat(buf, morse[c-'A']);
        }
        else {
            char nstr[2];
            nstr[0]=c;
            nstr[1]=0;
            strcat(buf, nstr);
        }
    }
    
    return buf;
}

OV_SINGLE_MODULE_WRAPPER(OVOFMorseCode);