// VXLoadableIM.h

#ifndef __VXLoadableIM_h
#define __VXLoadableIM_h

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLoadable.h>
#include <OpenVanilla/OVUtility.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>

enum
{
	ovExceptionLoader=0x100,
	vxExceptionCannotLoad=ovExceptionLoader+1,
	vxExceptionCannotUnload=ovExceptionLoader+2
};

class VXLoadableLibrary
{
public:
	VXLoadableLibrary() { clear(); }

	void load(char *libpath, char *workpath);
	void unload();
	void clear();

    char path[PATH_MAX];
    void *libhandle;
    int availableim;
    int unloadable;
    unsigned int version;
    OVLNewType *imnew;
    OVLDeleteType *imdelete;
};

class VXLibraryIMPair
{
public:
	VXLibraryIMPair() : im(NULL), lib(NULL), imid(0), inited(0) { *id=0; }
	
	OVInputMethod *im;
	VXLoadableLibrary *lib;
	int imid;
	int inited;
	char id[ovMaxIdentifierLength];
};

const int vxMaxLibrary=16;
const int vxMaxIM=32;

class VXLibraryList
{
public:
	VXLibraryList() : libcntr(0), imcntr(0) {}
	
	void add(char *libpath, char *workpath=NULL);
	void addglob(char *libpath, char *ext=NULL);
	OVInputMethod* find(char *id);
	int findPos(char *id);
	OVInputMethod* getFirst() { return impair[0].im; }
	
	int libcntr;
	VXLoadableLibrary liblist[vxMaxLibrary];
	
	int imcntr;
	VXLibraryIMPair impair[vxMaxIM];
};

#endif
