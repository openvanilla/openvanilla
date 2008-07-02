// OVCINSQ.cpp: .cin processor
//
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

//#define  OV_DEBUG
#include "OVCINSQ.h"
#include "OVFileHandler.h"
#include "OVWildcard.h"


#include <algorithm>	//for stable_sort() and transform()

using namespace std;
using namespace _OVCINSQ;
using namespace OpenVanilla;

/*

OVCINSQ::OVCINSQ(const char* fileName)
{
    OVFileHandler* fileHandler = new OVFileHandler(fileName);
    vector<string> stringVector;
    fileHandler->getLines(stringVector);
    delete fileHandler;
    
    state = PARSE_LINE;
    delimiters = " \t";
    parseCinVector(stringVector);
}
*/
OVCINSQ::OVCINSQ(const OVCINSQInfo& cininfo, SQLite3 *globaldb) : db(globaldb)
{
	tablename = cininfo.shortfilename.substr(0,cininfo.shortfilename.find_first_of(string(".")));
	murmur("OVCINSQ: Processing table: %s",tablename.c_str());

	bool parseTable=false;
	
	// Check  table... 
	SQLite3Statement *sth=db->prepare(
		"select dwHighTimeStamp, dwLowTimeStamp from tablelist where name = '%q';", tablename.c_str()); 
	if (sth && sth->step()==SQLITE_ROW) {
		int timestampHi = atoi(sth->column_text(0));
		int timestampLo = atoi(sth->column_text(1));
		delete sth;
		murmur("timestamp:%u, %u", timestampHi, timestampLo);
		if(timestampHi!=cininfo.dwHighTimeStamp ||timestampLo!=cininfo.dwLowTimeStamp)
		{
			murmur("Table:%s updated:  delete from '%q' where 1;",tablename.c_str());
			db->execute("delete from '%q' where 1;",tablename.c_str());
			murmur("update tablelist set dwHighTimeStamp='%u', dwLowTimeStamp='%u' where name ='%s';"
				,cininfo.dwHighTimeStamp, cininfo.dwLowTimeStamp, tablename.c_str());
			db->execute("update tablelist set dwHighTimeStamp='%d', dwLowTimeStamp='%d' where name ='%q';"
				,cininfo.dwHighTimeStamp, cininfo.dwLowTimeStamp, tablename.c_str());
			parseTable=true;
		}
		
	}
	else 
	{	
		
		murmur("create table '%s': insert into tablelist values ('%s','%s','%s','%s','%s','%s','%s','%u','%u');"
				,tablename.c_str(),tablename.c_str()
				, cininfo.shortfilename.c_str(), cininfo.longfilename.c_str()
				,cininfo.ename.c_str(), cininfo.cname.c_str()
				,cininfo.tcname.c_str(), cininfo.scname.c_str()
				,cininfo.dwHighTimeStamp, cininfo.dwLowTimeStamp);
		db->execute("insert into tablelist values ('%q','%q','%q','%q','%q','%q','%q','%d','%d');"  // timestamp has to be inserted as %d, not %u.
				,tablename.c_str()
				,cininfo.shortfilename.c_str(),cininfo.longfilename.c_str()
				,cininfo.ename.c_str(), cininfo.cname.c_str()
				,cininfo.tcname.c_str(), cininfo.scname.c_str()
				,cininfo.dwHighTimeStamp, cininfo.dwLowTimeStamp);
		db->execute("create table '%q' (key, value, ord);", tablename.c_str());
		db->execute("create index %q_index_key on '%q' (key);", tablename.c_str(), tablename.c_str());
		db->execute("create index %q_index_value on '%q' (value);", tablename.c_str(), tablename.c_str());
				
		parseTable=true;
	}

	
	if(parseTable)
	{
		murmur("Parse table:%s into db.",tablename.c_str());

		//db->execute("create temp table 'tempcin' (key, value, ord);");

		Watch watch;
		watch.start();

		OVFileHandler* fileHandler = new OVFileHandler(cininfo.longfilename.c_str());
		vector<string> stringVector;
		fileHandler->getLines(stringVector); 
		delete fileHandler; 
		
		watch.stop(); 
		murmur("reading cin into memory in %1.3f.\n",watch.getSec());
    
		db->execute("PRAGMA journal_mode = OFF;");
		db->execute("begin;");
		murmur("begin;");

		state = PARSE_LINE;
		delimiters = " \t";
		
		watch.start();
		
		parseCinVector(stringVector);
		
		watch.stop();
		murmur("parsecinvector() in %1.3f.\n",watch.getSec());
		
		

		// parse properties
		for(int i=0;i< _OVCINSQ::NUM_PROPERTY; i++)
		{
			//db->execute("insert into 'tempcin' values ('_property_%q', '%q', -1);",  propertyNames[i], properties[i].c_str() );
			db->execute("insert into '%q' values ('_property_%q', '%q', -1);",tablename.c_str(),  propertyNames[i], properties[i].c_str() );
			murmur("parsing properties: insert into '%s' values ('_property_%s', '%s', -1);",tablename.c_str(), propertyNames[i], properties[i].c_str());
		}
		
 
		db->execute("commit;");
		murmur("commit;");

		//watch.start();
		
		//db->execute("insert into '%q' select * from 'tempcin';", tablename.c_str());
		
		//watch.stop();
		//murmur("copy temp table back in %1.3f.\n",watch.getSec());
		

	}
	else
	{
		// Retrieve properties from db...
		for(int i=0;i< _OVCINSQ::NUM_PROPERTY; i++)
		{
			SQLite3Statement *sth=db->prepare(
			"select value from '%q' where key = '_property_%q';", tablename.c_str(), propertyNames[i]); 
			if (sth && sth->step()==SQLITE_ROW) {
				properties[i] = string(sth->column_text(0));
				delete sth;
			}
			
			murmur("Retrive properties: '%s' : '_property_%s';",  propertyNames[i], properties[i].c_str());
		}
	
	}



}


