#ifndef _stdheader_h
#define _stdheader_h

// default includes
#include <ctype.h>
#include <map>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <vector>
#include <fcntl.h> //file constants, also will include io.
#include <stdio.h>


#if defined(_DEBUG) && defined (_WIN32) && defined (COMPILER_MSVC)
#define _CRTDBG_MAP_ALLOC
#endif
#include <stdlib.h>
#if defined(_DEBUG) && defined (_WIN32) && defined (COMPILER_MSVC)
//with this header enabled one can detect memory leaks using Microsoft Visual C++
//using the functions _CrtSetDbgFlag and _CrtSetBreakAlloc
#include <crtdbg.h>
#endif


///////////////////////////////////////
// default options
#include "CLConfig.h"
//
///////////////////////////////////////

#include "CLucene/debug/condition.h"



//Common data types
#define l_byte_t unsigned char

// DSR:2004.07.04:AMD64-port:
// int_t must be able to hold signed 32-bit values, and ideally it should
// remain consistently sized across platforms (this consistency makes life
// easier for client programmers, especially those using SWIG).
//
// Previously, int_t was an alias for the native long, but on some 64-bit
// platforms (including AMD64/Linux), long is 64 bits.  To achieve consistency
// between ia32 and AMD64, we use the native int if it can hold at least 32
// bits.
#define INT_CAN_HOLD_32 (INT_MAX >= 2147483647 && INT_MIN <= (-2147483647-1))
#if INT_CAN_HOLD_32
  #define int_t int
  #define INT_T_MAX INT_MAX
#else
  #define int_t long
  #define INT_T_MAX LONG_MAX
#endif

#define float_t float
#define floatSquareRoot sqrt
#define floatLog log
#define uint_t unsigned long

//windows attributes - avoids having to include windows.h
//todo2: this should be defined on a per platform basis???
#ifndef MAX_PATH
#ifdef PATH_MAX
#define MAX_PATH PATH_MAX
#else
#define MAX_PATH 256    /* Should be safe for any weird systems that do not define it */
#endif // PATH_MAX
#endif // MAX_PATH

#define CL_MAX_DIR 8192 //32 directories * 256 bytes...

#ifndef FILE_ATTRIBUTE_DIRECTORY
#define FILE_ATTRIBUTE_DIRECTORY	16
#endif // FILE_ATTRIBUTE_DIRECTORY

//misc options
#define CHAR_RADIX 36
#define DELETE_TYPE_NONE 0
#define DELETE_TYPE_DELETE 1
#define DELETE_TYPE_DELETE_ARRAY 2

//error catching
#define _TRY try
#define _FINALLY(x) catch(...){ x; throw; } x //note, doesn't code in x if a normal return occurs
#define _THROWC(y) throw THROW_TYPE(y)


//shortcut defines
#define StringArray lucene::util::VoidList<const char_t*>
#define StringArrayConst lucene::util::VoidList<const char_t*>
#define StringArrayIterator vector<const char_t*>::const_iterator
#define StringArrayConstIterator vector<const char_t*>::iterator
#define _DELETE(x) if(x){delete x; x=NULL;}
//Macro to delete an array
#define _DELETE_ARRAY(x) if (x){delete [] x; x=NULL;}



///////////////////////////////////////////////////////////////////
//
//Platform specific definitions (in seperate files)
//
#ifdef _WIN32
 #include "CLucene/LuceneWindows.h"
#else

#ifdef _APPLE
 #include "CLucene/LuceneApple.h"
#else // _APPLE
 #include "CLucene/LuceneUnix.h"
#endif

#endif
//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
// Non platform specific Unicode/NonUncode definitions
//
#ifdef _UNICODE
#include <wchar.h>

// types aliases
#define char_t wchar_t

#ifndef _T
#define _T(x)       L ## x
#endif // _T

#define isSpace iswspace
#define isDigit iswdigit
#define isAlNum iswalnum
#define toLower towlower

// functions aliases
#define TO_CHAR_T(x) lucene::util::Misc::charToWide(x)
#define _cout wcout
#define _cin wcin

#define _THROWX(y) throw THROW_TYPE(Misc::wideToChar(y))
#define _CREATE_THROWTYPE(y) (THROW_TYPE(Misc::wideToChar(y)))

#define stringCompare wcscmp
#define stringNCompare wcsncmp
#define stringNCopy wcsncpy
#define stringCopy wcscpy
//#define stringICompare _wcsicmp
#define stringPrintF swprintf
#define stringSpn wcsspn
#define stringCSpn wcscspn
#define stringLength wcslen
#define stringToFloat wcstod
#define stringToInteger _wtoi
#define stringFind wcsstr
#define stringFindChar wcschr
#define stringAdvance _wcsninc
#define stringCat wcscat
#define integerToString _i64tow
#define fileRename _wrename
#define fileFullName _wfullpath
#define printFormatted wprintf
#define stringToken wcstok

wchar_t *lucenewcsdup(const wchar_t *v);
#define stringDuplicate lucenewcsdup

#endif // _UNICODE

#ifndef _UNICODE

// types aliases
#define char_t char

#ifndef _T
#define _T(x)       x
#endif // _T

// functions aliases
#define isSpace(x) (((unsigned)x) < 0x100?isspace(x):0)
#define isDigit(x) (((unsigned)x) < 0x100?isdigit(x):0)
#define isAlNum(x) (((unsigned)x) < 0x100?isalnum(x):0)
#define toLower(x) (((unsigned)x) < 0x80?tolower(x):x)

char * lucenestrdup(const char *v);

#define stringDuplicate lucenestrdup

#define TO_CHAR_T(x) stringDuplicate(x)
#define _cout cout
#define _cin cin

#define _THROWX(y) throw THROW_TYPE(y)
#define _CREATE_THROWTYPE(y) (THROW_TYPE(y))

#define stringCompare strcmp
#define stringNCompare strncmp
#define stringNCopy strncpy
#define stringCopy strcpy
#define stringPrintF sprintf
#define stringSpn strspn
#define stringCSpn strcspn
#define stringLength strlen
#define stringToFloat strtod
#define stringToInteger atoi
#define stringFind strstr
#define stringFindChar strchr
#define stringAdvance strinc
#define stringCat strcat
#define integerToString _i64toa
#define fileRename rename
#define fileFullName _fullpath
#define printFormatted printf
#define stringToken strtok
#endif // not _UNICODE

// end non-platform specific unicode/non-unicode defines.
//
//
///////////////////////////////////////////////////////////////////


//lucene threading support
#include "LuceneThreads.h"

//misc items for precompiled header in windows
#ifdef _WIN32
#include "CLucene/util/Misc.h"
#include "CLucene/store/InputStream.h"
#include "CLucene/store/OutputStream.h"
#include "CLucene/util/VoidMap.h"
#include "CLucene/util/VoidList.h"
#include "CLucene/store/Directory.h"
#endif
#endif // STDHEADER_H
