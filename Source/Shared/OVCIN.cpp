// OVXCIN.cpp

#include "OVXCIN.h"
#include "OVFileHandler.h"
#include "OVStringToolKit.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

OVXCIN::OVXCIN(char* fileName)
{
	OVFileHandler* fileHandler = new OVFileHandler(fileName);
	vector<string> stringVector;
	fileHandler->getLines(stringVector);
	delete fileHandler;
	
	ename = getPropertyByName(stringVector, "ename");
	cname = getPropertyByName(stringVector, "cname");
	selkey = getPropertyByName(stringVector, "selkey");
	
	getMapByName(stringVector, keyMap, "keyname");
	getMapByName(stringVector, charMap, "chardef");
}

OVXCIN::~OVXCIN()
{
	charMap.clear();
	keyMap.clear();
}

vector<string> OVXCIN::getCharVectorByKey(string key)
{
	return getVectorFromMap(keyMap, key);
}

vector<string> OVXCIN::getWordVectorByChar(string key)
{
	return getVectorFromMap(charMap, key);
}

vector<string> OVXCIN::getVectorFromMap(map< string, vector<string> >& inMapRef,
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
 
string OVXCIN::getPropertyByName(vector<string>& inStringVectorRef,
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
	
	return NULL;
}

int OVXCIN::getMapByName(vector<string>& inStringVectorRef,
						 map< string, vector<string> >& outMapRef,
						 string mapName)
{
	bool doGet = false;
	string pattern = "%" + mapName + " begin";	
	for(int i = 0; i < inStringVectorRef.size(); i++)
	{
		string currentString = inStringVectorRef[i];
		if(!doGet) {
			if(currentString.find(pattern, 0) == 0) {
				doGet = true;
				pattern = "%" + mapName + " end";
			}
		} else {
			if(currentString.find(pattern, 0) == 0)
				break;
			else {
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
    OVXCIN ovxcin(argv[1]);
	
	cout << "ename:\t" << ovxcin.ename << endl;
	cout << "cname:\t" << ovxcin.cname << endl;
	cout << "selkey:\t" << ovxcin.selkey << endl;
		
	string inKey;
	cout << "key:";
	cin >> inKey;
	vector<string> charVector = ovxcin.getCharVectorByKey(inKey);	
	for(int i = 0; i < charVector.size(); i++)
		cout << "keyMap[\"" + inKey + "\"] = " << charVector[i] << endl;
	charVector.clear();
	inKey.clear();

	string inChar;
	cout << "char:";
	cin >> inChar;
	vector<string> wordVector = ovxcin.getWordVectorByChar(inChar);
	for(int j = 0; j < wordVector.size(); j++)
		cout << "charMap[\"" + inChar + "\"] = " << wordVector[j] << endl;
	wordVector.clear();
	inChar.clear();
	
	return 0;
}