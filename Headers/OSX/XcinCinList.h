// XcinCinList.h

#ifndef __XcinCinList_h
#define __XcinCinList_h

struct CinPair
{
    CinPair()
    {
        filename[0]=ename[0]=cname[0]=0;
        encoding=ovEncodingUTF8;
    }
    
    char filename[PATH_MAX];
    char ename[64];
    char cname[128];
    OVEncoding encoding;
};

const int vxMaxCINFiles=64;

class CinList : public OVObject
{
public:
    void load(char *libpath);    
    int preparse(char *loadpath, char *fname, int i);
    int index;
    CinPair list[vxMaxCINFiles];
    char cinpath[PATH_MAX];
};

#endif