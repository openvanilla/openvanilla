// XcinCinList.cpp

#define OVDEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>
#include <sys/param.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
//#include "OVCIN.h"
#include "XcinCinList.h"

const int vxPreparseSuppress=100;   // max. lines to be read for preparser

char selectfilter[PATH_MAX];
    
int file_select(struct dirent *entry)
{
    int p=strlen(entry->d_name)-strlen(selectfilter);
    if (p<0) return 0;
    if (!strcmp(entry->d_name+p, selectfilter)) return 1; 
    return 0;
}

void CinList::load(char *libpath)
{
    struct dirent **files;
    char searchpath[PATH_MAX];

    strcpy(selectfilter, ".cin");

    strcpy(searchpath, libpath);
    int l=strlen(searchpath);
    if (l) if (searchpath[l-1]=='/') searchpath[l-1]=0;

    strcpy(cinpath, searchpath);

    int count=scandir(searchpath, &files, file_select, alphasort); 
    if (count<=0) return;
    
    for (int i=0; i<count; i++)
    {
        if (index<vxMaxCINFiles)
            if (preparse(searchpath, files[i]->d_name, index)) index++;
        free(files[i]);
    }
    free(files);
}


int CinList::preparse(char *loadpath, char *fname, int i)
{
    char filename[PATH_MAX];
    sprintf(filename, "%s/%s", loadpath, fname);
    
    FILE *in=fopen(filename, "r");
    if (!in) return 0;

    strcpy(list[i].filename, fname);
    strcpy(list[i].cname, fname);
    strcpy(list[i].ename, fname);
    list[i].encoding=ovEncodingUTF8;

    char buf[256];
    int line=0;

    while (!feof(in))
    {
        char key[256], value[256];
        
        fgets(buf, 255, in);
        if (buf[0]=='#') continue;
        
        sscanf(buf, "%s %s", key, value);

        if (!strcasecmp(key, "%ename")) strcpy(list[i].ename, value);
        if (!strcasecmp(key, "%encoding")) list[i].encoding=ovEncodingUTF8;
        if (!strcasecmp(key, "%cname")) strcpy(list[i].cname, value);
        
        line++;
        if (line >= vxPreparseSuppress) break;
    }
    
    fclose(in);
    return 1;
}