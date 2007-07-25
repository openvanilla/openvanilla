#ifndef OVOSDEF_H
#define OVOSDEF_H

#ifndef WIN32
	#include <limits.h>      //for PATH_MAX
#else
	#include <stdlib.h>
	#define PATH_MAX _MAX_PATH

	//<comment author='b6s'>
	// Why does it not work? =_=
	//#define _SCL_SECURE_NO_WARNINGS
	//</comment>
	#pragma warning(disable:4996)
	#include <string.h>
	#define strcasecmp stricmp
#endif

#endif
