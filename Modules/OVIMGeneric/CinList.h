// XcinCinList.h

#ifndef __XcinCinList_h
#define __XcinCinList_h
#ifdef WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#endif

struct CinPair
{
    CinPair()
    {
        filename[0]=ename[0]=cname[0]=0;
    }
    
    char filename[PATH_MAX];
    char ename[64];
    char cname[128];
};

const int vxMaxCINFiles=64;

class CinList : public OVBase
{
public:
    CinList();
    void load(char *libpath);    
    int preparse(char *loadpath, char *fname, int i);
    int index;
    CinPair list[vxMaxCINFiles];
    char cinpath[PATH_MAX];
};

#endif