int OVCINSQ::setProperty(const string& key, const string& value){
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
		
		Watch watch;
		watch.start();
		
        setBlockMap();

		watch.stop(); 
		murmur("setBlockMap() in %1.3f.\n",watch.getSec());
		

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

void OVCINSQ::setBlockMap(){
    vector< pair<string, string> >::const_iterator it;
	
	Watch watch;
	watch.start();
    stable_sort(block_buf.begin(), block_buf.end(), cmpBlockEntry());
	watch.stop();

	murmur("stable_sort() in %1.3f.\n",watch.getSec());
	
 //   CinMap &curMap = maps[curMapIndex];
	if(curMapIndex == _OVCINSQ::M_KEY && block_buf.size()) {
		for(it = block_buf.begin(); it != block_buf.end(); ++it) {
			//db->execute("insert into 'tempcin' values ('_key_%q', '%q', -1);", it->first.c_str(), it->second.c_str());
			db->execute("insert into '%q' values ('_key_%q', '%q', -1);",tablename.c_str(), it->first.c_str(), it->second.c_str());
			murmur("insert into '%s' values ('_key_%s', '%s', -1);",tablename.c_str(), it->first.c_str(), it->second.c_str());
		}
		block_buf.clear();
	}			
	else if(curMapIndex == _OVCINSQ::M_CHAR && block_buf.size()) {		
	int ord; 
	string lastkey=string("");
	for(it = block_buf.begin(); it != block_buf.end(); ++it) {
        //if( !curMap.empty() && curMap.back().first == it->first )
		if(lastkey==it->first){
            //curMap.back().second.push_back(it->second);
			db->execute("insert into '%q' values ('%q', '%q', %d);",tablename.c_str() , it->first.c_str(), it->second.c_str(), ++ord);
		 	//murmur("insert into '%s' values ('%s', '%s', %d);",tablename.c_str(), it->first.c_str(), it->second.c_str(), ord);
		}
        else{
            //vector<string> v; 
            //v.push_back(it->second);
			ord=0;
			db->execute("insert into '%q' values ('%q', '%q', %d);",tablename.c_str(), it->first.c_str(), it->second.c_str(), ord);
			//murmur("insert into '%s' values ('%s', '%s', %d);",tablename.c_str(), it->first.c_str(), it->second.c_str(), ord);
            //curMap.push_back( make_pair( it->first, v) );
        }
		lastkey = it->first;
	}
	block_buf.clear();
	}
    
}

void OVCINSQ::parseCinVector(const vector<string>& cinVector){
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
		/*		
				if(curMapIndex == _OVCINSQ::M_KEY ){  //key 
					db->execute("insert into '%q' values ('_key_%q', '%q', -1);", tablename.c_str(),key.c_str(), value.c_str());
					murmur("insert into '%s' values ('_key_%s', '%s', -1);", tablename.c_str(),key.c_str(), value.c_str());
				}
				else
				{
					int ord=0;
					SQLite3Statement *sth=db->prepare(
						"select count(key) from '%q' where key = '%q';", tablename.c_str(), key.c_str()); 
		
					if (sth && sth->step()==SQLITE_ROW) 
					{
						ord=atoi(sth->column_text(0));
						delete sth;
					} 
					
					db->execute("insert into '%q' values ('%q', '%q', %d);", tablename.c_str(), key.c_str(), value.c_str(), ord);
					//murmur("insert into 'tempcin' values ('%s', '%s', %d);",key.c_str(), value.c_str(), ord);


				}
		*/		
				block_buf.push_back( make_pair(key, value) );
				
            }
        }
    }
}

