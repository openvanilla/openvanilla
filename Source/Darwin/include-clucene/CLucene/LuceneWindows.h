#ifndef LuceneWindows_H
#define LuceneWindows_H

#ifdef _WIN32

#ifdef MSVC6
#pragma warning(disable: 4786) // This would be very annoying
#endif

///////////////////////////////////////////////////////////////////
//
// now we know we are WIN32 system.
//
/* DSR: Previous 64-bit-int-type-selection code didn't work ideally on MinGW. */


#ifdef LLONG_MIN
# define ulong_t unsigned long long
# define long_t long long
#else
# define ulong_t unsigned __int64
# define long_t __int64
#endif

# define processSleep _sleep

# define PATH_DELIMITER _T("\\")
# define Struct_Stat _stat

//Test for large file support in different ways depending upon compiler.
//MSVC:
//  All version of Windows since 98 (except CE) include a C library that
//  supports large files.  There are widely used file systems on Windows that
//  don't support large files, but presumably the MS C library will dynamically
//  determine whether the underlying file system has the requisite support.
//MinGW:
//  Typically available, but can be excluded.
#if (defined(_MSC_VER) && !defined(_WIN32_WCE)) || (defined(COMPILER_MINGW32) && !defined(__NO_MINGW_LFS))
#  define _LARGE_FILE_SUPPORT
# endif

# ifdef _LARGE_FILE_SUPPORT
#  define fileSize _filelengthi64
# else
#  define fileSize _filelength
# endif

# define uchar_t char_t

# ifdef _EXPORT_SYMBOLS
#  define EXPORT_LUCENE __declspec(dllexport)
# else
#  define EXPORT_LUCENE
# endif
#include <exception>

#ifdef MSVC6
 typedef int intptr_t;
 #include <tchar.h>
#endif

#ifdef COMPILER_MINGW32
  /* DSR: MinGW needs the following include to import runtime_error: */
  #include <stdexcept>
  /* DSR: MinGW GCC 3.3.3 does not recognize runtime_exception; I reverted to
  ** runtime_error, which CL 0.8.9 used. */
  #define THROW_TYPE runtime_error
  typedef int intptr_t;
#else
# define THROW_TYPE exception
#endif

# ifdef _UNICODE
/* DSR: Standard name for _wcstoi64, which MinGW uses, is wcstoll: */
#  ifdef _MSC_VER
#   define stringToIntegerBase _wcstoi64
#  else
#   define stringToIntegerBase wcstoll
#  endif
#  define Cmd_Stat _wstat
#  define stringICompare _wcsicmp
#  define openFile _wopen
#  define makeDirectory _wmkdir
#  define stringUpper _wcsupr
#  define stringLower _wcslwr
#  define unlinkFile _wunlink
# else
/* DSR: Standard name for _strtoi64, which MinGW uses, is strtoll: */
#  ifdef _MSC_VER
#   define stringToIntegerBase _strtoi64
#  else
#   define stringToIntegerBase strtoll
#  endif
#  define Cmd_Stat _stat
#  define stringICompare _stricmp
#  define openFile _open
#  define makeDirectory _mkdir
#  define stringUpper _strupr
#  define stringLower _strlwr
#  define unlinkFile _unlink
# endif


// end WIN32 specific defines.
//
//
///////////////////////////////////////////////////////////////////

#endif // _WIN32


#endif
