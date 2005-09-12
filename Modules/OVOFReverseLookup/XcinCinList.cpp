// XcinCinList.cpp

#define OV_DEBUG
#include "OpenVanilla.h"
#include "OVUtility.h"

#ifndef WIN32
#include <sys/param.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif

#include <string.h>
#include <stdlib.h>
#include "OVCIN.h"
#include "XcinCinList.h"

#ifdef WIN32
#include <windows.h>
#endif

const int vxPreparseSuppress=100;   // max. lines to be read for preparser

#ifndef WIN32
#ifdef __linux__
   int file_select(const struct dirent *entry)
#else
   int file_select(struct dirent *entry)
#endif
{
    char *selectfilter = ".cin";
    int p=strlen(entry->d_name)-strlen(selectfilter);
    if (p<0) return 0;
    if (!strcmp(entry->d_name+p, selectfilter)) return 1; 
    return 0;
}
#else
int file_select(WIN32_FIND_DATA entry)
{
    char *selectfilter = ".cin";
    int p=strlen(entry.cFileName)-strlen(selectfilter);
    if (p<0) return 0;
    if (!strcmp(entry.cFileName+p, selectfilter)) return 1; 
    return 0;
}
#endif


CinList::CinList() {
    index=0;
}

#ifndef WIN32
void CinList::load(char *libpath)
{
    if (index)  return;     // already loaded by another binary instance!

    struct dirent **files;
    strcpy(cinpath, libpath);
    int l=strlen(cinpath);
    if (l) if (cinpath[l-1]=='/') cinpath[l-1]=0;
    strcat(cinpath, "/OVOFReverseLookup");

    int count=scandir(cinpath, &files, file_select, alphasort); 
    if (count<=0) return;
    
    for (int i=0; i<count; i++)
    {
        if (index<vxMaxCINFiles)
            if (preparse(cinpath, files[i]->d_name, index)) index++;
        free(files[i]);
    }
    free(files);
    // murmur("CinList::load called, index=%d", index);
}
#else
void CinList::load(char *libpath)
{
    if (index)  return;     // already loaded by another binary instance!

    WIN32_FIND_DATA FileData;
    BOOL fFinished;
    HANDLE hList;
    char findpath[MAX_PATH];

    strcpy(cinpath, libpath);
    strcpy(findpath, libpath);
    int l=strlen(cinpath);
    if (l) if (cinpath[l-1]=='\\') cinpath[l-1]=0;
    strcat(cinpath, "\\OVOFReverseLookup");
    strcat(findpath, "\\OVOFReverseLookup\\*");
    hList = FindFirstFile(findpath, &FileData);

    if(hList == INVALID_HANDLE_VALUE)
    {
	    printf("No files found\n");
	    return;
    }
    else
    {
	    fFinished = FALSE;
	    while (!fFinished)
	    {
		    if (index<vxMaxCINFiles)
			    if (preparse(cinpath, FileData.cFileName, index)) index++;
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
    // murmur("CinList::load called, index=%d", index);
}
#endif

int CinList::preparse(char *loadpath, char *fname, int i)
{
    char filename[PATH_MAX];
#ifndef WIN32
    sprintf(filename, "%s/%s", loadpath, fname);
#else
    sprintf(filename, "%s\\%s", loadpath, fname);
#endif
    
    FILE *in=fopen(filename, "r");
    if (!in) return 0;

	strcpy(list[i].filename, fname);
    strcpy(list[i].cname, fname);
    strcpy(list[i].ename, fname);

    char buf[256];
    int line=0;

    while (!feof(in))
    {
        char key[256], value[256];
        
        fgets(buf, 255, in);
        if (buf[0]=='#') continue;
        
        sscanf(buf, "%s %s", key, value);

		if (!strcmp(key, "%ename")) strcpy(list[i].ename, value);
		if (!strcmp(key, "%cname")) strcpy(list[i].cname, value);
        line++;
        if (line >= vxPreparseSuppress) break;
    }
   
    fclose(in);
	
    return 1;
}
