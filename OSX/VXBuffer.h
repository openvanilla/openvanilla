// VXBuffer.h

#ifndef __VXBuffer_h
#define __VXBuffer_h

#include <Carbon/Carbon.h>
#include "OpenVanilla.h"
#include "VXUtility.h"

class VXBuffer : public OVBuffer
{
public:
    VXBuffer() : cimbuf(NULL) {}
    
    virtual OVBuffer* clear()
        { if (cimbuf) cimbuf->clear(); return this; }
    virtual OVBuffer* send(OVLanguage lang=ovLangAll)
    { 
        if (cimbuf) cimbuf->update(TRUE, -1, -1, -1, lookupscript(lang), 
            lookuplang(lang)); 
        return this; 
    }
    virtual OVBuffer* updated(int cursorpos=-1, int hilitefrom=-1, 
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
    
    ScriptCode lookupscript(OVLanguage lang)
    {
        switch (lang)
        {
            case ovLangTradChinese: return smTradChinese;
            case ovLangSimpChinese: return smSimpChinese;
            case ovLangJapanese: return smJapanese;
            case ovLangKorean: return smKorean;
        }
        return smUnicodeScript;
    }
    
    LangCode lookuplang(OVLanguage lang)
    {
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