// OVXCIN.h

#ifndef __OVXCIN_h
#define __OVXCIN_h

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>

#include <string>
#include <vector>
#include <map>

using namespace std;

class OVXCIN
{
public:
    OVXCIN();
    ~OVXCIN();
    void read(char *fname, int shiftselkey=0);
    vector<string> find(string key);
    
protected:
    map<string, vector<string> > keytable;
    map<string, vector<string> > chartable;
    string selkey;
    string ename;
    string cname;
};

#endif