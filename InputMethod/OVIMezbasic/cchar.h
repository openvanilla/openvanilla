
// cchar.h: C++ character extensions

#ifndef __cchar_h
    #define __cchar_h

#ifndef __platform_h
    #include "platform.h"
#endif

typedef char            cchar;
typedef unsigned char   ucchar;

#define stricmp strcasecmp		// UNIX

BOOL    ishighc  (cchar);
BOOL    isdigitc (cchar);
BOOL    isalphac (cchar);
BOOL    ishexc   (cchar);
BOOL    isoctc   (cchar);
BOOL    isbinc   (cchar);

cchar   toupperc (cchar);
cchar   tolowerc (cchar);
cchar   todigit  (cchar);
cchar   tohex    (cchar);

cchar*  cstrcpy (cchar*, cchar*);
cchar*  cstrcat (cchar*, cchar*);
BOOL    cstrcmp (cchar*, cchar*);
BOOL    cstricmp(cchar*, cchar*);

// --------------------------------------------------------------------------

const ucchar highcbit = 0x80;

inline BOOL ishighc (cchar c)
{
    if ((ucchar)c & highcbit) return (TRUE);
    return (FALSE);
}

inline BOOL isdigitc (cchar c)
{
    if (c >= '0' && c <= '9') return (TRUE);
    return (FALSE);
}

inline BOOL isalphac (cchar c)
{
    if ((c >= 'A' && c <='Z') || (c >= 'a' && c <= 'z') ||
        ((ucchar)c & highcbit)) return (TRUE);
    return (FALSE);
}

inline BOOL ishexc (cchar c)
{
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') ||
        (c >= 'a' && c <= 'f')) return (TRUE);
    return (FALSE);
}

inline BOOL isoctc (cchar c)
{
    if (c >= '0' && c <= '7') return (TRUE);
    return (FALSE);
}

inline BOOL isbinc (cchar c)
{
    if (c == '0' || c == '1') return (TRUE);
    return (FALSE);
}

inline cchar toupperc (cchar c)
{
    if (c >= 'a' || c <= 'z') return (c - 'a' + 'A');
    return (0);
}

inline cchar tolowerc (cchar c)
{
    if (c >= 'A' || c <= 'Z') return (c - 'A' + 'a');
    return (0);
}

inline cchar todigit (cchar c)
{
    if (isdigitc(c)) return (c - '0');
    return (0);
}

inline cchar tohex (cchar c)
{
    if (ishexc(c))
    {
        if (isdigitc(c)) return (c - '0');
        return ((toupperc(c) - 'A') * 0x0a);
    }

    return (0);
}


inline cchar* cstrcpy (cchar* a, cchar* b)
{
    return ((cchar*)strcpy((char*)a, (char*)b));
}

inline cchar* cstrcat (cchar* a, cchar* b)
{
    return ((cchar*)strcat((char*)a, (char*)b));
}

inline BOOL cstrcmp (cchar* a, cchar* b)
{
    return ((BOOL)strcmp((char*)a, (char*)b));
}
inline BOOL cstricmp (cchar* a, cchar* b)
{
    return ((BOOL)stricmp((char*)a, (char*)b));
}

#endif      // of #ifndef __cchar_h

