/* @(#)OVUtility.h
 */

#ifndef _OVUTILITY_H
#define _OVUTILITY_H 1

#include <stdio.h>

/**
 * Debugging routine.
 * This function accept the same argument as in printf(),
 * and print message to stderr. But it doesn't print anything
 * unless OVDEBUG is set at compile time.
 */
inline void murmur(const char* format,...) {
#ifdef OVDEBUG
    va_list args;
    va_start (args, format);
    vfprintf (stderr,format,args);
    va_end (args);
    fprintf (stderr,"\n");
#endif
}

const int ovMaxPoolSize=256;

class OVAutoDeletePool
{
public:
    OVAutoDeletePool() : p(0) {}
    ~OVAutoDeletePool() { for (int i=0; i<p; i++) if (pool[i]) delete pool[i]; }
    OVObject* add(OVObject *o)
    {
        if (p==ovMaxPoolSize) return o; // note: no exception
        return pool[p++]=o;
    }
        
protected:
    int p;
    OVObject pool[ovMaxPoolSize];
};

#endif /* _OVUTILITY_H */

