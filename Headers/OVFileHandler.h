#ifndef __OVFILEHANDLER_h
#define __OVFILEHANDLER_h

#include <string>
#include <vector>
#include <fstream>
/*
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
*/

using namespace std;

class OVFileHandler
{
public:
	OVFileHandler(char* fileName);
	~OVFileHandler();
	
	int getLines(vector<string>& outStringVectorRef);
	
private:
	ifstream inFile;
	int filePtr;
	int getSize();
	string getFileStringBySTL();
//	string getFileStringByMMAP();	
};
	
#endif