// OVModuleLoader.cpp
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

#define OVDEBUG
#include <OpenVanilla/OVModuleLoader.h>
#include <OpenVanilla/OVUtility.h>

// let's tell first if this is compiled on Windows platform
#if defined(_Windows) || defined(__WINDOWS__) || defined(__WIN32__) || \
    defined(WIN32) || defined(__WINNT__) || defined(__NT__)
    #ifndef WINDOWS
        #define WINDOWS
    #endif
#endif

#include <stdlib.h>

#ifndef WINDOWS     // use un*x dlopen
    #include <dlfcn.h>
#else   // use Win32 API
    #include <windows.h>
#endif

OVLoadableModuleFile* OVLoadModuleFile(const char *libfile,
    OVDictionary *globalPref, OVService *srv, const char *modulePath, 
    const char *userPath, const char *seperator)
{
    void *libhndl=dlopen(libfile, RTLD_LAZY);
    if (!libhndl) return NULL;      // failed for whatever reasion
    
    _OVVersionType* modver=(_OVVersionType*)dlsym(libhndl, "OVModuleVersion");
    if (!modver) return NULL;       // no OVModuleVersion(), fails
    if (modver() < ovVersion) return NULL;  // version too old, fails
    
    _OVInitType* init=(_OVInitType*)dlsym(libhndl, "OVMoudleInitialize");
    if (init)
    {
        // if OVModuleInitialize() fails, fails
        if (!init(globalPref, srv, modulePath, userPath, seperator)) return NULL;
    }
    
    int imcount=0, ofcount=0;
    _OVGetCountType* cntr;
    if (cntr=(_OVGetCountType*)dlsym(libhndl, "OVModuleAvailableInputMethodCount"))
        imcount=cntr();
    if (cntr=(_OVGetCountType*)dlsym(libhndl, "OVModuleAvailableOutputFilterCount"))
        ofcount=cntr();
    
    // must have at least one moudle of any kind (InputMethod or OutputFilter)
    if (!(imcount || ofcount)) return NULL; 
    
    _OVNewIMType *nim;
    _OVNewOFType *nof;
    nim=(_OVNewIMType*)dlsym(libhndl, "OVModuleNewInputMethod");
    nof=(_OVNewOFType*)dlsym(libhndl, "OVModuleNewOutputFilter");
    
    // if neither of the module-creation functions exist, fails
    if (!nim && !nof) return NULL;
    
    // module successfully loaded, create the object
    OVLoadableModuleFile *mfile=new OVLoadableModuleFile;
    if (!mfile) return NULL;    // memory full (though highly unlikely)
    
    mfile->libhndl=libhndl;
    mfile->imcount=imcount;
    mfile->ofcount=ofcount;
    mfile->newim=nim;
    mfile->newof=nof;
    return mfile;
}

class Dump : public OVDictionary
{
public:
    virtual int keyExist(const char*) { return 0; }
    virtual int getInteger(const char*) { return 0; }
    virtual int setInteger(const char*, int) { return 0; }
    virtual const char* getString(const char*) { return ""; }
    virtual const char* setString(const char*, const char*) { return ""; }
};

class Srv : public OVService
{
public:
    virtual void beep() {}
};

#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2) return 1;
    
    
    OVLoadableModuleFile *f=OVLoadModuleFile(argv[1],
        new Dump, new Srv, "", "", "");
        
    if (f)
    {
        murmur("loaded");
        OVInputMethod *im=f->newInputMethod(0);
        if (im)
        {
            murmur("im!");
            printf ("im identifier=%s\n", im->identifier());
        }
    }
    
    return 0;
}