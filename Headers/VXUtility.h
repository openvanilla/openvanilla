// VXUtility.h

#ifndef __VXUtility_h
#define __VXUtility_h

#include <Carbon/Carbon.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>

CFStringRef VXCreateCFString(const void *s, OVEncoding e=ovEncodingUTF8, int l=0);
int VXConvertCFString(CFStringRef ref, void *s, OVEncoding e=ovEncodingUTF8, 
    int maxlen=0);

CFURLRef VXCreateURL(char *localfilename);
int VXGetCurrentLocale(CFBundleRef bundle, char *str, int maxlen=256);

const int vxMaxPoolSize=256;

class VXCFAutoreleasePool
{
public:
    VXCFAutoreleasePool() : p(0) {}
    ~VXCFAutoreleasePool() { for (int i=0; i<p; i++) CFRelease(pool[i]); }
    CFTypeRef add(CFTypeRef r)
    {
		if (!r) return r;
        if (p==vxMaxPoolSize) return r;     // note: no exception
        return pool[p++]=r;
    }        
    
protected:
    CFTypeRef pool[vxMaxPoolSize];
    int p;
};


#endif
