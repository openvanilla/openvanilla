// VXConfig.h

#ifndef __VXConfig_h
#define __VXConfig_h

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include "VXDictionary.h"

enum
{
    vxcExceptionFile=1
};

typedef struct stat CLibStat;
typedef struct timespec CLibTimeSpec;

class VXConfig
{
public:
    VXConfig(const char *fn)
    {
        bzero(&timestamp, sizeof(timestamp));
        strcpy(filename, fn);
        url=VXCreateURL(filename);
        if (!url) throw ovExceptionMemory;
        dictroot=new VXDictionary;
        read();
    }
    
    ~VXConfig()
    {
        CFRelease(url);
        delete dictroot;
    }
    
    int changed()
    {
        CLibStat fstat;
        if (stat(filename, &fstat)) return 0;        
        if (!memcmp(&timestamp, &(fstat.st_mtimespec), sizeof(timestamp))) return 0;
        return 1;
    }
    
    int read()
    {
        if (!changed()) return 1;       
        if (!dictroot->readFromURL(url)) return 0;
        updatetimestamp();
        return 1;
    }
    
    int write()
    {
        if (!dictroot->writeToURL(url)) return 0;
        updatetimestamp();
        return 1;        
    }
    
    VXDictionary* getDictionaryRoot() { return dictroot; }
    
protected:
    void updatetimestamp()
    {
        CLibStat fstat;
        if (stat(filename, &fstat)) return;
        timestamp=fstat.st_mtimespec;
    }
    
    VXDictionary *dictroot;
    char filename[PATH_MAX];
    CFURLRef url;    
    CLibTimeSpec timestamp;
};

#endif
