// VXUtility.cpp

#include "VXUtility.h"

const int VXDefMaxLen=1024;
CFStringEncoding VXMapEncoding(OVEncoding e);
CFStringRef VXCreateCFStringNone8BitEncoding(void *s, 
    OVEncoding e, int l);
int VXConvertCFStringNone8BitEncoding(CFStringRef ref, void *s,
	OVEncoding e, int l);

CFStringEncoding VXMapEncoding(OVEncoding e)
{
   CFStringEncoding enc=kCFStringEncodingUTF8;

    // map OpenVanilla encoding to Carbon encoding    
    switch (e)
    {
        case ovEncodingBig5HKSCS:
            enc=kCFStringEncodingBig5_HKSCS_1999;
    } 
}

CFStringRef VXCreateCFString(void *s, OVEncoding e, int l)
{
    if (e & ovEncodingNone8BitEncodingMask)
        return VXCreateCFStringNone8BitEncoding(s, e, l);
    
    return CFStringCreateWithCString(NULL, (const char*)s, VXMapEncoding(e));
}


#warning Architecture-dependent code (currently only for big-endian machines)
CFStringRef VXCreateCFStringNone8BitEncoding(void *s, OVEncoding e, int l)
{    
    switch (e)
    {
        case ovEncodingUTF16BE:
            return CFStringCreateWithCharacters(NULL, (const UniChar*)s, l);
    }
    
    return NULL;
}

int VXConvertCFString(CFStringRef ref, void *s, OVEncoding e, int maxlen)
{
    if (e & ovEncodingNone8BitEncodingMask)
        return VXConvertCFStringNone8BitEncoding(ref, s, e, maxlen);

    int l=maxlen ? maxlen : VXDefMaxLen;
    if (!CFStringGetCString(ref, (char*)s, l, VXMapEncoding(e))) return 0;
    return strlen((const char*)s);	
}

int VXConvertCFStringNone8BitEncoding(CFStringRef ref, void *s, OVEncoding e, 
    int maxlen)
{
    CFRange r;
    int strl=CFStringGetLength(ref);
    int l=maxlen ? maxlen : VXDefMaxLen;
    switch (e)
    {
        case ovEncodingUTF16BE:
            r.location=0;
            r.length=(l > strl) ? strl : l;
            CFStringGetCharacters(ref, r, (UniChar*)s);
            return r.length;
    }
    
    return 0;
}



