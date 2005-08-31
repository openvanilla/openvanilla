#ifndef AVLoaderUtility_h
#define AVLoaderUtility_h

#include "OpenVanilla.h"
#include "OVUtility.h"
#include <string>
#include <vector>
extern "C" {
#include "ltdl.h"
}

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

vector<OVModule*> AVLoadEverything(string path, OVService *srv);


#endif // AVLoaderUtility_h
