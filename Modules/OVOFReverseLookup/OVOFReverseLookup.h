// OVOFReverseLookup.h

#ifndef __OVOFReverseLookup_h
#define __OVOFReverseLookup_h

#ifndef WIN32
#include <OpenVanilla/OpenVanilla.h>
#else
#include "OpenVanilla.h"
#endif
#include "OVCIN.h"
#include "OVKeySequence.h"
#include "OVCandidateList.h"
#include "XcinCinList.h"
#include <string>

using namespace std;

class OVOFReverseLookup : public OVOutputFilter
{
public:
    OVOFReverseLookup(char *lpath, char *cfile, char *en=NULL, char *cn=NULL);
    virtual ~OVOFReverseLookup();
    virtual const char* identifier();
    virtual const char* localizedName(const char* locale);
    virtual int initialize(OVDictionary*, OVService*, const char*);
    virtual const char* process(const char *src, OVService *srv);
    
protected:
    char loadpath[PATH_MAX];
    char cinfile[PATH_MAX];
    OVCIN* cintab;
    
    char ename[128];
    char cname[128];
    char idbuf[128];
};

#endif
