// OVEmuClasses.h

// #ifndef __OVEmuClasses_h
// #define __OVEmuClasses_h

#define OVDEBUG
#include "OpenVanilla.h"
#include "OVUtility.h"
#include <stdint.h>
#include <stdio.h>
#include "VXLoadableIM.h"

inline const char *EMMapEncoding(OVEncoding e)
{
	switch (e)
	{
		case ovEncodingBig5HKSCS: return "big5-hkscs";
		case ovEncodingEUC_CN: return "euc_cn";
	}
	return "utf-8";
}

inline const char *EMMapLanguage(OVLanguage l)
{
	switch (l)
	{
    	case ovLangTradChinese: return "ovLangTradChinese";
    	case ovLangSimpChinese: return "ovLangSimpChinese";
    	case ovLangJapanese: return "ovLangJapanese";
    	case ovLangKorean: return "ovLangKorean";
		case ovLangWestern: return "ovLangWestern";
	}
	return "ovLangAll";
}

inline int EMIsLittleEndian()
{
	short s=1;				// should use a platform 
	char *v=(char*)&s;
	return *v;
}

inline uint16_t EMUTF16ToUINT16(uint16_t utfchar, OVEncoding type)
{
	int machine=EMIsLittleEndian(), swap=0;
	union
	{
		uint16_t u;
		char c[2];
	} x;
	
	switch(type)
	{
		case ovEncodingUTF16Auto:	// same, no need to turn
			break;
		case ovEncodingUTF16LE:
			if (!machine) swap=1;	// big-endian machine encounters UTF16LE
			break;
		case ovEncodingUTF16BE:
			if (machine) swap=1;	// little-endian machine encounters UTF16BE		
	}
	
	x.u=utfchar;
	if (swap)
	{
		x.c[0] ^= x.c[1];
		x.c[1] ^= x.c[0];
		x.c[0] ^= x.c[1];
	}
	return x.u;
}

inline int EMUTF16ToUTF8(uint16_t u16, char *u8s)
{
	unsigned char high=u16 >> 8, low=u16 & 0xff;
	
	if (u16 < 0x80)
	{
		*u8s=(char)u16;
		return 1;
	}
	
	if (u16 < 0x800)
	{
		*u8s++=0xc0 | (high << 2) | ((low & 0xc0) >> 6);
		*u8s=0x80 | (low & 0x3f);
		return 2;
	}
	
	*u8s++=0xe0 | (high >> 4);
	*u8s++=0x80 | ((high & 0xf) << 2) | ((low & 0xc0) >> 6);
	*u8s++=0x80 | (low & 0x3f);
	return 3;
}

inline int EMSurrogateToUTF8(uint16_t lead, uint16_t trail, char *u8s)
{
	const uint32_t surrogate_offset=0x10000 - (0xd800 << 10)-0xdc00;
	uint32_t codepoint=((uint32_t)lead << 10) + (uint32_t)trail +
		surrogate_offset;
		
	printf ("utf32=%x\n", codepoint);
	*u8s++ = 0xf0 | (codepoint >> 18);
	*u8s++ = 0x80 | ((codepoint & 0x30000) >> 12) | ((codepoint & 0xf000) >> 12);
	*u8s++ = 0x80 | ((codepoint & 0xf00) >> 6) | ((codepoint & 0xc0) >> 6);
	*u8s = 0x80 | (codepoint & 0x3f);
	return 4;
}

inline int EMUTF16StringToUTF8(uint16_t* u16s, char *u8s, int len, OVEncoding type)
{
	int u8len=0;
	for (int i=0; i<len; i++) 	// no 0xfffe or 0xfeff processing
	{
		uint16_t u=EMUTF16ToUINT16(u16s[i], type);
		int l;
		if (u >= 0xd800 && u <= 0xdbff)
		{
			i++;
			l=EMSurrogateToUTF8(u, EMUTF16ToUINT16(u16s[i], type), u8s);	
		}
		else l=EMUTF16ToUTF8(u, u8s);
		
		u8len+=l;
		u8s+=l;
	}
	*u8s=0;
	return u8len;
}

