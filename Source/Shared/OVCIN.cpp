// OVCIN.cpp

#include "OVCIN.h"
#include "OVFileHandler.h"
#include "OVStringToolKit.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

OVCIN::OVCIN(char* fileName)
{
	OVFileHandler* fileHandler = new OVFileHandler(fileName);
	vector<string> stringVector;
	fileHandler->getLines(stringVector);
	delete fileHandler;
	
	ename = getPropertyByName(stringVector, "ename");
	cname = getPropertyByName(stringVector, "cname");
	selkey = getPropertyByName(stringVector, "selkey");
	endkey = getPropertyByName(stringVector, "endkey");
	
	getMapByName(stringVector, keyMap, "keyname");
	getMapByName(stringVector, charMap, "chardef");
}

OVCIN::~OVCIN()
{
	charMap.clear();
	keyMap.clear();
}

string OVCIN::getSelKey()
{
	return selkey;
}

string OVCIN::getCName()
{
	return cname;
}

string OVCIN::getEName()
{
	return ename;
}

string OVCIN::getEndKey()
{
	return endkey;
}

bool OVCIN::isEndKey(char keyChar)
{
	int foundIndex = endkey.find(keyChar, 0);
	if(foundIndex > -1)
		return true;
	else
		return false;
}

vector<string> OVCIN::getCharVectorByKey(string key)
{
	return getVectorFromMap(keyMap, key);
}

vector<string> OVCIN::getWordVectorByChar(string key)
{
	return getVectorFromMap(charMap, key);
}

vector<string> OVCIN::getVectorFromMap(map< string, vector<string> >& inMapRef,
									  string key)
{
	if(inMapRef[key].size() > 0)
		return inMapRef[key];
	else {
		vector<string> selfVector;
		selfVector.push_back(key);
		return selfVector;
	}
}
 
string OVCIN::getPropertyByName(vector<string>& inStringVectorRef,
								 string propertyName)
{
	string pattern = "%" + propertyName;
	int startIndex = pattern.length() + 1;
	for(int i = 0; i < inStringVectorRef.size(); i++)
	{
		string currentString = inStringVectorRef[i];
		if(currentString.find(pattern, 0) == 0)
			return currentString.substr(startIndex,
									currentString.length() - startIndex + 1);
	}
	
	return string("");
}

int OVCIN::getMapByName(vector<string>& inStringVectorRef,
						 map< string, vector<string> >& outMapRef,
						 string mapName)
{
	bool doGet = false;
	string pattern = "begin";	
	for(int i = 0; i < inStringVectorRef.size(); i++)
	{
		string currentString = inStringVectorRef[i];
		int foundIndex = currentString.find("%" + mapName, 0);		
		if(!doGet) {
			if(foundIndex == 0) {
				int foundBegin = currentString.find(pattern, foundIndex);
				if(foundBegin > -1) {					
					doGet = true;
					pattern = "end";
				}
			}
		} else {
			if(foundIndex == 0) {
				int foundEnd = currentString.find(pattern, foundIndex);
				if(foundEnd)
					break;
			} else {
				vector<string> pairVector;
				int vectorSize = OVStringToolKit::splitString(currentString,
															  pairVector, ' ');
				if(vectorSize == 2) {
					// pairVector[0] is the key,
					// pairVector[1] is the value.
					if(outMapRef.find(pairVector[0]) == outMapRef.end()) {
						vector<string> currentVector;
						currentVector.push_back(pairVector[1]);
						outMapRef.insert(make_pair(pairVector[0], currentVector));
					} else
						outMapRef[pairVector[0]].push_back(pairVector[1]);
				}
			}
		} // The end of if(!doGet)
	} // The end of for
	
	return outMapRef.size();
}

int main(int argc, char**argv)
{
    OVCIN ovcin(argv[1]);
	
	cout << "ename:\t" << ovcin.getEName() << endl;
	cout << "cname:\t" << ovcin.getCName() << endl;
	cout << "selkey:\t" << ovcin.getSelKey() << endl;
	cout << "endkey:\t" << ovcin.getEndKey() << endl;
	
	char keyChar;
	cout << "is end key?\t";
	cin >> keyChar;
	if(ovcin.isEndKey(keyChar))
		cout << "Yes" << endl;
	else
		cout << "No" << endl;
		
	string inKey;
	cout << "key:";
	cin >> inKey;
	vector<string> charVector = ovcin.getCharVectorByKey(inKey);	
	for(int i = 0; i < charVector.size(); i++)
		cout << "keyMap[\"" + inKey + "\"] = " << charVector[i] << endl;
	charVector.clear();
	inKey.clear();

	string inChar;
	cout << "char:";
	cin >> inChar;
	vector<string> wordVector = ovcin.getWordVectorByChar(inChar);
	for(int j = 0; j < wordVector.size(); j++)
		cout << "charMap[\"" + inChar + "\"] = " << wordVector[j] << endl;
	wordVector.clear();
	inChar.clear();
	
	return 0;
}