void OVCINSQ::lowerStr(string& str){
    for(int i=static_cast<int>(str.length())-1; i>=0; i--)
		if(str[i]> 0x7E || str[i] < 0x20)// !isprint(str[i]) )
				return;
		//if(!isprint(str[i], m_locale))
    transform( str.begin(), str.end(), str.begin(),(int(*)(int)) tolower );
}

/*

size_t OVCINSQ::getVectorFromMap(const CinMap& inMapRef,
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


size_t OVCINSQ::getVectorFromMapWithWildcardSupport(const CinMap& inMapRef,const string& inKey, vector<string>& outStringVectorRef, char matchOneChar, char matchZeroOrMoreChar)
{
	size_t i, size = inMapRef.size();
	OVWildcard wildcard(inKey, matchOneChar, matchZeroOrMoreChar, true);
	
	outStringVectorRef.clear();
	
	for (i = 0; i < size; i++) {
		const pair<string, vector<string> >& p = inMapRef[i];
		
		if (wildcard.match(p.first)) {
			const vector<string>& vs = p.second;
			size_t j, vssize = vs.size();
			
			for (j = 0; j < vssize; j++) outStringVectorRef.push_back(vs[j]);
		}
	}
	
	return outStringVectorRef.size();
}

int OVCINSQ::searchCinMap(const CinMap& m, const string& key) const{
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


const pair<int, int> OVCINSQ::findRangeStartingWith(const CinMap& m,
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


int OVCINSQ::findClosestUpperBound(const CinMap& m, const string& key) const{
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

int OVCINSQ::findClosestLowerBound(const CinMap& m, const string& key) const{
    string newkey=key;
    size_t l=newkey.length();
    if (l) {
        if ((unsigned char)newkey[l-1] != 0xff)
            newkey[l-1] = (unsigned char)newkey[l-1] +1;
    }
    
    return findClosestUpperBound(m, newkey);
}
*/

size_t OVCINSQ::getCharVectorByKey(const string& inKey, vector<string>& outStringVectorRef)
{
	SQLite3Statement *sth=db->prepare(
		"select value from '%q' where key = '_key_%q';", tablename.c_str(), inKey.c_str()); 
	outStringVectorRef.clear();
	if (sth && sth->step()==SQLITE_ROW) 
	{
		outStringVectorRef.push_back( string(sth->column_text(0)));
		delete sth;
		return 1;	
	}
	else  
		return 0;


    //return getVectorFromMap(maps[_OVCINSQ::M_KEY], inKey, outStringVectorRef);
}


size_t OVCINSQ::getWordVectorByCharWithWildcardSupport(const string& inKey, vector<string>& outStringVectorRef, 
													   char matchOneChar, char matchZeroOrMoreChar, bool smartorder)

{
	/*
	string::size_type i, len = inKey.length();
	for (i = 0; i < len; i++) {
		if (inKey[i] == matchOneChar || inKey[i] == matchZeroOrMoreChar)
			return getVectorFromMapWithWildcardSupport(maps[_OVCINSQ::M_CHAR], inKey, outStringVectorRef, matchOneChar, matchZeroOrMoreChar);
	}
	
	return getVectorFromMap(maps[_OVCINSQ::M_CHAR], inKey, outStringVectorRef);
	*/
	bool wildcard=false;
	string realKey = inKey;

	string::size_type pos = 0;
	while ( (pos = realKey.find(matchOneChar) ) != string::npos ) {
        realKey.replace( pos, 1, 1,'_');
		wildcard = true;
        pos++;
    }

	while ( (pos = realKey.find(matchZeroOrMoreChar)) != string::npos ) {
        realKey.replace( pos, 1, 1, '%');
		wildcard = true;
        pos++;
    }
	
	if(wildcard){

		SQLite3Statement *sth = db->prepare(
				"select value from '%q' where key like '%q' and ord >-1;", tablename.c_str(), realKey.c_str()); 


		murmur("getWordVectorByCharWithWildcard: select value from '%s' where key like '%s' and ord >-1;", tablename.c_str(), realKey.c_str()); 
		outStringVectorRef.clear();
		if (sth){
			while (sth->step()==SQLITE_ROW) 
				outStringVectorRef.push_back( string(sth->column_text(0)));

			murmur("\t get %d candidates.", outStringVectorRef.size());
			delete sth;
			return outStringVectorRef.size();	
		
		}
		else  
			return 0;

	}
	else
		return getWordVectorByChar(inKey, outStringVectorRef, smartorder);
}

