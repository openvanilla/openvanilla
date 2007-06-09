// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

