/* @(#)OVUtility.h
 */

#ifndef __OVUtility_h
#define __OVUtility_h

#include <stdarg.h>
#include <stdio.h>

/**
 * Debugging routine.
 * This function accept the same argument as in printf(),
 * and print message to stderr. But it doesn't print anything
 * unless OVDEBUG is set at compile time.
 */
#ifdef OVDEBUG
inline static void murmur(const char* format,...) {
    va_list args;
    va_start (args, format);
    vfprintf (stderr, format, args);
    va_end (args);
    fprintf (stderr, "\n");
}
#else
inline static void murmur(const char* format,...)
{
}
#endif

const int ovMaxPoolSize=32;     // if over this, rethink *your* design...

class OVAutoDeletePool
{
public:
    OVAutoDeletePool() : p(0) {}
    ~OVAutoDeletePool() { for (int i=0; i<p; i++) delete pool[i]; }
    OVObject* add(OVObject *o)
    {
        if (!o) return o;
        if (p==ovMaxPoolSize) return o; // note: no exception
        return pool[p++]=o;
    }
        
protected:
    int p;
    OVObject *pool[ovMaxPoolSize];
};

#define OVAUTODELETE    OVAutoDeletePool _ovPool
#define OVSafe(exp)     _ovPool.add(exp)

#endif /* _OVUTILITY_H */

