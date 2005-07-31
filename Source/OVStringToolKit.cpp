#include "OVStringToolKit.h"
#include <iostream>

using namespace std;

OVStringToolKit::OVStringToolKit() {}

OVStringToolKit::~OVStringToolKit() {}

int OVStringToolKit::getLines(string inString,
							  vector<string>& outStringVectorRef)
{
	vector<string> delimiters;
	delimiters.push_back("\n");
	return splitString(inString, outStringVectorRef, delimiters, false);
}

int OVStringToolKit::splitString(string inString,
								 vector<string>& outStringVectorRef,
								 vector<string> delimiterVector,
								 bool hasDelimiter)
{
	///* string::find()
	int previousPosition = 0, currentPosition = 0;
	string currentSubString;
	string matchedDelimiter;
	while(currentPosition > -1)
	{
		for(unsigned int i = 0; i < delimiterVector.size(); i++)
		{
			currentPosition =
				inString.find(delimiterVector[i], previousPosition);
			if(currentPosition > -1) {
				matchedDelimiter = delimiterVector[i];
				break;
			}
		}
		
		if(currentPosition > -1) {
			currentSubString =
				inString.substr(previousPosition,
								currentPosition - previousPosition);

			previousPosition = currentPosition + 1;
		} else
			currentSubString =
				inString.substr(previousPosition,
								inString.length() - previousPosition + 1);

		if(currentSubString.length() > 0) {
			if(currentSubString == matchedDelimiter && hasDelimiter)
				outStringVectorRef.push_back(matchedDelimiter);
			else
				outStringVectorRef.push_back(currentSubString);

			currentSubString.erase();
			//.clear() -> .erase() makes VC++ happy...
		}
	}
	//*/
	
	/* handcraft sequencial search... =_=
	bool doPush = false;
	string currentSubString;
	for(int charIndex = 0; charIndex < inString.length(); charIndex++)
	{
		if(inString[charIndex] != delimiter) {
			currentSubString.push_back(inString[charIndex]);
			
			if(charIndex == inString.length() - 1)
				doPush = true;
		} else
			doPush = true;
		
		if(doPush) {
			doPush = false;
			
			if(currentSubString.length() > 0) {
				outStringVectorRef.push_back(currentSubString);
				currentSubString.clear();
			}
		}
	}
	*/
	
	return outStringVectorRef.size();
}