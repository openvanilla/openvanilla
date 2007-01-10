#ifndef OVOSDEF_H
#define OVOSDEF_H

#ifndef WIN32
        #include <limits.h>      //for PATH_MAX
#else
        #include <windows.h>
        #define PATH_MAX MAX_PATH

        #define strcasecmp stricmp
#endif

#endif
