// VXUtility.cpp

#include "VXUtility.h"

const int VXDefMaxLen=1024;
CFStringEncoding VXMapEncoding(OVEncoding e);
CFStringRef VXCreateCFStringNon8BitEncoding(const void *s, OVEncoding e, int l);
int VXConvertCFStringNon8BitEncoding(CFStringRef ref, void *s,
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
    
    return enc;
}

CFStringRef VXCreateCFString(const void *s, OVEncoding e, int l)
{
    if (e & ovEncodingNon8BitEncodingMask)
        return VXCreateCFStringNon8BitEncoding(s, e, l);
    
    return CFStringCreateWithCString(NULL, (const char*)s, VXMapEncoding(e));
}


#warning Architecture-dependent code (currently only for big-endian machines)
CFStringRef VXCreateCFStringNon8BitEncoding(const void *s, OVEncoding e, int l)
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
    if (e & ovEncodingNon8BitEncodingMask)
        return VXConvertCFStringNon8BitEncoding(ref, s, e, maxlen);

    int l=maxlen ? maxlen : VXDefMaxLen;
    if (!CFStringGetCString(ref, (char*)s, l, VXMapEncoding(e))) return 0;
    return strlen((const char*)s);	
}

int VXConvertCFStringNon8BitEncoding(CFStringRef ref, void *s, OVEncoding e, 
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

CFURLRef VXCreateURL(char *localfilename)
{
    CFMutableStringRef str=CFStringCreateMutable(NULL, 0);
    if (!str) return NULL;
    
    CFStringRef filename=VXCreateCFString(localfilename);
    if (!filename) return NULL;
    
    CFStringAppendCString(str, "file://", kCFStringEncodingUTF8);
    CFStringAppend(str, filename);
    
    CFURLRef url=CFURLCreateWithString(NULL, str, NULL);
    
    CFRelease(filename);
    CFRelease(str);
    
    return url;
}

int VXGetCurrentLocale(CFBundleRef bundle, char *str, int maxlen)
{
	char *defvalue="en";
	strcpy(str, defvalue);
	
	if (!bundle) return strlen(defvalue);

	CFArrayRef ar=CFBundleCopyBundleLocalizations(bundle);
	if (!ar) return strlen(defvalue);
	
	CFArrayRef pref=CFBundleCopyPreferredLocalizationsFromArray(ar);
	if (!pref) 
	{
		CFRelease(ar);
		return strlen(defvalue);
	}
    
	if (CFArrayGetCount(pref))
	{
		CFStringRef r=CFArrayGetValueAtIndex(pref, 0);
		if (r)
		{
			CFStringRef cr=CFLocaleCreateCanonicalLocaleIdentifierFromString
				(NULL, r);			
			if (cr)
			{
				if (!CFStringGetCString(cr, str, maxlen, kCFStringEncodingUTF8))
				{
					strcpy(str, defvalue);
				}
				
				CFRelease(cr);
			}
		}
		CFRelease(pref);
		CFRelease(ar);
	}
	
	return strlen(str);
}
