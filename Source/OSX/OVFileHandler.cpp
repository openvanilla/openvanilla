#include "OVFileHandler.h"
#include "OVStringToolKit.h"

using namespace std;

OVFileHandler::OVFileHandler(char* fileName)
{
	inFile.open(fileName, ios_base::binary);
}

OVFileHandler::~OVFileHandler()
{
	inFile.close();
}

int OVFileHandler::getSize()
{
	int length = 0;
	
	inFile.seekg(0, ios_base::end);
	length = inFile.tellg();
	inFile.seekg(0);
	
	return length;
}

string OVFileHandler::getFileString()
{
	int length = getSize();
	char* buffer = new char[length];
	inFile.read(buffer, length);
	string fileString(buffer);
	delete[] buffer;
	
	return fileString;
}

int OVFileHandler::getLines(vector<string>& outStringVectorRef)
{
	string fileString = getFileString();
	return OVStringToolKit::getLines(fileString, outStringVectorRef);
}