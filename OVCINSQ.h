// OVCINSQ.h: .cin processor
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
//#define OV_DEBUG
#ifndef __OVCINSQ_h
#define __OVCINSQ_h

#if !defined(WIN32) && !defined(WINCE)
    #include <OpenVanilla/OpenVanilla.h>  
    #include <OpenVanilla/OVUtility.h>  
#else
    #include "OpenVanilla.h"
    #include "OVUtility.h"
#endif

#include <string>
#include <vector>
#include <map>
//#include <locale>
#include "OVSQLite3.h"
#include "OVCINSQInfo.h"

using namespace std;




namespace _OVCINSQ{
    const int NUM_PROPERTY = 7;
    const int NUM_MAP = 2;
    enum EnumProperty{
        P_SELKEY=0, P_ENAME, P_CNAME, P_TCNAME, P_SCNAME, P_ENDKEY, P_ENCODING
    };
    enum EnumMap {
        M_KEY, M_CHAR
    };
	static const char *propertyNames[NUM_PROPERTY] = {
    "selkey", "ename", "cname", "tcname", "scname", "endkey", "encoding" 
	};

	static const char *mapNames[NUM_MAP] = {
    "keyname", "chardef"
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



class OVCINSQ
{
private:
    typedef vector< pair<string, vector<string> > > CinMap;
    _OVCINSQ::State state;
    string delimiters;
    string properties[_OVCINSQ::NUM_PROPERTY];
    vector< pair<string, string > > block_buf;
    CinMap maps[_OVCINSQ::NUM_MAP];
    int curMapIndex;
//	locale m_locale;
	
	SQLite3 *db;
	std::string tablename;

public:    
	

    // VXCIN::read()
    //OVCINSQ(const char* fileName);
	OVCINSQ(const OVCINSQInfo& cininfo, SQLite3 *dbfile);
    
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
    
    // VXCIN::getKey()  //inline statement removed
    size_t getCharVectorByKey(const string& inKey, vector<string>& outStringVectorRef);
    
    // VXCIN::find()  //inline statement removed
    size_t getWordVectorByChar(const string& inKey, vector<string>& outStringVectorRef, bool smartorder=false);
	size_t getWordVectorByCharWithWildcardSupport(const string& inKey, vector<string>& outStringVectorRef, 
		char matchOneChar = 0, char matchZeroOrMoreChar = 0, bool smartorder = false);

	size_t getCharVectorByWord(const string& inWord, vector<string>& outStringVectorRef);

	// Associate phrase
	size_t getAssociatedPhrases(const string& inWord, vector<string>& outStringVectorRef);

	// PhraseUserFreq
	int updatePhraseUserFrequency(const char* phrase);


    void show(const CinMap &m, int x);
    void runtest(const string &s);
    void test();

protected:
    
					
    void lowerStr(string& str);
    void parseCinVector(const vector<string>& cinVector);
    void setBlockMap();
    int setProperty(const string& key, const string& value);
    void insertMap(const string& line);

	/*
	size_t getVectorFromMap(const CinMap& inMapRef,
                         const string& inKey,
                         vector<string>& outStringVectorRef);
	size_t getVectorFromMapWithWildcardSupport(const CinMap& inMapRef,const string& inKey, vector<string>& outStringVectorRef, char matchOneChar = 0, char matchZeroOrMoreChar = 0);
    
	int searchCinMap(const CinMap& m, const string& key) const;
    
    
    const pair<int, int> findRangeStartingWith(const CinMap& m,
        const string& key) const;
    int findClosestUpperBound(const CinMap& m, const string& key) const;
    int findClosestLowerBound(const CinMap& m, const string& key) const;

	*/
};




string& OVCINSQ::getSelKey()
{
	return properties[_OVCINSQ::P_SELKEY];
}

string& OVCINSQ::getCName()
{
	return properties[_OVCINSQ::P_CNAME];
}

string& OVCINSQ::getEName()
{
	return properties[_OVCINSQ::P_ENAME];
}

string& OVCINSQ::getEncoding()
{
	return properties[_OVCINSQ::P_ENCODING];
}

string& OVCINSQ::getEndKey()
{
	return properties[_OVCINSQ::P_ENDKEY];
}

bool OVCINSQ::isEndKey(char keyChar)
{
	string::size_type pos = getEndKey().find(keyChar, 0);
	if(pos == string::npos)
        return false;
    else
        return true;
}

bool OVCINSQ::isValidKey(const string& keyString) const
{
	SQLite3Statement *sth=db->prepare(
		"select value from '%q' where key = '_key_%q';", tablename.c_str(), keyString.c_str()); 
	if (sth && sth->step()==SQLITE_ROW) {
		delete sth;
		return true;
		
	}
	else return false;

	/*
    if( searchCinMap( maps[_OVCINSQ::M_KEY], keyString ) != -1 )
        return true;
    else
        return false;
		*/
}



#endif
