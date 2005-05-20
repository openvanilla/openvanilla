// OVCIN.cpp

//#define  OV_DEBUG
#include "OVCIN.h"
#include "OVFileHandler.h"
#include "OVUtility.h"
#include<ext/algorithm>

using namespace std;
using namespace _OVCIN;


static const char *propertyNames[NUM_PROPERTY] = {
    "selkey", "ename", "cname", "endkey", "encoding" 
};

static const char *mapNames[NUM_MAP] = {
    "keyname", "chardef"
};

OVCIN::OVCIN(char* fileName)
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
        if( it->find("#") == 0 )    continue;
        const string& line = *it;
        unsigned int del_pos;
        if( (del_pos=line.find_first_of(delimiters)) != string::npos ){
            string key = line.substr(0, del_pos);
            unsigned int value_pos=line.find_first_not_of(delimiters, del_pos);
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
    for(int i=str.length()-1;i>=0;i--)
        if( !isprint(str[i]) )
            return;
    transform( str.begin(), str.end(), str.begin(),(int(*)(int)) tolower );
}





int OVCIN::getVectorFromMap(const CinMap& inMapRef,
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
    int mid, low = 0, high = m.size() - 1;
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
