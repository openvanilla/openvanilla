//#define OV_DEBUG
#ifndef WINCE
#ifdef UNICODE
#undef UNICODE
#endif
#ifdef _UNICODE
#undef _UNICODE
#endif
#endif

#include "AVLoaderUtility.h"
#include <string>
#if defined(WIN32) || defined(WINCE)
#include <windows.h>
#endif
using namespace std;

#ifdef WINCE
vector<HINSTANCE> *lib_vector;
#endif
vector<OVModule*> *mod_vector;
static OVService *lsrv;

static OVLibrary* open_module(const char* modname){
    OVLibrary* mod = new OVLibrary();
#ifndef WINCE
    mod->handle = lt_dlopen(modname);
#else
	WCHAR wbuf[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, modname, (int)strlen(modname)+1, wbuf, MAX_PATH);
	//mbstowcs(wbuf, modname, MAX_PATH);
    mod->handle = LoadLibraryW(wbuf);
#endif
    if(mod->handle == NULL){
	murmur("dlopen %s failed\n", modname);
	goto OPEN_FAILED;
    }
#ifndef WINCE
    mod->getModule = (TypeGetModule)lt_dlsym( mod->handle, 
	    "OVGetModuleFromLibrary" );
    mod->getLibVersion = (TypeGetLibVersion)lt_dlsym( mod->handle, 
	    "OVGetLibraryVersion" );
	mod->initLibrary = (TypeInitLibrary)lt_dlsym( mod->handle,
	    "OVInitializeLibrary" );
#else
	mod->getModule = (TypeGetModule)GetProcAddressW( mod->handle, 
	    L"OVGetModuleFromLibrary" );
    mod->getLibVersion = (TypeGetLibVersion)GetProcAddressW( mod->handle, 
		L"OVGetLibraryVersion" );
	mod->initLibrary = (TypeInitLibrary)GetProcAddressW( mod->handle,
	    L"OVInitializeLibrary" );
#endif
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
#if defined(WIN32)||defined(WINCE)
    BOOL fFinished;
    HANDLE hList;
    WIN32_FIND_DATAW FileData;
	
	WCHAR wbuf[MAX_PATH];
	char buf[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, (path + "*").c_str(), (int)(path.length()+2), wbuf, MAX_PATH);
	//mbstowcs(wbuf ,(path + "*").c_str(), MAX_PATH);

    hList = FindFirstFileW(wbuf, &FileData);
    if(hList == INVALID_HANDLE_VALUE)
    {
	murmur("No files found\n");
    }
    else
    {
	fFinished = FALSE;
	while (!fFinished)
	{
	    if(wcsstr(FileData.cFileName, L".dll") || wcsstr(FileData.cFileName, L".DLL"))
	    {
			//wcstombs(buf, FileData.cFileName, MAX_PATH);
			WideCharToMultiByte(CP_UTF8, 0, FileData.cFileName, (int)(wcslen(FileData.cFileName)+1), buf, MAX_PATH,NULL, NULL);
			func(path, buf);
	    }
	    if (!FindNextFileW(hList, &FileData))
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
#ifndef WINCE
    lt_dlinit();
    lt_dlsetsearchpath(path.c_str());
#endif
    lsrv = srv;
    scan_dir(path, load_module);
}

void AVUnloadLibrary(vector<OVModule*> &vec)
{
	
	/* FIXME: uncomment this will cause segfault in ime. 
	for(size_t i = 0; i < vec.size(); i++)
		if(vec.at(i) != NULL)
			delete vec.at(i);
	*/
#ifdef WINCE

#else
	lt_dlexit();
#endif
	vec.clear();

}
