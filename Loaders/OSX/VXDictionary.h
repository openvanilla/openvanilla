// VXDictionary.h

#ifndef __VXDictionary_h
#define __VXDictionary_h

#include <OpenVanilla/OpenVanilla.h>
#include "VXUtility.h"

class VXDictionary : public OVDictionary
{
public:
    VXDictionary(CFDictionaryRef ref=NULL, int cpy=0)
    {
        if (!ref) dict=CFDictionaryCreateMutable(NULL, 0, 
            &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        else
        {
            if (cpy)                
                dict=CFDictionaryCreateMutableCopy(NULL, 0, ref);
            else
            {   
                dict=(CFMutableDictionaryRef)ref;
                CFRetain(dict);
            }
        }
    }
    
    virtual ~VXDictionary()
    {
        if (dict) CFRelease(dict);
    }

    virtual int keyExist(const void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
    {
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return 0;
        int r=CFDictionaryGetValueIfPresent(dict, keyref, NULL);
        CFRelease(keyref);
        return r;
    }
        
    virtual int getInt(const void *key, OVEncoding e=ovEncodingUTF8, int keylen=0)
    {
        int r=0;
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return 0;
		CFTypeRef n;
        if (!(n=CFDictionaryGetValue(dict, keyref))) return 0;
        if (CFGetTypeID(n) != CFNumberGetTypeID())
        {
            if (CFGetTypeID(n) != CFStringGetTypeID()) return 0;
            r=CFStringGetIntValue((CFStringRef)n);
        }
        else
        {
            CFNumberGetValue((CFNumberRef)n, kCFNumberIntType, &r);
        }
        
        CFRelease(keyref);
        return r; 
    }
    
    virtual int setInt(const void *key, int value, OVEncoding e=ovEncodingUTF8,
        int keylen=0)
    {
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        CFNumberRef numref=CFNumberCreate(NULL, kCFNumberIntType,
            &value);
        if (!keyref || !numref) return 0;    
        CFDictionarySetValue(dict, keyref, numref);
        CFRelease(numref);
        CFRelease(keyref);
        return 1;
    }
            	
	virtual int getString(const void *key, void *str, OVEncoding e=ovEncodingUTF8,
		int keylen=0, int maxlen=0)
    {
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return 0;
        CFTypeRef valueref;
        if (!(valueref=CFDictionaryGetValue(dict, keyref))) return 0;
        if (CFGetTypeID(valueref) != CFStringGetTypeID())
        {
            if (CFGetTypeID(valueref) != CFNumberGetTypeID()) return 0;
            char buf[256];
            sprintf (buf, "%d", getInt(key, e, keylen));
            CFStringRef sr=VXCreateCFString(buf);                        
            int l=VXConvertCFString(sr, str, e, maxlen);
            CFRelease(sr);
            return l;
        }
        
        int r=VXConvertCFString((CFStringRef)valueref, str, e, maxlen);
        CFRelease(keyref);
        return r;
    }
    
	virtual int setString(const void *key, const void *value, OVEncoding
	    e=ovEncodingUTF8, int keylen=0, int valuelen=0) 
    { 
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        CFStringRef valueref=VXCreateCFString(value, e, valuelen);
        if (!keyref || !valueref) return 0;    
        CFDictionarySetValue(dict, keyref, valueref);
        CFRelease(keyref);
        CFRelease(valueref);
        return 1;
    }

    virtual int newDictionary(const void *key, OVEncoding e=ovEncodingUTF8, 
        int keylen=0)
    {
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return 0;        
        VXDictionary newdict;        
        CFDictionarySetValue(dict, keyref, newdict.getDictRef());
        CFRelease(keyref);
        return 1;
    }

    virtual OVDictionary* getDictionary(const void *key, OVEncoding 
        e=ovEncodingUTF8, int keylen=0)
    {
        CFStringRef keyref=VXCreateCFString(key, e, keylen);
        if (!keyref) return NULL;
        CFTypeRef r=CFDictionaryGetValue(dict, keyref);
        if (!r) return NULL;
        if (CFGetTypeID(r) != CFDictionaryGetTypeID()) return NULL;
        CFRelease(keyref);
        VXDictionary *dict=new VXDictionary((CFDictionaryRef)r);
        return dict;
    }
    
    virtual CFDictionaryRef getDictRef() { return dict; }

    virtual CFDataRef createXML()
    {
        return CFPropertyListCreateXMLData(NULL, dict);
    }

    virtual int writeToURL(CFURLRef url)
    {
        Boolean status;
        SInt32 errcode;
        
        CFDataRef data=createXML();
        if (!data) return 0;
        
        status=CFURLWriteDataAndPropertiesToResource(url, data, NULL, &errcode);
        CFRelease(data);
        return status;
    }

    virtual int readFromURL(CFURLRef url)
    {
        SInt32 errcode;
        CFDataRef data;
        Boolean status=CFURLCreateDataAndPropertiesFromResource(NULL,
            url, &data, NULL, NULL, &errcode);
        if (!status) return 0;
        
        CFPropertyListRef p=CFPropertyListCreateFromXMLData
            (NULL, data,  kCFPropertyListMutableContainersAndLeaves, NULL);
        CFRelease(data);
        
        if (!p) return 0;
        if (CFGetTypeID(p) != CFDictionaryGetTypeID()) return 0;
        
        if (dict) CFRelease(dict);
        dict=(CFMutableDictionaryRef)p;
        return 1;
    }

protected:
    CFMutableDictionaryRef dict;
};


#endif