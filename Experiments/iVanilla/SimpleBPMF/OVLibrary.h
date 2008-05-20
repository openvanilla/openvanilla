// OVLibrary.h: Interface header for dynamic loadable OV modules
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __OVLibrary_h
#define __OVLibrary_h

// as a dynamically-loadable library (be it .so, .DLL, .dylib or .bundle),
// we only need to export these three functions

// extern "C" unsigned int OVGetLibraryVersion();
// extern "C" int OVInitializeLibrary(OVService* srv, const char* libpath);
// extern "C" OVModule *OVGetModuleFromLibrary(int idx);

// wrapper macro for libraries that have exactly only one OVModule (IM/OF)
#define OV_SINGLE_MODULE_WRAPPER(classname) \
    extern "C" unsigned int OVGetLibraryVersion() { return OV_VERSION; } \
    extern "C" int OVInitializeLibrary(OVService*, const char*) { return 1; } \
    extern "C" OVModule *OVGetModuleFromLibrary(int idx) {\
        return (idx==0) ? new classname : NULL; \
    }

// below are used for most OV Loader implementations, not needed by any 
// loadable library

typedef unsigned int _OVGetLibraryVersion_t();
typedef int _OVInitializeLibrary_t(OVService *srv, const char *libpath);
typedef OVModule* _OVGetModuleFromLibrary_t(int);

struct OVLoadedLibrary {
    OVLoadedLibrary(_OVInitializeLibrary_t *i, _OVGetModuleFromLibrary_t *m) :
        initialize(i), getModule(m) {}
    _OVInitializeLibrary_t *initialize;
    _OVGetModuleFromLibrary_t *getModule;
};

#endif
