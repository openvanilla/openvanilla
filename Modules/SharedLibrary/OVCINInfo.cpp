// OVCINInfo.cpp: a list of .cin information
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// #define OV_DEBUG

#include "OVCINInfo.h"

#ifndef WIN32
    #include <OpenVanilla/OpenVanilla.h>
    #include <OpenVanilla/OVUtility.h>
    #include <sys/param.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <unistd.h>

    #ifdef __linux__
    	#include <stdlib.h>
    #endif
#else   
    #include "OpenVanilla.h"
    #include "OVUtility.h"
	//<comment author='b6s'> Uses OVOSDef.h instead.
	//#define strcasecmp stricmp
	//</comment>
	#include <string.h>	//memset
	#define bzero(addr, num) memset(addr, 0, num)

	#include <windows.h> //FindFirstFile
#endif
#include "OVOSDef.h"

const char *clExtension=NULL;

void CLSplitString(const char *s, string& k, string& v) {
   
    // find first \s
    size_t fs=strcspn(s, " \t");
    
    // find then the non \s
    size_t fl=strspn(s+fs, " \t");
    
    // find until end
    size_t en=strcspn(s+fs+fl, "\n\r");

    string ss(s);
    k=ss.substr(0, fs);
    v=ss.substr(fs+fl, en);    
}

#ifndef WIN32
    #ifdef __linux__
       int CLFileSelect(const struct dirent *entry)
    #else
       int CLFileSelect(struct dirent *entry)
    #endif
    {
        const char *selectfilter = clExtension ? clExtension : ".cin";
        int p=strlen(entry->d_name)-strlen(selectfilter);
        if (p<0) return 0;
        if (!strcmp(entry->d_name+p, selectfilter)) return 1; 
        return 0;
    }
#else
    //#error This part of code won't run--needs refactoring

    int file_select(WIN32_FIND_DATA entry)
    {
        char *selectfilter = ".cin";
        size_t p=strlen(entry.cFileName)-strlen(selectfilter);
        if (p<0) return 0;
        if (!strcmp(entry.cFileName+p, selectfilter)) return 1; 
        return 0;
    }
#endif


OVCINList::OVCINList(const char *pathseparator) {
    pathsep=pathseparator;
}

#ifndef WIN32
    int OVCINList::load(const char *loadpath, const char *extension) {
        clExtension=extension;
    
        struct dirent **files = NULL;
        int count=scandir(loadpath, &files, CLFileSelect, alphasort);        

        int loaded=0;

	if ( count < 0 )
	{
		return loaded; 
	}

        for (int i=0; i<count; i++) {
            if (preparse(loadpath, files[i]->d_name)) loaded++;
            free(files[i]);
        }
		
		if (files) {
			free(files);
		}
		
        return loaded;
    }
#else   
    int OVCINList::load(const char *loadpath, const char *extension) {
		int loaded=0;
		BOOL fFinished;
		HANDLE hList;
		WIN32_FIND_DATA FileData;
		string pathString(loadpath);
		pathString += "\\*";
		pathString += extension;
		const char* findpath = pathString.c_str();
		hList = FindFirstFile(findpath, &FileData);
		if(hList == INVALID_HANDLE_VALUE)
		{
			murmur("No files found in %s\n", pathString.c_str());
		}
		else
		{
			fFinished = FALSE;
			while (!fFinished)
			{
				if(strstr(FileData.cFileName, extension))
				{
					if (preparse(loadpath, FileData.cFileName)) loaded++;
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
		return loaded;
        // murmur("OVCINList::load called, index=%d", index);
    }
#endif

bool OVCINList::preparse(const char *loadpath, const char *filename) {
    // check if a file of the same short name has been alread loaded
    for (size_t i=0; i<list.size(); i++) {
        OVCINInfo &x=list[i];
        if (x.shortfilename==filename) {
            murmur("OVCINList: file %s not loaded, short name '%s' already exists", loadpath, filename);
            return false;
        }
    }


    string longname = string(loadpath) + pathsep + string(filename);

    FILE *in=fopen(longname.c_str(), "r");
    if (!in) {
        murmur("OVCINList: opening %s failed", longname.c_str());
        return false;
    }

    OVCINInfo info;
    info.shortfilename=filename;
    info.longfilename=longname;

    int line=0;
    const size_t bs=2049;
    char buf[bs];
    bzero(buf, bs);	
    
    string k, v;
    
    while (!feof(in))
    {
        fgets(buf, bs-1, in);
        if (buf[0]=='#') continue;      // ignore comment

        CLSplitString(buf, k, v);        
        const char *key=k.c_str();
            
		if (!strcasecmp(key, "%ename")) info.ename=v;
		else if (!strcasecmp(key, "%cname")) info.cname=v;
		else if (!strcasecmp(key, "%tcname")) info.tcname=v;
		else if (!strcasecmp(key, "%scname")) info.scname=v;

        line++;
        if (line >= CL_PREPARSELIMIT) break;
    }
   
    fclose(in);

    // some fallbacks..
    if (!info.ename.length()) info.ename=filename;
    if (!info.cname.length()) info.cname=info.ename;
    if (!info.tcname.length()) info.tcname=info.cname;
    if (!info.scname.length()) info.scname=info.cname;
    list.push_back(info);
	
	murmur("Loaded: longfilename=%s, shortfilename=%s, ename=%s, cname=%s, tcname=%s, scname=%s",
	   info.longfilename.c_str(), info.shortfilename.c_str(), info.ename.c_str(), 
	   info.cname.c_str(), info.tcname.c_str(), info.scname.c_str());
	
    return 1;
}
