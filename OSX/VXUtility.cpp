// VXUtility.cpp

#include "VXUtility.h"

CFStringRef VXCreateCFStringNone8BitEncoding(void *s, 
    OVEncoding e, int l);

CFStringRef VXCreateCFString(void *s, OVEncoding e=ovEncodingUTF8, int l=0)
{
    if (e & ovEncodingNone8BitEncodingMask)
        return VXCreateCFStringNone8BitEncoding(s, e, l);
    
    CFStringEncoding enc=kCFStringEncodingUTF8;

    // map OpenVanilla encoding to Carbon encoding    
    switch (e)
    {
        case ovEncodingBig5HKSCS:
            enc=kCFStringEncodinBig5_HKSCS_1999;
    }
    
    return CFStringCreateWithCString(NULL, (const char*)s, enc);
}


#warning Architecture-dependent code (currently only for big-endian machines)
CFStringRef VXCreateCFStringNone8BitEncoding(void *s, 
    OVEncoding e, int l)
{
    
    switch (e)
    {
        case ovEncodingUTF16BE:
            return CFStringCreateWithCharacters(NULL, (const UniChar*)s, l);
    }
    
    return NULL;
}
