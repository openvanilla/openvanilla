
// platform.h: common used type definitions and platform configurations

#ifndef __platform_h
    #define __platform_h

// PLATFORM INDICATORS
// ===================
// #define PF_REALMODE          // Intel x86, MS-DOS, BC++, VC++
   #define PF_FLATADDRESS       // Win32s, WinNT, UNIX, Watcom C+

// #define PF_WINDOWS           // Windows platform
// #define PF_NOIOSTREAM        // do not use iostream
// #define PF_CPP20             // AT&T C++ 2.0 standard (no new[])
   #define PF_EXCEPTION         // exception handling (AT&T C++ 2.1 & above)

// #ifndef PF_NOSTREAM
//    #include <iostream.h>
// #endif

#ifdef PF_WINDOWS               // if on Windows platform but no windows.h
    #ifndef _INC_WINDOWS
        #ifndef __WINDOWS_H
            #include <windows.h>
        #endif
    #endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#ifndef PF_WINDOWS
    typedef short               BOOL;
    typedef unsigned char       BYTE;
    typedef unsigned short      WORD;       // 16-bit word
    typedef unsigned long       DWORD;      // 32-bit word

//    const BOOL TRUE = 1, FALSE = 0;
#endif

    typedef unsigned short  WORD16;         // less confusing
    typedef unsigned long   WORD32;
    typedef unsigned int    uint;           // for easy-using
    typedef unsigned short  ushort;
    typedef unsigned long   ulong;


#ifndef PF_EXCEPTION
    #define throw(x)
    #define try             if(1)
    #define catch(x)        if(0)
#endif

#endif      // of #ifndef __platform_h

