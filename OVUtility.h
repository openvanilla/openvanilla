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

#endif /* _OVUTILITY_H */

