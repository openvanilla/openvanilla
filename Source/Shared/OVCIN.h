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
	// VXCIN::read()
    OVCIN(char* fileName);
	
    ~OVCIN();

	// VXCIN::selKey()
	string& getSelKey();
	
	// VXCIN::name()
	string& getCName();
	
	// VXCIN::name()
	string& getEName();
	
	string& getEndKey();
	
	// VXCIN::isEndKey()
	bool isEndKey(char keyChar);
	
	// VXCIN::getKey()
	int getCharVectorByKey(string inKey,
						   vector<string>& outStringVectorRef);
	
	// VXCIN::find()
	int getWordVectorByChar(string inKey,
							vector<string>& outStringVectorRef);

protected:
	int getVectorFromMap(map< string, vector<string> >& inMapRef,
						 string inKey,
						 vector<string>& outStringVectorRef);
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