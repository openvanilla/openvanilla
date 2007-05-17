#ifndef OVOSDEF_H
#define OVOSDEF_H

#ifndef WIN32
	#include <limits.h>      //for PATH_MAX
#else
	#include <stdlib.h>
	#define PATH_MAX _MAX_PATH

	#include <string.h>
	#define strcasecmp stricmp
#endif

#endif
