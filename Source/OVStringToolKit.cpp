#include "OVStringToolKit.h"
#include <iostream>

using namespace std;

OVStringToolKit::OVStringToolKit() {}

OVStringToolKit::~OVStringToolKit() {}

int OVStringToolKit::getLines(string inString,
							  vector<string>& outStringVectorRef)
{
	return splitString(inString, outStringVectorRef, "\n");
}

int OVStringToolKit::splitString(string inString,
								 vector<string>& outStringVectorRef,
								 string delimiter)
{
	///* string::find()
	int previousPosition = 0, currentPosition = 0;
	string currentSubString;
	while(currentPosition > -1)
	{
		currentPosition = inString.find(delimiter, previousPosition);
		if(currentPosition > -1) {
			currentSubString =
				inString.substr(previousPosition,
								currentPosition - previousPosition);

			previousPosition = currentPosition + 1;
		} else
			currentSubString =
				inString.substr(previousPosition,
								inString.length() - previousPosition + 1);

		if(currentSubString.length() > 0 && currentSubString != delimiter) {
			outStringVectorRef.push_back(currentSubString);
			currentSubString.clear();
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