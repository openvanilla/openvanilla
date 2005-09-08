#include "AVLoaderUtility.h"
#include <string>
#ifdef WIN32
#include <windows.h>
#endif
using namespace std;

vector<OVModule*> *mod_vector;
static OVService *lsrv;

static OVLibrary* open_module(const char* modname){
    OVLibrary* mod = new OVLibrary();
    mod->handle = lt_dlopen(modname);
    if(mod->handle == NULL){
	murmur("dlopen %s failed\n", modname);
	goto OPEN_FAILED;
    }
    mod->getModule = (TypeGetModule)lt_dlsym( mod->handle, 
	    "OVGetModuleFromLibrary" );
    mod->getLibVersion = (TypeGetLibVersion)lt_dlsym( mod->handle, 
	    "OVGetLibraryVersion" );
    mod->initLibrary = (TypeInitLibrary)lt_dlsym( mod->handle,
	    "OVInitializeLibrary" );
    if( !mod->getModule || !mod->getLibVersion || !mod->initLibrary ){
	murmur("dlsym %s failed\n", modname);
	goto OPEN_FAILED;
    }
    if( mod->getLibVersion() < OV_VERSION ){
	murmur("%s %d is too old\n", modname, mod->getLibVersion());
	goto OPEN_FAILED;
    }
    return mod;

OPEN_FAILED:
    delete mod;
    return NULL;
}

static void load_module(string path, string file)
{
    murmur("Load OV module: %s\n", file.c_str());
    OVLibrary* mod = open_module((path + file).c_str());
    if(mod){
	OVModule* m;
	mod->initLibrary(lsrv, path.c_str());
	for(int i=0; m = mod->getModule(i); i++)
	{	
	    mod_vector->push_back(m);
	    murmur("Load OVModule: %s\n", m->localizedName("zh_TW"));
	}
	delete mod;
    }
}
typedef void (*loadfunc)(string path, string file);

// scan_dir is platform dependent, each platform must have it's own implement
static void scan_dir(string path, loadfunc func)
{
#ifdef WIN32
    BOOL fFinished;
    HANDLE hList;
    WIN32_FIND_DATA FileData;
    hList = FindFirstFile((path + "*").c_str(), &FileData);
    if(hList == INVALID_HANDLE_VALUE)
    {
	murmur("No files found\n");
    }
    else
    {
	fFinished = FALSE;
	while (!fFinished)
	{
	    if(strstr(FileData.cFileName, ".dll") || strstr(FileData.cFileName, ".DLL"))
	    {
		func(path, FileData.cFileName);
	    }
	    if (!FindNextFile(hList, &FileData))
	    {
		if (GetLastError() == ERROR_NO_MORE_FILES)
		{
		    fFinished = TRUE;
		}
	    }
	}
    }
    FindClose(hList);
#else
#endif
}

void AVLoadEverything(string path, OVService *srv, vector<OVModule*> &vector)
{   
    mod_vector = &vector;
    lt_dlinit();
    lt_dlsetsearchpath(path.c_str());
    lsrv = srv;
    scan_dir(path, load_module);
}

void AVUnloadLibrary(vector<OVModule*> &vec)
{
	/* FIXME: uncomment this will cause segfault in ime.
	for(int i = 0; i < vec.size(); i++)
		if(vec.at(i) != NULL)
			delete vec.at(i);
	*/
	lt_dlexit();
}
