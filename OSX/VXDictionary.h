// VXDictionary.h

#ifndef __VXDictionary_h
#define __VXDictionary_h

#include "OpenVanilla.h"
#include "VXUtility.h"

class VXDictionary : public OVDictionary
{
public:
    VXDictionary(CFDictionaryRef ref=NULL)
    {
        if (!ref) dict=CFDictionaryCreateMutable(kCFAllocatorDefault,
            0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        else dict=CFDictionaryCreateMutableCopy(kCFAllocatorDefault,
            0, ref);
    }
    
    VXDictionary(CFDataRef ref)
    {
    }
    
    virtual ~VXDictionary()
    {
        if (dict) CFRelease(dict);
    }

    virtual int keyExist(void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
    {
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return 0;
        int r=CFDictionaryGetValueIfPresent(dict, keyref, NULL);
        CFRelease(keyref);
        return r;
    }
        
    virtual int getInt(void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
    {
        int r=0;
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return 0;
		CFNumberRef n;
        if (!(n=(CFNumberRef)CFDictionaryGetValue(dict, keyref))) return -1;
        CFNumberGetValue(n, kCFNumberIntType, &r);
        CFRelease(keyref);
        return r; 
    }
    
    virtual int putInt(void *key, int value, OVEncoding e=ovEncodingUTF8,
        int keylen=0)
    {
		printf ("setting value=%d\n", value);
		
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        CFNumberRef numref=CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType,
            &value);
        if (!keyref || !numref) return 0;    
        CFDictionarySetValue(dict, keyref, numref);
        CFRelease(numref);
        CFRelease(keyref);
        return 1;
    }
            	
	virtual int getString(void *key, void *str, OVEncoding e=ovEncodingUTF8,
		int keylen=0, int maxlen=0)
    {
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return 0;
        CFStringRef valueref;
        if (!(valueref=(CFStringRef)CFDictionaryGetValue(dict, keyref))) return 0;
        int r=VXConvertCFString(valueref, str, e, maxlen);
        CFRelease(keyref);
        return r;
    }
    
	virtual int putString(void *key, void *value, OVEncoding e=ovEncodingUTF8,
		int keylen=0, int valuelen=0) 
    { 
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        CFStringRef valueref=VXCreateCFString(value, e, valuelen);
        if (!keyref || !valueref) return 0;    
        CFDictionarySetValue(dict, keyref, valueref);
        CFRelease(keyref);
        CFRelease(valueref);
        return 1;
    }

    virtual CFDictionaryRef getdictref() { return dict; }
    
    virtual CFDictionaryRef getDictionary(void *key, OVEncoding e=ovEncodingUTF8,
        int keylen=0)
    {
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return 0;
        CFDictionaryRef r=(CFDictionaryRef)CFDictionaryGetValue(dict, keyref);
        CFRelease(keyref);
        return ;

    }
    
	virtual int putDictionary(void *key, CFDictionaryRef dref, 
        OVEncoding e=ovEncodingUTF8, int keylen=0, int valuelen=0) 
    {
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return 0;
        CFDictionarySetValue(dict, keyref, dref);
        CFRelease(keyref);
        return 1;
    }

    virtual CFDataRef createxml()
    {
        return CFPropertyListCreateXMLData(kCFAllocatorDefault, dict);
    }

    


protected:
    CFMutableDictionaryRef dict;
};


#endif