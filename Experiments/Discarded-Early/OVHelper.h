// OVHelper.h :
// Abstract interface and utilities for writing an IM module


#ifndef __OVHelper_h
#define __OVHelper_h

#include <stdlib.h>
#include <ctype.h>
#include "OpenVanilla.h"

class OVKeySequence : public OVObject
{
public:
    virtual int isvalidchar(char) { return 0; }
    virtual int add(char) { return 0; }
    virtual int clear() { return 1; }
    virtual int removelast() { return 1; }
};





#endif