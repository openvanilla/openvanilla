/*
 *  OVCIN.h
 *  CinEdit
 *
 *  Created by zonble on 2007/6/6.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include <vector>

using namespace std;
const int CL_PREPARSELIMIT=32; 

struct Keyname {
	string key;
	string value;
};

struct Chardef {
	string key;
	string value;
};

class OVCIN
{
public:
    OVCIN() {};
    bool load(const char *loadpath, const char *filename);	
    const char* get_filename(){ return shortfilename.c_str(); }
    const char* get_ename(){ return ename.c_str(); }
    const char* get_cname(){ return cname.c_str(); }
    const char* get_tcname(){ return tcname.c_str(); }
    const char* get_scname(){ return scname.c_str(); }	
    const char* get_selkey(){ return selkey.c_str(); }	
    const char* get_endkey(){ return endkey.c_str(); }
	Keyname get_key(size_t i) { return keynames[i];}
	Chardef get_char(size_t i) { return chardefs[i];}	
	size_t keycount() { return keynames.size(); }
	size_t charcount() { return chardefs.size(); }	
protected:
	string longfilename;
    string shortfilename;
    string ename;
    string cname;
    string tcname;
    string scname;
	string selkey;
	string endkey;
	vector<Keyname> keynames;
	vector<Chardef> chardefs;
};

