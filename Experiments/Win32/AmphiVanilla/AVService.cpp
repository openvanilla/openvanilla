#include "AVService.h"
#include "iconv.h"
#include <cstring>

const char *AVService::pathSeparator()
{
#ifdef WIN32
	return "\\"; 
#else
	return "/";
#endif
}

const char *AVService::toUTF8(const char *encoding, const char *src)
{
	char *out = NULL;
	size_t inbytesleft = strlen(src) + 1;
	size_t outbytesleft = 1024;
	iconv_t cd;
	memset(internal, 0, 1024);
	out = internal;
	cd = iconv_open("UTF-8", encoding);
	iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	iconv_close(cd);
	return internal;
}

const char *AVService::fromUTF8(const char *encoding, const char *src)
{
	char *out = NULL;
	size_t inbytesleft = strlen(src) + 1;
	size_t outbytesleft = 1024;
	iconv_t cd;
	memset(internal, 0, 1024);
	out = internal;
	cd = iconv_open(encoding, "UTF-8");
	iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	iconv_close(cd);
	return internal;
}

const char *AVService::UTF16ToUTF8(unsigned short *s, int l)
{
	    char *b = internal;
	    for (int i=0; i<l; i++)
	    {
		    if (s[i] < 0x80)
		    {
			    *b++=s[i];
		    }
		    else if (s[i] < 0x800)
		    {
			    *b++=(0xc0 | s[i]>>6);
			    *b++=(0x80 | s[i] & 0x3f);
		    }
		    else if (s[i] < 0xd800 || s[i] > 0xdbff)
		    {
			    *b++ = (0xe0 | s[i]>>12);
			    *b++ = (0x80 | s[i]>>6 & 0x3f);
			    *b++ = (0x80 | s[i] & 0x3f);

		    }
		    else
		    {
			    unsigned int offset= 0x10000 - (0xd800 << 10) - 0xdc00;
			    unsigned int codepoint=(s[i] << 10) + s[i+1]+offset;
			    i++;
			    *b++=(0xf0 | codepoint>>18);
			    *b++=(0x80 | codepoint>>12 & 0x3f);
			    *b++=(0x80 | codepoint>>6 & 0x3f);
			    *b++=(0x80 | codepoint & 0x3F);
		    }
	    }

	    *b=0;
	    return internal;
}
