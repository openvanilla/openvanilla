// OVModuleLoader.h

#ifndef __OVModuleLoader_h
#define __OVModuleLoader_h

#include "OpenVanilla.h"

typedef OVInputMethod* OVMNewInputMethodType(int);
typedef OVBufferFilter* OVMNewBufferFilterType(int);
typedef unsigned int OVMVersionType();
typedef int OVMAvailableCountType(char*);

class OVModuleLoader : public OVObject {
public:
    OVModuleLoader();
    virtual int load(const char *fileName, const char *modulePath, const char 
        *userPath);
    virtual int getInputMethodCount();
    virtual int getBufferFilterCount();
    virtual OVInputMethod *newInputMethod(int idx);
    virtual OVBufferFilter *newBufferFilter(int idx);
    
protected:
    void *libhandle;
    int imcount, bfcount;
    OVMNewInputMethodType *newim;
    OVMNewBufferFilterType *newbf;
};





#endif
