// VXLoadableIM.cpp

#define OVDEBUG

#include <sys/param.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "VXLoadableIM.h"

void VXLoadableLibrary::clear()
{
	strcpy (path, "");
    libhandle=NULL;
    availableim=0;
    unloadable=0;
    imnew=NULL;
    imdelete=NULL;
}
    
void VXLoadableLibrary::load(char *libpath, char *workpath)
{
	murmur("VXLoadableLibrary::load, libpath=%s, working path=%s", 
		libpath, workpath);
    
    strcpy(path, libpath);
    if (!(libhandle=dlopen(path, RTLD_LAZY)))
    {
    	murmur ("Load failed, dlopen errmsg=%s", dlerror());
    	throw vxExceptionCannotLoad;
    	return;
    }

    imnew=(OVLNewType*)dlsym(libhandle, "OVLoadableNewIM");
    imdelete=(OVLDeleteType*)dlsym(libhandle, "OVLoadableDeleteIM");
    OVLVersionType *ver=(OVLVersionType*)dlsym(libhandle, "OVLoadableVersion");
    OVLAvailableType *avl=(OVLAvailableType*)
    	dlsym(libhandle, "OVLoadableAvailableIMCount");
    OVLUnloadType *unl=(OVLUnloadType*)dlsym(libhandle, "OVLoadableCanUnload");
        
    if (!imnew || !imdelete || !ver) 
    {
    	murmur ("Required interface functions missing, loading failed.");
    	throw vxExceptionCannotLoad;
    	return;
    }
        
    version=ver();
    murmur ("Loaded library version=%08x", version);
    
    if (version < ovVersion)
    {
    	murmur ("Current OV version=%08x, library too old, loading aborted");
    	throw vxExceptionCannotLoad;
    	return;
    }
    
    if (unl) unloadable=unl();
    if (avl) availableim=avl(workpath);	else availableim=1;
}
    
void VXLoadableLibrary::unload()
{
	murmur ("VXLoadableLibrary::unload unloading library %s", path);
	
	if (!unloadable)
	{
		murmur ("Library not unloadable, returned without error.");
		return;
	}
		
	int r=dlclose(libhandle);
	const char *msg=dlerror();
	
	murmur ("System function dlclose() returns %d, errmsg=%s", r, msg);
	
	if (r || msg) throw vxExceptionCannotUnload;

	clear();	
}

void VXLibraryList::add(char *libpath, char *workpath)
{
	if (libcntr == vxMaxLibrary)
	{
		murmur ("VXLibraryList::add() full");
	}

	char wpathbuf[PATH_MAX];
	if (workpath)
	{
		strcpy (wpathbuf, workpath);
	}
	else
	{
		// strip the last part of libpath
		strcpy (wpathbuf, libpath);
		int i=0;
    	for (i=strlen(wpathbuf)-1; i>0; i--) if (wpathbuf[i]=='/') break;
    	wpathbuf[i+1]=0;
	}
	
	VXLoadableLibrary *lib=&liblist[libcntr];
	try
	{
		lib->load(libpath, wpathbuf);
	}
	catch(OVException e)
	{
		murmur ("VXLibraryList::add() exception=%d, returned", e);
		return;
	}
	
	libcntr++;
	for (int i=0; i< lib->availableim; i++)
	{
		if (imcntr == vxMaxIM)
		{
			murmur ("VXLibraryList::add() IM-pair list full"); 
			return;
		}
		
		OVInputMethod *im=impair[imcntr].im=lib->imnew(i);
		im->identifier(impair[imcntr].id);
		impair[imcntr].lib=lib;
		impair[imcntr].imid=i;
		
		murmur ("VXLibraryList::add() loaded IM, imid=%d, IM name=%s",
				i, impair[imcntr].id);	
		imcntr++;
		murmur ("imcntr now=%d", imcntr);
	}
	murmur ("libcntr now=%d", libcntr);
}

OVInputMethod *VXLibraryList::find(char *id)
{
	for (int i=0; i<imcntr; i++)
		if (!strcmp(id, impair[i].id)) return impair[i].im;
	
	return NULL;
}

int VXLibraryList::findPos(char *id)
{
	for (int i=0; i<imcntr; i++)
		if (!strcmp(id, impair[i].id)) return i;
	
	return -1;
}


char selectfilter[PATH_MAX];

int file_select(struct dirent *entry)
{
    int p=strlen(entry->d_name)-strlen(selectfilter);
    if (p<0) return 0;
    if (!strcmp(entry->d_name+p, selectfilter)) return 1; 
    return 0;
}

void VXLibraryList::addglob(char *libpath, char *ext)
{
    struct dirent **files;
    char searchpath[PATH_MAX];

	if (ext) strcpy(selectfilter, ext); else strcpy(selectfilter, ".dylib");

	strcpy(searchpath, libpath);
	int l=strlen(searchpath);
	if (l) if (searchpath[l-1]=='/') searchpath[l-1]=0;

	murmur ("VXLibraryList::addglob, path=%s, ext=%s", searchpath, ext);    
    int count=scandir(searchpath, &files, file_select, alphasort); 
    if (count<=0) return;
    
    for (int i=0; i<count; i++)
    {
    	char libname[PATH_MAX];
    	strcpy (libname, searchpath);
    	strcat (libname, "/");
    	strcat (libname, files[i]->d_name);
    	murmur ("adding library %s", libname);
    	add(libname);
        free(files[i]);
    }
    free(files);
}


