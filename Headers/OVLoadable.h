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


// there two are optional
extern "C" int OVLoadableAvailableIMCount(char *path);
extern "C" int OVLoadableCanUnload();

typedef OVInputMethod* OVLNewType(int);
typedef void OVLDeleteType(OVInputMethod*);
typedef unsigned int OVLVersionType();
typedef int OVLAvailableType(char*);
typedef int OVLUnloadType();


// We also have an OVLOADABLEWRAPPER ("OpenVanilla loadble IM wrapper").
// If you only have one IM module, e.g. class MyInputMethod, you can simply
// use this wrapper:
//  OVLOADABLEWRAPPER(MyInputMethod)         // no ending ; needed

#define OVLOADABLEWRAPPER(classname)     \
    extern "C" int OVLoadableAvailableIMCount(char*)    \
    {   \
        return 1;   \
    }   \
    extern "C" unsigned int OVLoadableVersion()  \
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

// use this wrapper if your IM will be statically linked into the Loader
#define OVSTATICWRAPPER(classname)	\
	OVInputMethod* OVStatcLinkedIM_##classname()	\
	{	\
		return new classname;	\
	}
    
// use this wrapper if your IM module is unloadable
// if you use Objective-C(++) to write your IM moudle, you should NEVER
// call use this wrapper
#define OVLOADABLECANUNLOAD   \
    extern "C" int OVLoadableCanUnload()    \
    {   \
        return 1;   \
    }

// if you use Objective-C(++) to write an IM moudle,
// you MUST use this wrapper
#define OVLOADABLEOBJCWRAPPER   \
    id autoreleasepool;    \
    extern "C" void _init() \
    {   \
	    NSApplicationLoad();	\
        autoreleasepool=[[NSAutoreleasePool alloc] init]; \
    }   \
    extern "C" int OVLoadableCanUnload()    \
    {   \
        return 0;   \
    }
    
#endif
