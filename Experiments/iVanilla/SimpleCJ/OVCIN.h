// OVCIN.h: .cin processor
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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

#ifndef __OVCIN_h
#define __OVCIN_h

#ifndef WIN32
    #include <OpenVanilla/OpenVanilla.h>  
    #include <OpenVanilla/OVUtility.h>  
#else
    #include "OpenVanilla.h"
    #include "OVUtility.h"
#endif

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace _OVCIN{
    const int NUM_PROPERTY = 7;
    const int NUM_MAP = 2;
    enum EnumProperty{
        P_SELKEY=0, P_ENAME, P_CNAME, P_TCNAME, P_SCNAME, P_ENDKEY, P_ENCODING
    };
    enum EnumMap {
        M_KEY, M_CHAR
    };
    enum State {
        PARSE_BLOCK, PARSE_LINE
    };

    template<class T1, class T2>
    struct CmpPair {
        typedef pair<T1, T2> T;
        bool operator() (const T& a, const T& b){
            return a.first < b.first;
        }
    };

    typedef CmpPair<string, string> cmpBlockEntry;
    typedef CmpPair<string, vector<string> > cmpMapEntry;
};

class OVCIN
{
private:
    typedef vector< pair<string, vector<string> > > CinMap;
    _OVCIN::State state;
    string delimiters;
    string properties[_OVCIN::NUM_PROPERTY];
    vector< pair<string, string > > block_buf;
    CinMap maps[_OVCIN::NUM_MAP];
    int curMapIndex;

public:    
    // VXCIN::read()
    OVCIN(const char* fileName);
    
    // VXCIN::selKey()
    inline string& getSelKey();
    
    // VXCIN::name()
    inline string& getCName();
    
    // VXCIN::name()
    inline string& getEName();
    
    inline string& getEndKey();
    
    inline string& getEncoding();
    
    // VXCIN::isEndKey()
    inline bool isEndKey(char keyChar);
    
    inline bool isValidKey(const string& keyString) const;
    
    // VXCIN::getKey()
    inline size_t getCharVectorByKey(const string& inKey, vector<string>& outStringVectorRef);
    
    // VXCIN::find()
    inline size_t getWordVectorByChar(const string& inKey, vector<string>& outStringVectorRef);
	inline size_t getWordVectorByCharWithWildcardSupport(const string& inKey, vector<string>& outStringVectorRef, char matchOneChar = 0, char matchZeroOrMoreChar = 0);

    void show(const CinMap &m, int x);
    void runtest(const string &s);
    void test();

protected:
    size_t getVectorFromMap(const CinMap& inMapRef,
                         const string& inKey,
                         vector<string>& outStringVectorRef);
	size_t getVectorFromMapWithWildcardSupport(const CinMap& inMapRef,const string& inKey, vector<string>& outStringVectorRef, char matchOneChar = 0, char matchZeroOrMoreChar = 0);
					
    void lowerStr(string& str);
    void parseCinVector(const vector<string>& cinVector);
    void setBlockMap();
    int setProperty(const string& key, const string& value);
    void insertMap(const string& line);
    int searchCinMap(const CinMap& m, const string& key) const;
    
    
    const pair<int, int> findRangeStartingWith(const CinMap& m,
        const string& key) const;
    int findClosestUpperBound(const CinMap& m, const string& key) const;
    int findClosestLowerBound(const CinMap& m, const string& key) const;
};


string& OVCIN::getSelKey()
{
    return properties[_OVCIN::P_SELKEY];
}

string& OVCIN::getCName()
{
    return properties[_OVCIN::P_CNAME];
}

string& OVCIN::getEName()
{
    return properties[_OVCIN::P_ENAME];
}

string& OVCIN::getEncoding()
{
    return properties[_OVCIN::P_ENCODING];
}

string& OVCIN::getEndKey()
{
    return properties[_OVCIN::P_ENDKEY];
}

bool OVCIN::isEndKey(char keyChar)
{
	string::size_type pos = getEndKey().find(keyChar, 0);
	if(pos == string::npos)
        return false;
    else
        return true;
}

bool OVCIN::isValidKey(const string& keyString) const
{
    if( searchCinMap( maps[_OVCIN::M_KEY], keyString ) != -1 )
        return true;
    else
        return false;
}

size_t OVCIN::getCharVectorByKey(const string& inKey,
                              vector<string>& outStringVectorRef)
{
    return getVectorFromMap(maps[_OVCIN::M_KEY], inKey, outStringVectorRef);
}

size_t OVCIN::getWordVectorByChar(const string& inKey,
                               vector<string>& outStringVectorRef)
{
    return getVectorFromMap(maps[_OVCIN::M_CHAR], inKey, outStringVectorRef);
}

size_t OVCIN::getWordVectorByCharWithWildcardSupport(const string& inKey, vector<string>& outStringVectorRef, char matchOneChar, char matchZeroOrMoreChar)
{
	string::size_type i, len = inKey.length();
	for (i = 0; i < len; i++) {
		if (inKey[i] == matchOneChar || inKey[i] == matchZeroOrMoreChar)
			return getVectorFromMapWithWildcardSupport(maps[_OVCIN::M_CHAR], inKey, outStringVectorRef, matchOneChar, matchZeroOrMoreChar);
	}
	
	return getVectorFromMap(maps[_OVCIN::M_CHAR], inKey, outStringVectorRef);
}


#endif
