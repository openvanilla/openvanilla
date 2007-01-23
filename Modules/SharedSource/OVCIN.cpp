// OVCIN.h: .cin processor
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

//#define  OV_DEBUG
#include "OVCIN.h"
#include "OVFileHandler.h"

#include <algorithm>	//for stable_sort() and transform()

using namespace std;
using namespace _OVCIN;


static const char *propertyNames[NUM_PROPERTY] = {
    "selkey", "ename", "cname", "tcname", "scname", "endkey", "encoding" 
};

static const char *mapNames[NUM_MAP] = {
    "keyname", "chardef"
};

OVCIN::OVCIN(const char* fileName)
{
    OVFileHandler* fileHandler = new OVFileHandler(fileName);
    vector<string> stringVector;
    fileHandler->getLines(stringVector);
    delete fileHandler;
    
    state = PARSE_LINE;
    delimiters = " \t";
    parseCinVector(stringVector);
}

int OVCIN::setProperty(const string& key, const string& value){
    const char* name = key.c_str() + 1;
    if( value == "begin" ){
        state = PARSE_BLOCK;
        for(int i=0;i<NUM_MAP;i++)
            if( !strcmp( name, mapNames[i] ) ){
                curMapIndex = i;
                break;
            }
        return 1;
    }
    else if( value == "end" ){
        state = PARSE_LINE;
        setBlockMap();
    }
    else{
        for(int i=0;i<NUM_PROPERTY;i++)
            if( !strcmp( name, propertyNames[i] ) ){
                murmur("[%s]: [%s]", name, value.c_str());
                properties[i] = value;
                break;
            }
    }
    return 0;
}

void OVCIN::setBlockMap(){
    vector< pair<string, string> >::const_iterator it;
    stable_sort(block_buf.begin(), block_buf.end(), cmpBlockEntry());
    CinMap &curMap = maps[curMapIndex];
    for(it = block_buf.begin(); it != block_buf.end(); ++it)
        if( !curMap.empty() && curMap.back().first == it->first )
            curMap.back().second.push_back(it->second);
        else{
            vector<string> v;
            v.push_back(it->second);
            curMap.push_back( make_pair( it->first, v) );
        }
    block_buf.clear();
}

void OVCIN::parseCinVector(const vector<string>& cinVector){
    vector<string>::const_iterator it;
    for(it = cinVector.begin(); it != cinVector.end(); ++it){
        if( it->find("#") == 0 && !state == PARSE_BLOCK)    continue;
        const string& line = *it;
        string::size_type del_pos;
        if( (del_pos=line.find_first_of(delimiters)) != string::npos ){
            string key = line.substr(0, del_pos);
            unsigned int value_pos=(unsigned int)line.find_first_not_of(delimiters, del_pos);
            if( value_pos == string::npos )
                continue;
            string value = line.substr( value_pos, line.length() - value_pos );
			int isBlockBegin = 0;
            if(key.find("%") == 0)
                isBlockBegin = setProperty(key, value);
            if(state == PARSE_BLOCK && !isBlockBegin){
                lowerStr(key);
                block_buf.push_back( make_pair(key, value) );
            }
        }
    }
}

void OVCIN::lowerStr(string& str){
    for(int i=static_cast<int>(str.length())-1; i>=0; i--)
        if( !isprint(str[i]) )
            return;
    transform( str.begin(), str.end(), str.begin(),(int(*)(int)) tolower );
}


size_t OVCIN::getVectorFromMap(const CinMap& inMapRef,
                            const string& inKey,
                            vector<string>& outStringVectorRef)
{
    int pos;
    murmur("getVectorFromMap: %s", inKey.c_str());

    if( (pos=searchCinMap( inMapRef, inKey )) != -1) {
        murmur("search ret: %s", inMapRef[pos].first.c_str());
        outStringVectorRef = inMapRef[pos].second;
        return outStringVectorRef.size();
    }
    else
        outStringVectorRef.clear();
    return 0;    
}
 
int OVCIN::searchCinMap(const CinMap& m, const string& key) const{
    int mid, low = 0, high = static_cast<int>(m.size()) - 1;
    while(low <= high){
        mid = (low + high) / 2;
        if( key == m[mid].first )
          return mid;
        else if( key < m[mid].first )
            high = mid -1;
        else
            low = mid + 1;
    }
    return -1;
}


const pair<int, int> OVCIN::findRangeStartingWith(const CinMap& m,
    const string& key) const
{
    pair<int, int> r;
    r.second=-1;
    
    r.first = findClosestUpperBound(m, key);
    if (r.first == -1) return r;
    
    r.second = findClosestLowerBound(m, key);
    if (r.second == -1) r.second=static_cast<int>(m.size());
    r.second--;
    
    if (r.first > r.second) r.first=r.second=-1;
    return r;
}


int OVCIN::findClosestUpperBound(const CinMap& m, const string& key) const{
    int mid, low = 0, high = static_cast<int>(m.size())-1;

    while(low <= high){
        mid = (low + high) / 2;

        if( key == m[mid].first )
          return mid;
          
        if (mid > 0) {
            if (key > m[mid-1].first && key <= m[mid].first) return mid;
        }

        if( key < m[mid].first )
            high = mid -1;
        else
            low = mid + 1;
    }
    return -1;
}

int OVCIN::findClosestLowerBound(const CinMap& m, const string& key) const{
    string newkey=key;
    size_t l=newkey.length();
    if (l) {
        if ((unsigned char)newkey[l-1] != 0xff)
            newkey[l-1] = (unsigned char)newkey[l-1] +1;
    }
    
    return findClosestUpperBound(m, newkey);
}




// discarded test routines
/*
void OVCIN::show(const CinMap &m, int x) {
    if (x==-1) {
        printf ("    not found\n");
        return;
    }
    
    int size=m.size();
    printf ("    %d: ", x);
    if (x > 0) {
        printf ("[%d: %s >>> (%d: %s)", x-1, m[x-1].first.c_str(), x, m[x].first.c_str());
    }
    else {
        printf ("(%d: %s)", x, m[x].first.c_str());
    }

    if (x < size-1) {
        printf (" <<< %d: %s]\n", x+1, m[x+1].first.c_str());
    }    
    else {
        printf ("\n");
    }
}

void OVCIN::runtest(const string &s) {
    CinMap &m=maps[_OVCIN::M_CHAR];

    pair<int, int> p=findRangeStartingWith(m, s);
    
    printf ("key=%s, range=(%d, %d)", s.c_str(), p.first, p.second);
    if (p.first==-1) printf (": NOT FOUND\n"); 
    else {
        printf("\n");
        show(m, p.first);
        show(m, p.second);
    }
}
void OVCIN::test() {
    runtest("");
    runtest("0");
    runtest("9");
    runtest("a");
    runtest("az");
    runtest("b");
    runtest("yv");
    runtest("yvvv");
    runtest("yw");
    runtest("yz");
    runtest("zxc");
    runtest("zxcy");
    runtest("zxcz");
    runtest("zzzz");    
}
*/

