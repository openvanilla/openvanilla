// OVCINInfo.h: a list of .cin information
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
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

#ifndef __OVCINList_h
#define __OVCINList_h

#include <string>
#include <vector>

using namespace std;

const int CL_PREPARSELIMIT=32;      // preparse won't exceed first 64 lines

struct OVCINInfo {
    OVCINInfo() {}
    OVCINInfo(const OVCINInfo &c) : 
        longfilename(c.longfilename), shortfilename(c.shortfilename),
        ename(c.ename), cname(c.cname), tcname(c.tcname), scname(c.scname) {}
    const OVCINInfo& operator=(const OVCINInfo& c) {
        longfilename=c.longfilename;
        shortfilename=c.shortfilename;
        ename=c.ename;
        cname=c.cname;
        tcname=c.tcname;
        scname=c.scname;
        return *this;
    }
    
    string longfilename;
    string shortfilename;
    string ename;
    string cname;
    string tcname;
    string scname;
};

class OVCINList
{
public:
    OVCINList(const char *pathseparator);
    int load(const char *loadpath, const char *extension=".cin");

    size_t count() { return list.size(); }
    const OVCINInfo& cinInfo(size_t i) { return list[i]; }
    
protected:
    bool preparse(const char *loadpath, const char *filename);

    string pathsep;
    vector<OVCINInfo> list;
};

#endif