/*
int main()
{
	char buf[256];
	
	uint16_t s1=65;
	uint16_t s2=0x3a3;
	uint16_t s3=0x9f14;
	uint16_t s4[2]={0xd855, 0xdde0};
	
	printf ("%d, %s\n", EMUTF16StringToUTF8(&s1, buf, 1, ovEncodingUTF16BE), buf);
	printf ("%d, %s\n", EMUTF16StringToUTF8(&s2, buf, 1, ovEncodingUTF16BE), buf);
	printf ("%d, %s\n", EMUTF16StringToUTF8(&s3, buf, 1, ovEncodingUTF16BE), buf);
	printf ("%d, %s\n", EMUTF16StringToUTF8(s4, buf, 2, ovEncodingUTF16BE), buf);
	
	for (int i=0; i<4; i++) printf ("%x ", (unsigned char)buf[i]);
	printf ("\n");
} */

const int emMaxStringLen=1024;

class EMString : public OVObject
{
public:
	EMString() { *str=0; }
	virtual EMString* clear() { *str=0; return this; }
	virtual EMString* append(void *s, OVEncoding e=ovEncodingUTF8, int len=0)
	{
		if (e & ovEncodingNon8BitEncodingMask)
		{
			char *p=str+strlen(str);
			EMUTF16StringToUTF8((uint16_t*)s, p, len, e);
			return this;
		}
		else strcat(str, (char*)s);
		return this;
	}
	
	char str[emMaxStringLen];
};
	
class EMTextBar : public OVTextBar
{
public:
	EMTextBar() : onscreen(0) {}
    virtual OVTextBar* clear() { str.clear(); return this; }
    virtual OVTextBar* append(void *s, OVEncoding e=ovEncodingUTF8, int len=0)
    {
    	str.append(s, e, len);
    	return this;
    }

    virtual OVTextBar* hide()
    {
 		murmur("OVEmu: textbar now hidden");
    	onscreen=0;
    	return this;
    }
    
    virtual OVTextBar* show() 
    {
    	murmur("OVEmu: textbar now showing");
    	return this;
    }
    virtual OVTextBar* update()
    {
    	murmur("OVEmu: textbar display \"%s\"", str.str);
    	return this;
    }
    	
    virtual int onScreen() { return onscreen; }
    
protected:
    EMString str;
    int onscreen;
};

class EMBuffer : public OVBuffer
{
public:
    virtual OVBuffer* clear()
    {
    	str.clear();
    	return this;
    }
    
    virtual OVBuffer* send(OVLanguage lang=ovLangAll)
    {
    	murmur ("OVEmu: buffer sent, lang=%s, data=\"%s\"", EMMapLanguage(lang),
    		str.str);
    	clear();
    	return this;
    }

    virtual OVBuffer* update(int cursorpos, int hilightfrom, 
        int hilightto, OVLanguage lang=ovLangAll)
    {
    	murmur ("OVEmu: buffer updated, curpos/hilite=(%d, %d-%d), lang=%s, data=\"%s\"", cursorpos, hilightfrom, hilightto, EMMapLanguage(lang), str.str);
    	return this;
    }
    virtual OVBuffer* append (void *s, OVEncoding e=ovEncodingUTF8, int len=0)
    {
    	str.append(s, e, len);
    	return this;
    }
	
    virtual int length() { return strlen(str.str); }
protected:
	EMString str;
};

const int emMaxDictPair=32;
const int emMaxDictStrLen=64;

// no subdictionary support!
class EMDictionary : public OVDictionary
{
public:
	EMDictionary()
	{
		index=0;
	}
	
	virtual int find(char *s)
	{
		for (int i=0; i<index; i++)
		{
			if (!strcmp(s, key[i])) return i;
		}
		return -1;
	} 
	
	virtual void dump()
	{
		for (int i=0; i<index; i++)
			murmur("(%s, %s)", key[i], value[i]);
	}
	
