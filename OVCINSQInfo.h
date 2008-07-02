#ifndef __OVCINSQList_h
#define __OVCINSQList_h

#include <string>
#include <vector>
#include "OVSQLite3.h"
using namespace std;

const int CL_PREPARSELIMIT=32;      // preparse won't exceed first 64 lines

struct OVCINSQInfo 
{
	OVCINSQInfo() {}
	OVCINSQInfo(const OVCINSQInfo &c) :
		longfilename(c.longfilename), shortfilename(c.shortfilename),
        ename(c.ename), cname(c.cname), tcname(c.tcname), scname(c.scname), 
		dwLowTimeStamp(c.dwLowTimeStamp), dwHighTimeStamp(c.dwHighTimeStamp) {}
    const OVCINSQInfo& operator=(const OVCINSQInfo& c) {
        longfilename=c.longfilename;
        shortfilename=c.shortfilename;
        ename=c.ename;
        cname=c.cname;
        tcname=c.tcname;
        scname=c.scname;
		dwLowTimeStamp=c.dwLowTimeStamp;
		dwHighTimeStamp=c.dwLowTimeStamp;
        return *this;
    }

	string longfilename;
    string shortfilename;
    string ename;
    string cname;
    string tcname;
    string scname;

	int dwLowTimeStamp;
	int dwHighTimeStamp;

};

class OVCINSQList //: public OVCINList
{
public:
	OVCINSQList(const char *pathseparator);
    int load(const char *loadpath, const char *extension=".cin");

	int loadfromdb(SQLite3 *db);
	

    size_t count() { return list.size(); }
    const OVCINSQInfo& cinInfo(size_t i) { return list[i]; }
    
	//OVCINSQInfo* assoc;
protected:
    bool preparse(const char *loadpath, const char *filename);

    string pathsep;
    vector<OVCINSQInfo> list;
	
	
};

#endif


