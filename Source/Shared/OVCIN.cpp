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

int OVCIN::getCharVectorByKey(string inKey,
							  vector<string>& outStringVectorRef)
{
	return getVectorFromMap(keyMap, inKey, outStringVectorRef);
}

int OVCIN::getWordVectorByChar(string inKey,
							   vector<string>& outStringVectorRef)
{
	return getVectorFromMap(charMap, inKey, outStringVectorRef);
}

int OVCIN::getVectorFromMap(map< string, vector<string> >& inMapRef,
							string inKey,
							vector<string>& outStringVectorRef)
{
	outStringVectorRef = inMapRef[inKey];
	
	return inMapRef[inKey].size();
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
	string sectionMark = "begin";
	for(int i = 0; i < inStringVectorRef.size(); i++)
	{
		string currentString = inStringVectorRef[i];
		int foundIndex = currentString.find("%" + mapName, 0);		
		if(!doGet) {
			if(foundIndex == 0) {
				int foundBegin = currentString.find(sectionMark, foundIndex);
				if(foundBegin > -1) {	// ready to read name-value pairs
					doGet = true;
					sectionMark = "end";
				}
			}
		} else {
			if(foundIndex == 0) {
				int foundEnd = currentString.find(sectionMark, foundIndex);
				if(foundEnd > -1)	// stop reading pairs
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
						outMapRef.insert(make_pair(pairVector[0],
												   currentVector));
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
	cout << "Is an end key or not:";
	cin >> keyChar;
	if(ovcin.isEndKey(keyChar))
		cout << "Yes." << endl;
	else
		cout << "No." << endl;
		
	string inKey;
	cout << "Get character(s) by a keystroke:";
	cin >> inKey;
	vector<string> charVector;
	int charVectorSize = ovcin.getCharVectorByKey(inKey, charVector);
	if(charVectorSize > 0)
		for(int i = 0; i < charVectorSize; i++)
			cout << inKey << "->" << charVector[i] << endl;
	else
		cout << "Not found." << endl;
	inKey.clear();

	string inChar;
	cout << "Get word(s) by a character:";
	cin >> inChar;
	vector<string> wordVector;
	int wordVectorSize = ovcin.getWordVectorByChar(inChar, wordVector);
	if(wordVectorSize > 0)
		for(int j = 0; j < wordVectorSize; j++)
			cout << inChar << "->" << wordVector[j] << endl;
	else
		cout << "Not found." << endl;
	inChar.clear();
	
	return 0;
}