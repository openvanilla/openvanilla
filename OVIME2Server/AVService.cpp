#include "AVService.h"
#include "AVDisplayServer.h"

#ifndef WIN32
#include "iconv.h"
#endif

#include <cstring>

#ifdef WIN32
#include <cstdlib>
#include <windows.h>

// Convert encoding name to Wnidows code page
static UINT EncodingNameToCodePage(const char* name)
{
	if( !name || !name[0] )
		return CP_ACP;

	UINT cp = 0;
	char upper[40];
	int i;
	// Convert to upper case
	for( i = 0; i < (sizeof(upper) - 1); ++i ) {
		if( ! name[i] )
			break;
		upper[i] = toupper( name[i] );
	}
	upper[i] = 0;

    if ( 0 == strcmp( upper, "BIG5") ) {
        cp = 950;
    }
    else if ( 0 == strcmp( upper, "GB2312") ) {
        cp = 936;
    }
    else if ( 0 == strcmp( upper, "SJIS") ||
                0 == strcmp( upper, "SHIFT_JIS") ||
                0 == strcmp( upper, "SHIFT-JIS") )
	{
        cp = 932;
    }
    else if ( 0 == strcmp( upper, "EUC-JP") ||
                0 == strcmp( upper, "EUC_JP") )
	{
        cp = 51932;
    }
    else if ( 0 == strcmp( upper, "EUC-KR") ||
                0 == strcmp( upper, "EUC_KR") )
    {
        cp = 51949;
    }
    else if ( 0 == strcmp( upper, "UTF-8") ) {
        cp = CP_UTF8;
    }
/*
	// I don't think we need Russian support.
    else if ( 0 == strcmp( upper, "KOI8-R") ||
                0 == strcmp( upper, "KOI8-RU") )
    {
        cp = 20866;
    }
	// I don't think we need Ukrainian support.
    else if ( 0 == strcmp( upper, "KOI8-U") )
    {
        cp = KOI8-U;
    }
*/
/*
    else if ( 0 == strncmp( upper, "ISO", 3 ) )	// iso-xxx
    {
        // the dash is optional (or, to be exact, it is not, but
        // several brokenmails "forget" it)
        const char* p = upper + 3;
		if ( *p == '-' )
			++p;
        unsigned int value;
        if ( sscanf(p, "8859-%u", &value) == 1 )
        {
			// Currently this is not supported
        }
    }
	*/
	return cp;
}

#endif


const char *AVService::pathSeparator()
{
#ifdef WIN32
	return "\\"; 
#else
	return "/";
#endif
}

void AVService::notify(const char *msg)
{
	dsvr->showNotify(msg);
}

const char *AVService::toUTF8(const char *encoding, const char *src)
{
	memset(internal, 0, 1024);
#ifdef WIN32
	UINT cp = EncodingNameToCodePage(encoding);
	if( cp == CP_UTF8 ) {
		strcpy( internal, src );
	}
	else {
		int src_len = strlen(src) + 1;
		int len = ::MultiByteToWideChar( cp, 0, src, src_len, NULL, 0 );
		wchar_t *wbuf = new wchar_t[len];
		len = ::MultiByteToWideChar( cp, 0, src, src_len, wbuf, len );
		::WideCharToMultiByte( CP_UTF8, 0, wbuf, len, internal, 1024, "?", NULL );
		delete []wbuf;
	}
#else	char *out = NULL;
	size_t inbytesleft = strlen(src) + 1;
	size_t outbytesleft = 1024;
	iconv_t cd;
	out = internal;
	cd = iconv_open("UTF-8", encoding);
	iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	iconv_close(cd);
#endif
	return internal;
}

const char *AVService::fromUTF8(const char *encoding, const char *src)
{
	memset(internal, 0, 1024);
#ifdef WIN32
	UINT cp = EncodingNameToCodePage(encoding);
	if( cp == CP_UTF8 ) {
		strcpy( internal, src );
	}
	else {
		int src_len = strlen(src) + 1;
		int len = ::MultiByteToWideChar( CP_UTF8, 0, src, src_len, NULL, 0 );
		wchar_t *wbuf = new wchar_t[len];
		len = ::MultiByteToWideChar( CP_UTF8, 0, src, src_len, wbuf, len );
		::WideCharToMultiByte( cp, 0, wbuf, len, internal, 1024, "?", NULL );
		delete []wbuf;
	}
#else
	char *out = NULL;
	size_t inbytesleft = strlen(src) + 1;
	size_t outbytesleft = 1024;
	iconv_t cd;
	out = internal;
	cd = iconv_open(encoding, "UTF-8");
	iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	iconv_close(cd);
#endif
	return internal;
}

const char *AVService::UTF16ToUTF8(unsigned short *s, int l)
{
	    char *b = internal;
	    for (int i=0; i<l; i++)
	    {
		    if (s[i] < 0x80)
		    {
			    *b++=static_cast<char>(s[i]);
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

enum { bit7=0x80, bit6=0x40, bit5=0x20, bit4=0x10, bit3=8, bit2=4, bit1=2, bit0=1 };

int AVService::UTF8ToUTF16(const char *src, unsigned short **rcvr)
{
	char *s1=(char*)src;
	int len=0;
	char a, b, c;
	while (*s1)
	{
		a=*s1++;
		if ((a & (bit7|bit6|bit5))==(bit7|bit6)) { // 0x000080-0x0007ff
			b=*s1++;

			u_internal[len] = ((a & (bit4|bit3|bit2)) >> 2) * 0x100;
			u_internal[len] += (((a & (bit1|bit0)) << 6) | (b & (bit5|bit4|bit3|bit2|bit1|bit0)));
		}
		else if ((a & (bit7|bit6|bit5|bit4))==(bit7|bit6|bit5)) // 0x000800-0x00ffff
		{
			b=*s1++;
			c=*s1++;

			u_internal[len] = (((a & (bit3|bit2|bit1|bit0)) << 4) | ((b & (bit5|bit4|bit3|bit2)) >> 2)) * 0x100;
			u_internal[len] += (((b & (bit1|bit0)) << 6) | (c & (bit5|bit4|bit3|bit2|bit1|bit0)));
		}
		else 
		{
			u_internal[len] =(0);
			u_internal[len] +=(a);
		}
		len++;
	}
	*rcvr = u_internal;
	return len;
}
