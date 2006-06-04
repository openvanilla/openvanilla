#include "AVConfig.h"

#ifdef WIN32
#include <windows.h>
#include <shlobj.h>
#include <io.h>
#define PATH_MAX MAX_PATH
#endif


AVConfig::AVConfig()
{
	char OV_BASEDIR[PATH_MAX];
	char OV_USERDIR[PATH_MAX];
	char OV_MODULEDIR[PATH_MAX];
#ifdef WIN32
	GetWindowsDirectoryA(OV_BASEDIR, MAX_PATH - 14);
	sprintf(OV_BASEDIR, "%s\\%s", OV_BASEDIR, "OpenVanilla\\");
	sprintf(OV_MODULEDIR, "%s\\%s", OV_BASEDIR, "Modules\\");

	if (SHGetFolderPathA(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE, 
				NULL, 0, OV_USERDIR) >= 0) 
	{
		sprintf(OV_USERDIR, "%s\\%s", OV_USERDIR, "OpenVanilla\\");

		/* Try to create the directory if it does not yet
		   exists.  */
		if (_access (OV_USERDIR, 0))
			CreateDirectoryA(OV_USERDIR, NULL);
	}
	else
		sprintf(OV_USERDIR, "%s\\%s", OV_BASEDIR, "User\\");
#else
#endif

	setBaseDir(OV_BASEDIR);
	setUserDir(OV_USERDIR);
	setModuleDir(OV_MODULEDIR);
}

const char *AVConfig::getBaseDir()
{
	return basedir.c_str();
}
const char *AVConfig::getUserDir()
{
	return userdir.c_str();
}
const char *AVConfig::getModuleDir()
{
	return moduledir.c_str();
}
void AVConfig::setBaseDir(const char *d)
{
	basedir = d;
}
void AVConfig::setUserDir(const char *d)
{
	userdir = d;
}
void AVConfig::setModuleDir(const char *d)
{
	moduledir = d;
}
