// OVXCIN.h

#ifndef __OVXCIN_h
#define __OVXCIN_h

#include <string>
#include <vector>
#include <map>

using namespace std;

class OVXCIN
{
public:
	string selkey;
    string ename;
    string cname;
	
    OVXCIN(char* fileName);
    ~OVXCIN();
	vector<string> getCharVectorByKey(string key);
	vector<string> getWordVectorByChar(string key);

protected:
	vector<string> getVectorFromMap(map< string, vector<string> >& inMapRef,
									string key);
	string getPropertyByName(vector<string>& inStringVectorRef,
							 string propertyName);
	int getMapByName(vector<string>& inStringVectorRef,
					 map< string, vector<string> >& outMapRef,
					 string mapName);
	
private:
	map<string, vector<string> > keyMap;
	map<string, vector<string> > charMap;
};

#endif