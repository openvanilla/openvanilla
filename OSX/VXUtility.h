// VXUtility.h

#ifndef __VXUtility_h
#define __VXUtility_h

#include <Carbon/Carbon.h>
#include "OpenVanilla.h"

CFStringRef VXCreateCFString(void *s, OVEncoding e=ovEncodingUTF8, int l=0);

#endif
