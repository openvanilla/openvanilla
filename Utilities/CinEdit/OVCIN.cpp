/*
 *  OVCIN.cpp
 *  CinEdit
 *
 *  Created by zonble on 2007/6/6.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "OVCIN.h"


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

bool OVCIN::load(const char *loadpath, const char *filename) {
    // check if a file of the same short name has been alread loaded	
	
    string longname = string(loadpath) + "/" + string(filename);
	
    FILE *in=fopen(longname.c_str(), "r");
	bool c_keyname = 0;
	bool c_chardef = 0;
	
    m_shortfilename=filename;
    m_longfilename=longname;
	
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
		
		if (!strcasecmp(key, "%ename")) m_ename=v;
		else if (!strcasecmp(key, "%cname")) m_cname=v;
		else if (!strcasecmp(key, "%tcname")) m_tcname=v;
		else if (!strcasecmp(key, "%scname")) m_scname=v;
		else if (!strcasecmp(key, "%selkey")) m_selkey=v;	
		else if (!strcasecmp(key, "%endkey")) m_endkey=v;
		
		if (!strcasecmp(key, "%keyname")) {
			if(!strcasecmp(v.c_str(), "begin")) {
				c_keyname = 1;
				continue;				
			} else if(!strcasecmp(v.c_str(), "end")) {
				c_keyname = 0;
				continue;				
			}				
		}

		if (!strcasecmp(key, "%chardef")) {
			if(!strcasecmp(v.c_str(), "begin")) {
				c_chardef = 1;
				continue;
			} else if(!strcasecmp(v.c_str(), "end")) {
				c_chardef = 0;
				continue;				
			}				
		}

		if(c_keyname) {
			Keyname my_k;
			my_k.key = k;
			my_k.value = v;
			keynames.push_back(my_k);
		}
		if(c_chardef) {
			Chardef my_c;
			my_c.key = k;
			my_c.value = v;
			chardefs.push_back(my_c);			
		}
		
        line++;
        //if (line >= CL_PREPARSELIMIT) break;
    }
	
    fclose(in);
	
    // some fallbacks..
    if (!m_ename.length()) m_ename=filename;
    if (!m_cname.length()) m_cname=m_ename;
    if (!m_tcname.length()) m_tcname=m_cname;
    if (!m_scname.length()) m_scname=m_cname;
    return 1;
}
