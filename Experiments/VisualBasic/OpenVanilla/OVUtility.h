// OVUtility.h: Inline utilities
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

// Thanks to modern C++ compiler, we can write this in the header file. 
// No extra compiling/library making/linking is even needed!

#ifndef __OVUtility_h
#define __OVUtility_h

// debug routine
#ifdef OV_DEBUG 
    #include <stdarg.h>
    #include <stdio.h>
    inline static void murmur(const char* format,...) {
        va_list args;
        va_start (args, format);
        vfprintf (stderr, format, args);
        va_end (args);
        fprintf (stderr, "\n");
    }
#else
    inline static void murmur(const char* format,...) {
    }
#endif

#endif