    virtual int keyExist(const void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
    {
    	if (find((char*)key)!=-1) return 1;
    	return 0;
    }
        
    virtual int getInt(const void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
    {
    	int i=find((char*)key);
    	if (i==-1) return -1;
    	return atoi(value[i]);
    }
    
    virtual int setInt(const void *k, int val, OVEncoding e=ovEncodingUTF8,
        int keylen=0)
    {
    	int i=find((char*)key);
    	if (i!=-1)
    	{
    		sprintf(value[i], "%d", val);
    		return 1;
    	}
    	if (index==emMaxDictPair) return 0;
    	strcpy(key[index], (const char*)k);
    	sprintf(value[index++], "%d", val);
    	return 1;
    }
            	
	virtual int getString(const void *key, void *str, OVEncoding e=ovEncodingUTF8,
		int keylen=0, int maxlen=0)
	{
	    int i=find((char*)key);
    	if (i==-1) return 0;
    	return strlen(strcpy((char*)str, value[i]));
	}
	
	virtual int setString(const void *k, const void *val, 
        OVEncoding e=ovEncodingUTF8, int keylen=0, int valuelen=0)
    {
    	int i=find((char*)key);
    	if (i!=-1)
    		return strlen(strcpy(value[i], (char*)val));
    	if (index==emMaxDictPair) return 0;
    	strcpy(key[index], (const char*)k);
    	return strlen(strcpy(value[index++], (char*)val));
    }
		
    virtual int newDictionary(const void *key, OVEncoding e=ovEncodingUTF8, 
        int keylen=0)
    {
    	throw ovExceptionNotSupported;
   	}
   	
    virtual OVDictionary* getDictionary(const void *key, 
        OVEncoding e=ovEncodingUTF8, int keylen=0)
    {
    	throw ovExceptionNotSupported;
    }
    
protected:
	int index;
	char key[emMaxDictPair][emMaxDictStrLen];
	char value[emMaxDictPair][emMaxDictStrLen];	
};


class EMService : public OVService
{
public:
    virtual void beep()
    {
    	murmur ("OVEmu: system beep");
    }
};

class EMKeyCode : public OVKeyCode
{
public:
	EMKeyCode() { clear(); }

	virtual void clear() { keycode=shift=capslock=ctrl=alt=command=0; }
	virtual int code() { return keycode; }
    virtual int isShift() { return shift; }
    virtual int isCapslock() { return capslock; }
    virtual int isCtrl() { return ctrl; }
    virtual int isAlt() { return alt; }
    virtual int isOpt() { return alt; }
    virtual int isCommand() { return command; }
    
	int keycode, shift, capslock, ctrl, alt, command;
};

// #endif  // of #ifndef __OVEmuClasses_h

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		murmur("usage: OVEmu imLibraryName");
		return 0;
	}
	
	VXLoadableLibrary lib;
	lib.load(argv[1], "./");		// for Windows here must be changed
	
	char buf[256];
	EMDictionary globalconfig, localconfig;
	EMService service;
	
	OVInputMethod *im=lib.imnew(0);
	im->identifier(buf);
	murmur ("OVEmu: loaded input method, identifier=%s", buf);
	im->initialize(&globalconfig, &localconfig, &service, (char*)"./");
	murmur ("OVEmu: dumping global config");
	globalconfig.dump();
	murmur ("OVEmu: dumping local config");
	localconfig.dump();
		
	OVIMContext *context=im->newContext();
	
	murmur ("\n\nOVEmu console ============");
	
	EMKeyCode key;
	EMTextBar textbar;
	EMBuffer buffer;
	while (!feof(stdin))
	{
		key.clear();
		char k=fgetc(stdin);
		switch(k)
		{
			case '!': key.keycode=27; break;
			case '@': key.keycode=13; break;
			default:
				if(isprint(k)) key.keycode=k; else continue;
		}
		
		context->keyEvent(&key, &buffer, &textbar, &service);
		murmur ("hit printable keys, '!'=escape, '@'=enter");
	}
	
	lib.imdelete(im);
	lib.unload();
}



