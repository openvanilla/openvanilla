#include "AVConfig.h"

#ifdef WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#endif


AVConfig::AVConfig()
{
	char OV_BASEDIR[PATH_MAX];
	char OV_USERDIR[PATH_MAX];
	char OV_MODULEDIR[PATH_MAX];
#ifdef WIN32
	GetWindowsDirectory(OV_BASEDIR, MAX_PATH - 14);
	sprintf(OV_BASEDIR, "%s\\%s", OV_BASEDIR, "\\OpenVanilla\\");
	sprintf(OV_USERDIR, "%s\\%s", OV_BASEDIR, "\\User\\");
	sprintf(OV_MODULEDIR, "%s\\%s", OV_BASEDIR, "\\Modules\\");
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
