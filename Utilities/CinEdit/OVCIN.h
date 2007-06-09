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
    const char* get_filename(){ return m_shortfilename.c_str(); }
    const char* get_ename(){ return m_ename.c_str(); }
    const char* get_cname(){ return m_cname.c_str(); }
    const char* get_tcname(){ return m_tcname.c_str(); }
    const char* get_scname(){ return m_scname.c_str(); }	
    const char* get_selkey(){ return m_selkey.c_str(); }	
    const char* get_endkey(){ return m_endkey.c_str(); }
	Keyname get_key(size_t i) { return keynames[i];}
	Chardef get_char(size_t i) { return chardefs[i];}	
	size_t keycount() { return keynames.size(); }
	size_t charcount() { return chardefs.size(); }	
protected:
	string m_longfilename;
    string m_shortfilename;
    string m_ename;
    string m_cname;
    string m_tcname;
    string m_scname;
	string m_selkey;
	string m_endkey;
	vector<Keyname> keynames;
	vector<Chardef> chardefs;
};

