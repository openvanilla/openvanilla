#ifndef __OVFILEHANDLER_h
#define __OVFILEHANDLER_h

#include <string>
#include <vector>
#include <fstream>


using namespace std;

class OVFileHandler
{
public:
	OVFileHandler(char* fileName);
	~OVFileHandler();
	
	int getSize();
	string getFileString();
	int getLines(vector<string>& outStringVectorRef);
	
private:
	ifstream inFile;
};
	
#endif