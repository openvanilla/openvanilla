// OVModuleLoader.h
// This provides an abstract interface for writing OV-conforming Loaders.

#ifndef __OVModuleLoader_h
#define __OVModuleLoader_h

#include "OpenVanilla.h"

typedef OVInputMethod* OVMNewInputMethodType(int);
typedef OVBufferFilter* OVMNewBufferFilterType(int);
typedef unsigned int OVMVersionType();
typedef int OVMAvailableCountType(char*);

class OVModuleLoader : public OVObject {
public:
    virtual int load(const char *fileName, const char *modulePath, const char 
        *userPath)=0;
    virtual int getInputMethodCount()=0;
    virtual int getBufferFilterCount()=0;
    virtual OVInputMethod *newInputMethod(int idx)=0;
    virtual OVBufferFilter *newBufferFilter(int idx)=0;
};






#endif
