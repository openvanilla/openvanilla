// OVLoadable.h :
// BSD-style loadable library (.so/.dylib) interface for platforms 
// that support dlopen() functionalities

// Copyright (c) 2004 by Lukhnos D. Liu (relocating e-mail address)

#ifndef __OVLoadable_h
#define __OVLoadable_h

#include "OpenVanilla.h"

// declares the required inteface functions, must be extern "C"
//      int OVLoadableAvailableIMCount(char *path)
//          receives the path base where the module is loaded
//          (e.g. if the module is /Library/OpenVanilla/foobar.dylib,
//          the path will be "/Library/OpenVanilla");
//          returns available input method modules (must be at least 1)
//      OVLoadableNewIM() and OVLoadableDeleteIM() creates and destroys
//      an instance of IM module. The parameter "imid" corresponds to the
//      available IM's that OVLoadableAvailableIMCount() returned.

// these three are required
extern "C" OVInputMethod* OVLoadableNewIM(int imid);
extern "C" void OVLoadbleDeleteIM(OVInputMethod *im);
extern "C" unsigned int OVLoadableVersion();

typedef OVInputMethod* OVLNewType(int);
typedef void OVLDeleteType(OVInputMethod*);

// there two are optional
extern "C" int OVLoadableAvailableIMCount(char *path);
extern "C" int OVLoadableCanUnload();

// We also have an OVLOADABLEWRAPPER ("OpenVanilla loadble IM wrapper").
// If you only have one IM module, e.g. class MyInputMethod, you can simply
// use this wrapper:
//  OVLOADABLEWRAPPER(MyInputMethod)         // no ending ; needed

#define OVLOADABLEWRAPPER(classname)     \
    extern "C" int OVLoadableAvailableIMCount(char*)    \
    {   \
        return 1;   \
    }   \
    extern "C" int OVLoadableCanUnload()    \
    {   \
        return 0;   \
    }   \
    extern "C" int OVLoadableVersion()  \
    {   \
        return ovVersion;   \
    }   \
    extern "C" OVInputMethod* OVLoadableNewIM(int) \
    {   \
        return new classname;   \
    }   \
    extern "C" void OVLoadableDeleteIM(OVInputMethod *im)  \
    {   \
        delete im;  \
    }
    
// if you use Objective-C/Objective-C++ to write an IM moudle,
// you MUST call this wrapper

#define OVLOADABLEOBJCWRAPPER   \
    id _autoreleasepool;    \
    extern "C" void _init() \
    {   \
        _autoreleasepool=[[NSAutoreleasePool] alloc] init]; \
    }
    
    
#endif
