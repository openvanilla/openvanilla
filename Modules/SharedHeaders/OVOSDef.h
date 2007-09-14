#ifndef OVOSDEF_H
#define OVOSDEF_H

#include <cstring>
#include <climits>
#ifdef WIN32
	#define PATH_MAX _MAX_PATH

	//<comment author='b6s'>
	// Why does it not work? =_=
	//#define _SCL_SECURE_NO_WARNINGS
	//</comment>
	#pragma warning(disable:4996)

	#define strcasecmp stricmp
#endif

#endif
