// VXUtility.h

#ifndef __VXUtility_h
#define __VXUtility_h

#include <Carbon/Carbon.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVUtility.h"

CFStringRef VXCreateCFString(void *s, OVEncoding e=ovEncodingUTF8, int l=0);
int VXConvertCFString(CFStringRef ref, void *s, OVEncoding e=ovEncodingUTF8, 
    int maxlen=0);

#endif
