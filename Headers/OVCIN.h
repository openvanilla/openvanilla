// OVCIN.h

#ifndef __OVCIN_h
#define __OVCIN_h

#include <string>
#include <vector>
#include <map>

using namespace std;

class OVCIN
{
public:	
    OVCIN(char* fileName);	// VXCIN::read()
    ~OVCIN();

	string getSelKey();	// VXCIN::selKey()
	string getCName();	// VXCIN::name()
	string getEName();	// VXCIN::name()
	string getEndKey();
	bool isEndKey(char keyChar);	// VXCIN::isEndKey()
	vector<string> getCharVectorByKey(string key);	// VXCIN::getKey()
	vector<string> getWordVectorByChar(string key);	// VXCIN::find()

protected:
	vector<string> getVectorFromMap(map< string, vector<string> >& inMapRef,
									string key);
	string getPropertyByName(vector<string>& inStringVectorRef,
							 string propertyName);
	int getMapByName(vector<string>& inStringVectorRef,
					 map< string, vector<string> >& outMapRef,
					 string mapName);
	
private:
	string selkey;
    string ename;
    string cname;
	string endkey;

	map<string, vector<string> > keyMap;
	map<string, vector<string> > charMap;
};

#endif