#ifndef AVLoaderUtility_h
#define AVLoaderUtility_h

#include "OpenVanilla.h"
#include "OVUtility.h"
#include <string>
#include <vector>

#ifdef WIN32
#include <windows.h>
//	Replace lt_ functions with native Win32 API
typedef		HMODULE						lt_dlhandle;
#define		lt_dlopen(x)				LoadLibraryA(x)
#define		lt_dlsym(l, n)				GetProcAddress(l, n)
#define		lt_dlexit()
#define		lt_dlinit()
#define		lt_dlsetsearchpath(x)
#else
extern "C" {
#include "ltdl.h"
}
#endif

using namespace std;

typedef OVModule* (*TypeGetModule)(int);
typedef int (*TypeInitLibrary)(OVService*, const char*);
typedef unsigned int (*TypeGetLibVersion)();
struct OVLibrary {
   lt_dlhandle handle;
   TypeGetModule getModule;
   TypeInitLibrary initLibrary;
   TypeGetLibVersion getLibVersion;
};

void AVLoadEverything(string path, OVService *srv, vector<OVModule*> &vector);
void AVUnloadLibrary(vector<OVModule*> &vector);


#endif // AVLoaderUtility_h
