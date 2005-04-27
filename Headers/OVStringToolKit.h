#ifndef __OVSTRINGTOOLKIT_h
#define __OVSTRINGTOOLKIT_h

#include <string>
#include <vector>

using namespace std;

class OVStringToolKit
{
public:
	OVStringToolKit();
	~OVStringToolKit();

	static int getLines(string inString,
						vector<string>& outStringVectorRef);	
	static int splitString(string inString,
						   vector<string>& outStringVectorRef,
						   vector<string> delimiterVector,
						   bool hasDelimiter);
};

#endif