// OVModuleWrapper: OpenVanilla loadable module wrapper macros
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

// At version 0.7, OpenVanilla has two kinds of "modules": input method (IM)
// and output filter (OF). OpenVanilla loosely defines a way to package IM/OF
// classes into dyamically loadble libraies (.so in most u*ix, .dylib in 
// Mac OS X/Darwin, .dll in Windows), which can then be loaded by an IM
// environment (Loader). This heavily relies on the dynamical linking facilities
// provided by the operating system. Luckily, the BSD-style dlopen() is widely
// available or can be easily simulated.

#ifndef __OVModuleWrapper_h
#define __OVModuleWrapper_h

// This section is remarked because it is not really necessary; any OV's
// loadable module file (hereafter the name in order to avoid the terminolgy
// confusion between DLL, shared library, loadable bundle, etc.) should
// at least export OVModuleVersion(). By design OV doesn't really care
// the content of the loadable module per se, it's the loaded IM/OF modules
// that count. Export other functions according to your own needs. 
// For an IM moudle, you should have an OVMoudleAvailableInputMethodCount
// that returns at least 1, telling the Loader that you have one IM class
// available. The Loader will call OVModuleNewInputMethod with an index number
// which begins at 0 (with the max index=available IM count-1).

/*
    extern "C"
    {
        int OVModuleVersion();    
        
        int OVModuleInitialize(OVDictionary *globalPref, OVService *srv,
            const char *modulePath, const char *userPath, const char *seperator);
            
        int OVModuleAvailableInputMethodCount();
        OVInputMethod* OVModuleNewInputMethod(int idx);    
        
        int OVModuleAvailableOutputFilterCount();
        OVOutputFilter* OVModuleNewOutputFilter(int idx);
    }
*/

#define OVMODULEWRAPPER_INPUTMETHOD(im) \
    extern "C" int OVModuleVersion() { return ovVersion; }  \
    extern "C" int OVModuleAvailableInputMethodCount() { return 1; } \
    extern "C" OVInputMethod* OVModuleNewInputMethod(int) { return new im; }

#endif