size_t OVCINSQ::getWordVectorByChar(const string& inKey,
                               vector<string>& outStringVectorRef, bool smartorder)
{
	SQLite3Statement *sth;
	
	if(smartorder)
		sth = db->prepare(
			"select value from '%q' where key = '%q' and ord >-1 order by\
(select count from freq.phrase where key='%q'.value)+0 desc, ord;", tablename.c_str(), inKey.c_str(), tablename.c_str()); 
	else
		sth = db->prepare(
			"select value from '%q' where key = '%q' and ord >-1 order by ord;", tablename.c_str(), inKey.c_str()); 

	murmur("getWordVectorByChar: select value from '%s' where key = '%s' and ord >-1 order by\
(select count from freq.phrase where key='%s'.value)+0 desc, ord;", tablename.c_str(), inKey.c_str(), tablename.c_str()); 

	outStringVectorRef.clear();

	if (sth){
		while (sth->step()==SQLITE_ROW) 
			outStringVectorRef.push_back( string(sth->column_text(0)));

		murmur("\t get %d candidates.", outStringVectorRef.size());
		delete sth;
		return outStringVectorRef.size();	
		
	}
	else  
		return 0;
	

    //return getVectorFromMap(maps[_OVCINSQ::M_CHAR], inKey, outStringVectorRef);
}


size_t OVCINSQ::getCharVectorByWord(const string& inWord,
                               vector<string>& outStringVectorRef)
{
	SQLite3Statement *sth = db->prepare(
			"select key from '%q' where value = '%q' and ord >-1 ", tablename.c_str(), inWord.c_str()); 
	
	murmur("getCharVectorByWord: sselect key from '%s' where value = '%s' and ord >-1 ", tablename.c_str(), inWord.c_str()); 

	outStringVectorRef.clear();

	if (sth){
		while (sth->step()==SQLITE_ROW) 
			outStringVectorRef.push_back( string(sth->column_text(0)));

		murmur("\t get %d candidates.", outStringVectorRef.size());
		delete sth;
		return outStringVectorRef.size();	
		
	}
	else  
		return 0;

}


int OVCINSQ::updatePhraseUserFrequency(const char* phrase)
{
	
//  update freq. table --------------------

	SQLite3Statement *sth = db->prepare("select count from freq.phrase where key='%q';",phrase);
	
	if (sth && sth->step()==SQLITE_ROW)
	{
		int count = atoi(sth->column_text(0));
		murmur("updatePhraseUserFreq: update freq.phrase set count='%d' where key='%s';",count+1, phrase);
		if (int err=db->execute("update freq.phrase set count='%d' where key='%q';",count+1, phrase)) 
		{
        murmur("SQLite3 insert freq phrase count error! code=%d", err);
		return 1;
		}
		delete sth;
			
	}
	else
	{
		murmur("updatePhraseUserFreq: insert into freq.phrase values('%s','1');", phrase);
		if (int err=db->execute("insert into freq.phrase values('%q','1');", phrase)) 
		{
        murmur("SQLite3 updatte freq phrase count error! code=%d", err);
		return 1; 
		}

	}
	

	return 0;
}


size_t OVCINSQ::getAssociatedPhrases(const string& inWord, vector<string>& outStringVectorRef)
{


	SQLite3Statement *sth=db->prepare(
		"select value from assoc where key = '%q' and ord >-1 order by \
(select count from freq.phrase where key = assoc.key||assoc.value)+0 desc, ord;", inWord.c_str()); 

	murmur("getAssociatedPhrases: select value from assoc where key = '%s' and ord >-1 order by \
(select count from freq.phrase where key = assoc.key||assoc.value)+0 desc, ord;", inWord.c_str());

	outStringVectorRef.clear();
	if (sth){
		while (sth->step()==SQLITE_ROW) 
			outStringVectorRef.push_back( string(sth->column_text(0)));

		murmur("\t get %d candidates.", outStringVectorRef.size());
		delete sth;
		return outStringVectorRef.size();	
		
	}
	else  
		return 0;
	


}



