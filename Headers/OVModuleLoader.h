// OVModuleLoader.h: Loader interface for loadable module files
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

// Because some platform (e.g. Windows) does not support the unloading of
// dynamically linked library, unload and reload are not taken into account
// in the design of this interface. In fact, closing and reloading an application
// (and henceforth the reloading of DLLs/shared libraries) are fast and memory-
// safe in almost every modern operating system, so we don't really have to get
// into the trouble ourselves.

#ifndef __OVModuleLoader_h
#define __OVModuleLoader_h

#include "OpenVanilla.h"

class OVLoadableModuleFile;

OVLoadableModuleFile* OVLoadModuleFile(const char *libfile,
    OVDictionary *globalPref, OVService *srv, const char *modulePath, 
    const char *userPath, const char *seperator);
    
typedef OVInputMethod* _OVNewIMType(int idx);
typedef OVOutputFilter* _OVNewOFType(int idx);
typedef unsigned int _OVVersionType();
typedef unsigned int _OVGetCountType();
typedef unsigned int _OVInitType(OVDictionary*, OVService*, const char*,
    const char*, const char*);
    
class OVLoadableModuleFile : public OVBase
{
public:
    int availableInputMethodCount() { return imcount; }
    int availableOutputFilterCount() { return ofcount; }
    OVInputMethod* newInputMethod(int i) { return (i<imcount) ? newim(i) : 0; }
    OVOutputFilter* newOutputFilter(int i) { return (i<imcount) ? newof(i) : 0; } 

protected:
    OVLoadableModuleFile() { imcount=ofcount=0; }
    
    void *libhndl;
    _OVNewIMType *newim;
    _OVNewOFType *newof;
    int imcount;
    int ofcount;

    friend OVLoadableModuleFile* OVLoadModuleFile(const char*,
        OVDictionary*, OVService*,const char*, const char*, const char*);
};

const int ovMSMax=64;

// this is a list WITHOUT ownership, i.e. we don't new/delete object herein
template<class T> class OVModuleList : public OVBase
{
public:
    OVModuleList() 
    {
        counter=0; 
        for (int i=0; i<ovMSMax; i++) { list[i]=0; inited[i]=0; }
    }

    void add(const T x)         { if (counter<ovMSMax) list[counter++]=x; }
    int count()                 { return counter; }
    T get(int i)                { return (i >=0 && i<ovMSMax) ? list[i] : 0; }
    T get(const char *i)        { return get(find(i)); }
    void setInitialized(int i)  { if (i >=0 && i<ovMSMax) inited[i]=1; }
    int isInitialized(int i)    { return (i >=0 && i<ovMSMax) ? inited[i]: 0; }
    void setInitialized(const char *i) { setInitialized(find(i)); }
    int isInitialized(const char* i) { return isInitialized(find(i)); }
    
    int find(const char *identifier)
    {
        for (int i=0; i<counter; i++)
            if (!strcmp(list[i]->identifier(), identifier)) return i;
        return -1;
    }
    
protected:
    T list[ovMSMax];
    int inited[ovMSMax];
    int counter;
};

class OVModuleManager : public OVBase
{
public:
    void addFile(OVLoadableModuleFile *f)
    {
        int i;
        for (i=0; i<f->availableInputMethodCount(); i++)
            imlist.add(f->newInputMethod(i));
        for (i=0; i<f->availableOutputFilterCount(); i++)
            oflist.add(f->newOutputFilter(i));        
    }
    
    void addInputMethod(OVInputMethod *im) { imlist.add(im); }
    void addOutputFilter(OVOutputFilter *of) { oflist.add(of); }
    
    OVModuleList<OVInputMethod*> imlist;
    OVModuleList<OVOutputFilter*> oflist;
};

#endif

