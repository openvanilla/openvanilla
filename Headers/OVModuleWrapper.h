// OVModule.h

#ifndef __OVModule_h
#define __OVModule_h

#include "OpenVanilla.h"

/*
extern "C"
{
    // required
    int OVModuleVersion();
    
    int OVModuleInitialize(const char *modulePath, const char *userPath,
        const char *seperator);
        
    int OVModuleAvailableInputMethodCount();
    OVInputMethod* OVModuleNewInputMethod(int imidx);
    
    int OVModuleAvailableBufferFilterCount();
    OVBufferFilter* OVModuleNewBufferFilter(int bfidx);
} */


#define OVModuleObjectiveC  \
    id _autoreleasepool;    \
    extern "C" void _init() { \
	    NSApplicationLoad();	\
        _autoreleasepool=[[NSAutoreleasePool alloc] init]; \
    }
    
#endif
