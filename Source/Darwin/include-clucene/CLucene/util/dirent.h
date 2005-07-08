#ifndef lucene_util_dirent_H
#define lucene_util_dirent_H

#include "CLucene/StdHeader.h"

#ifndef HAVE_DIRENT_H

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#include <windows.h>
#include <errno.h>
#include <io.h>

#include <sys/stat.h>
#include <stddef.h>

namespace lucene{ namespace util {
/*
 * dirent.c
 *
 * Derived from DIRLIB.C by Matt J. Weinstein 
 * This note appears in the DIRLIB.H
 * DIRLIB.H by M. J. Weinstein   Released to public domain 1-Jan-89
 *
 * Updated by Jeremy Bettis <jeremy@hksys.com>
 * Significantly revised and rewinddir, seekdir and telldir added by Colin
 * Cut down again & changed by Ben van Klinken
 * Peters <colin@fu.is.saga-u.ac.jp>
 *
 */

#ifdef _UNICODE
# define Struct_Find _wfinddata_t
#else
# define Struct_Find _finddata_t
#endif

struct dirent
{
	unsigned short	d_namlen;	/* Length of name in d_name. */
	char_t *d_name;		/* File name. */
};

struct DIR
{
	/* disk transfer area for this dir */
	struct Struct_Find dd_dta;

	/* dirent struct to return from dir (NOTE: this makes this thread
	 * safe as long as only one thread uses a particular DIR struct at
	 * a time) */
	struct dirent		dd_dir;

	/* _findnext handle */
	intptr_t			dd_handle;

	/*
         * Status of search:
	 *   0 = not started yet (next entry to read is first entry)
	 *  -1 = off the end
	 *   positive = 0 based index of next entry
	 */
	int			dd_stat;
	
	/* given path for dir with search pattern (struct is extended) */
	char_t			dd_name[CL_MAX_DIR];

};

#define DIRENT_SEARCH_SUFFIX _T("*")
#define DIRENT_SLASH PATH_DELIMITER

DIR* opendir (const char_t* filespec);
struct dirent*	readdir (DIR* dir);
int	closedir (DIR* dir);


}}

#elif defined (HAVE_DIRENT_H)
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif

#endif //HAVE_DIRENT_H
#endif
