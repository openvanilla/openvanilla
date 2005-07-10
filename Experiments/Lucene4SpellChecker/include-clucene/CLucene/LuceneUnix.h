#ifndef LuceneUnix_H
#define LuceneUnix_H

///////////////////////////////////////////////////////////////////
//
// should be unix or unix like platform
//

#include <sys/types.h>

#ifdef __GNUC__ // __GNUC__ compiler

using namespace std;

/* DSR:PROPOSED: Allow non-make-based build process on Unix if the build
** utility *explicitly* indicates so. */
#ifndef _SUPPRESS_MAKE_BASED_CONFIG
# include "config.h"
#endif

typedef unsigned long long ulong_t;
typedef long long long_t;

typedef int INT;
typedef char CHAR;
typedef CHAR *LPSTR;
typedef wchar_t WCHAR;
typedef wchar_t* PWCHAR;
typedef WCHAR *LPWSTR;
typedef long long  LONGLONG;
typedef unsigned long long ULONGLONG;

#define processSleep lucenesleep
#define PATH_DELIMITER _T("/")

//GCC's sys/types.h defines __USE_LARGEFILE64 if the C library supports large
//files.
  //TODO: The double underscore may indicate that the symbol is private, so
  //alternatives are welcome.
# ifdef __USE_LARGEFILE64
#  define _LARGE_FILE_SUPPORT
# endif

#define openFile open
#define makeDirectory(x) mkdir(x, 0777) // must do this or file will be created Read only
#define _S_IREAD  0444
#define _S_IWRITE 0333  // write and execute permissions

#include <stdexcept>
#define THROW_TYPE runtime_error
#undef _T //will redefine again later


#ifdef _UNICODE
# define uchar_t wchar_t
# define Cmd_Stat wstat
# define Struct_Stat wstat
# define stringToIntegerBase strtoll
# define stringUpper _wcsupr
# define stringLower _wcslwr
# define unlinkFile _wunlink
#else
# define Cmd_Stat stat
# define Struct_Stat stat
# define stringToIntegerBase strtoll
# define stringUpper strupr
# define stringLower strlwr
# define unlinkFile unlink
#ifdef UTF8
 typedef unsigned long uchar_t;
 #define stringICompare lc_utf8_stricmp

 #undef toLower
 #undef isSpace
 #undef isDigit
 #undef isAlNum
 #define toLower lc_unichar_tolower
 #define isSpace lc_utf32_isspace
 #define isDigit lc_utf32_isdigit
 #define isAlNum lc_utf32_isalnum
 int     lc_utf8_stricmp( const char * str1, const char * str2 );
 LPSTR   lc_utf8_strcasefold( char * str1 );
 int     lc_unichar_to_utf8 (uchar_t c, char *outbuf);
 bool    lc_utf32_isletter(uchar_t c);
 bool    lc_utf32_isalnum(uchar_t c);
 bool    lc_utf32_isdigit(uchar_t c);
 bool    lc_utf32_isspace(uchar_t c);
 uchar_t lc_utf8_get_char (const char *p, int *len);
 char *  lc_ucs4_to_utf8 (const uchar_t *str, int len);
 uchar_t lc_unichar_tolower (uchar_t c);
 extern const char * const lc_utf8_skip;
 #define lc_utf8_next_char(p) (char *)((p) + lc_utf8_skip[*(unsigned char *)(p)])
#else
 #define uchar_t char_t
 #define stringICompare strcasecmp  // I know this will cause Unicode
			  // issues but for now this will do.
#endif
#endif // _UNICODE



void lucenesleep( int_t MilliSeconds);
LPSTR strlwr( LPSTR str );
LPSTR strupr( LPSTR str );
long_t fileSize(int_t filehandle);
char * _i64toa(
	   LONGLONG value, /* [I] Value to be converted */
	   char *str,      /* [O] Destination for the converted value */
	   int radix);      /* [I] Number base for conversion */
LPWSTR _i64tow(
	   LONGLONG value, /* [I] Value to be converted */
	   LPWSTR str,     /* [O] Destination for the converted value */
	   INT radix);      /* [I] Number base for conversion */

#endif // __GNUC__


#endif
