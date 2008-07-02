//#define OV_DEBUG
#include "OVUtility.h"
#include "AVConfig.h"


#if defined(WIN32) || defined(WINCE)
#include <windows.h>
#ifndef WINCE
#include <shlobj.h>
#include <io.h>
#endif
#define PATH_MAX MAX_PATH
#endif


AVConfig::AVConfig()
{
	char OV_BASEDIR[PATH_MAX];
	char OV_USERDIR[PATH_MAX];
	char OV_MODULEDIR[PATH_MAX];
#if defined(WIN32) || defined(WINCE)
	//@warning OV-specific paths here!

	//GetWindowsDirectoryA(OV_BASEDIR, MAX_PATH - 14);
	//sprintf(OV_BASEDIR, "%s\\%s", OV_BASEDIR, "OpenVanilla\\");
#ifdef WINCE
	WCHAR wbuf[MAX_PATH];
	GetModuleFileNameW(GetModuleHandleW(L"OVIME.dll"), wbuf, MAX_PATH);
	//GetModuleFileNameW(NULL, wbuf, MAX_PATH);
	//wcstombs(OV_BASEDIR,wbuf, MAX_PATH);

	WideCharToMultiByte(CP_UTF8, 0, wbuf, (int)(wcslen(wbuf)+1), OV_BASEDIR, MAX_PATH,NULL, NULL);
	//strcpy(OV_BASEDIR,"\\Program Files\\MVsip\\");

	char* lb= strrchr(OV_BASEDIR,'\\');
	OV_BASEDIR[((DWORD)lb-(DWORD)OV_BASEDIR)/sizeof(char) +1]=0; //stripped the filename
	strcpy(OV_USERDIR, OV_BASEDIR);
	strcpy(OV_MODULEDIR, OV_BASEDIR);
#else
	strcpy(OV_BASEDIR, "C:\\Program Files\\OpenVanilla\\");  // Try here first
	if (_access(OV_BASEDIR, 0))			//If not exist, try root location.
		strcpy(OV_BASEDIR, "C:\\OpenVanilla\\");  
	if (_access(OV_BASEDIR, 0))
		murmur("Default base dir does not exsit!!");
	murmur("Base dir at %s", OV_BASEDIR);
	
		
	sprintf(OV_MODULEDIR, "%s\\%s", OV_BASEDIR, "Modules\\");

	if (SHGetFolderPathA(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE, 
				NULL, 0, OV_USERDIR) >= 0) 
	{
		
		sprintf(OV_USERDIR, "%s\\%s", OV_USERDIR, "OpenVanilla\\");

		/* Try to create the directory if it does not yet
		   exists.  */
		if (_access(OV_USERDIR, 0))
			CreateDirectoryA(OV_USERDIR, NULL);
	}
	else
		sprintf(OV_USERDIR, "%s\\%s", OV_BASEDIR, "User\\");

#endif
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
