// VXBuffer.h

#ifndef __VXBuffer_h
#define __VXBuffer_h

#include <Carbon/Carbon.h>
#include <OpenVanilla/OpenVanilla.h>
#include "VXUtility.h"
#include "VXHanConvert.h"

class VXCIMBufferFilter : public CIMInputBuffer
{
public:
	void tradChineseToSimpChinese()
	{
		for (int i=0; i<len; i++)
		{
			unsigned short c=VXUCS2TradToSimpChinese(buffer[i]);
			if (c) buffer[i]=c;
		}
	}
	
	void simpChineseToTradChinese()
	{
		for (int i=0; i<len; i++)
		{
			unsigned short c=VXUCS2SimpToTradChinese(buffer[i]);
			if (c) buffer[i]=c;
		}
	}
};


class VXBuffer : public OVBuffer
{
public:
    VXBuffer() : cimbuf(NULL), conversionfilter(0) {}
    
	virtual void setConversionFilter(int f) { conversionfilter=f; }
	
    virtual OVBuffer* clear()
        { if (cimbuf) cimbuf->clear(); return this; }
    virtual OVBuffer* send(OVLanguage lang=ovLangAll)
    { 
        if (cimbuf) 
        {
			if (conversionfilter)
			{
				// we use a C++ hack here
				VXCIMBufferFilter *f=(VXCIMBufferFilter*)cimbuf;
				
				switch (conversionfilter)
				{
					case 1: f->tradChineseToSimpChinese(); break;
					case 2: f->simpChineseToTradChinese(); break;
				}
			}
			
            cimbuf->update(TRUE, -1, -1, -1, lookupscript(lang), 
                lookuplang(lang)); 
            cimbuf->clear();
        }
        return this; 
    }
    virtual OVBuffer* update(int cursorpos=-1, int hilitefrom=-1, 
        int hiliteto=-1, OVLanguage lang=ovLangAll)
    {
        if (cimbuf) cimbuf->update(FALSE, cursorpos, hilitefrom, hiliteto,
			lookupscript(lang), lookuplang(lang));
        return this;
    }
    virtual OVBuffer* append (void *s, OVEncoding e=ovEncodingUTF8, int l=0)
    {
        CFStringRef ref=VXCreateCFString(s, e, l);
        if (!ref) return this;
        if (cimbuf) cimbuf->put(ref);
        CFRelease(ref);
        return this;
    }
    
    virtual int length() { if (cimbuf) return cimbuf->length(); return 0; }

    virtual void bind(CIMInputBuffer *buf)
        { cimbuf=buf; }
    
protected:
    CIMInputBuffer *cimbuf;
    int conversionfilter;

    ScriptCode lookupscript(OVLanguage lang)
    {
		// script code hack
		if (lang & 0xf0000000)
		{
			int c=(lang & 0x0fff0000) >> 16;
			fprintf (stderr, "script code=%d\n", c);
			return c;
		}
		
        switch (lang)
        {
            case ovLangTradChinese: return smTradChinese;
            case ovLangSimpChinese: return smSimpChinese;
            case ovLangJapanese: return smJapanese;
            case ovLangKorean: return smKorean;
        }
        return smRoman;
    }
    
    LangCode lookuplang(OVLanguage lang)
    {
		// language code hack
		if (lang & 0xf0000000)
		{
			int c=lang & 0xffff;
			fprintf (stderr, "language code=%d\n", c);
			return c;
		}

        switch (lang)
        {
            case ovLangTradChinese: return langTradChinese;
            case ovLangSimpChinese: return langSimpChinese;
            case ovLangJapanese: return langJapanese;
            case ovLangKorean: return langKorean;
        }
        return langEnglish;
    }
};

#endif