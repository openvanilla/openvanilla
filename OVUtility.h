/* @(#)OVUtility.h
 */

#ifndef _OVUTILITY_H
#define _OVUTILITY_H 1

#include <stdio.h>

/**
 * Debugging routine.
 * This function accept the same argument as in fprintf(),
 * but it doesn't print anything unless OVDEBUG is set at compile time.
 */
inline void murmur(FILE *f, const char* format,...) {
#ifdef OVDEBUG
    va_list args;
    va_start (args, format);
    vfprintf (f,format,args);
    va_end (args);
    fprintf (f,"\n");
#endif
}

#endif /* _OVUTILITY_H */

