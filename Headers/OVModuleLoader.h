// OVModuleLoader.h: Loader interface for loadable module files
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

// Because some platform (e.g. Windows) does not support the unloading of
// dynamically linked library, unload and reload are not taken into account
// in the design of this interface. In fact, closing and reloading an application
// (and henceforth the reloading of DLLs/shared libraries) are fast and memory-
// safe in modern operating system, so we don't really have to get into 
// trouble ourselves.

#ifndef __OVModuleLoader_h
#define __OVModuleLoader_h

#include "OpenVanilla.h"

class OVLoadableModuleFile;

OVLoadableModuleFile* OVLoadModuleFile(OVDictionary *globalPref, OVService *srv,
    const char *modulePath, const char *userPath, const char *seperator);

typedef OVInputMethod* _OVNewIMType(int idx);
typedef OVOutputFilter* _OVNewOFType(int idx);
    
class OVLoadableModuleFile : public OVBase
{
public:
    int availableInputMethodCount() { return imcount; }
    int availableOutputFilterCount() { return ofcount; }
    OVInputMethod* newInputMethod(int idx);
    OVOutputFilter* newOutputFilter(int idx);

protected:
    OVLoadableModuleFile() { imcount=ofcount=0; }
    
    void *libhndl;
    _OVNewIMType *newim;
    _OVNewOFType *newof;
    int imcount;
    int ofcount;
    
    friend OVLoadableModuleFile* OVLoadModuleFile(OVDictionary*, OVService*,
        const char*, const char*, const char*);
};

const int ovMLMaxItem=256;

template<class T> class OVModuleList : public OVBase
{
public:
    OVModuleList ()
    {
        counter=0;
        for (int i=0; i<ovMLMaxItem; i++)
        {
            initialized[i]=0;
            list[i]=0;
        }
    }
    
    ~OVModuleList ();
    int add(const T& x)     { list[counter++]=x; }
    int count();
    int find(const char *identifier);   // -1 if not found
    int remove(int idx);
    T get(int idx);
    void setInitialized(int idx);
    int isInitialized(int idx);
    
protected:
    int counter;
    int initialized[ovMLMaxItem];
    T list[ovMLMaxItem];    
};

class OVModuleManager : public OVBase
{
public:
    void addFile(OVLoadableModuleFile *f);
    void addInputMethod(OVInputMethod *im)
    {
        imlist.add(im);
    }
    
    void addOutputFilter(OVOutputFilter *of);
    
    OVModuleList<OVInputMethod*> imlist;
    OVModuleList<OVOutputFilter*> oflist;
};

#endif

