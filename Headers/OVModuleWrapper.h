// OVModuleWrapper.h
// This header file does nothing. It only provides a few macros so that
// you don't have to implement every "module outlet" function.

#ifndef __OVModuleWrapper_h
#define __OVModuleWrapper_h

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
