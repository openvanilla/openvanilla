#include "OVFileHandler.h"
#include "OVStringToolKit.h"

using namespace std;

OVFileHandler::OVFileHandler(char* fileName)
{
	inFile.open(fileName, ios_base::binary);
	filePtr = open(fileName, O_RDONLY);
}

OVFileHandler::~OVFileHandler()
{
	inFile.close();
	//close(filePtr);
}

int OVFileHandler::getSize()
{
	int length = 0;
	
	inFile.seekg(0, ios_base::end);
	length = inFile.tellg();
	inFile.seekg(0);
	
	return length;
}

string OVFileHandler::getFileStringBySTL()
{
	int length = getSize();
	char* buffer = new char[length];
	inFile.read(buffer, length);
	string fileString(buffer);
	delete[] buffer;
	
	return fileString;
}

string OVFileHandler::getFileStringByMMAP()
{
	void* mmap_ptr;
	int size;
	
	size = getSize();
	mmap_ptr = mmap(0, size, PROT_READ, MAP_SHARED, filePtr, 0);
	close(filePtr);
	fflush(NULL);

	string fileString(static_cast<char*>(mmap_ptr));

	return fileString;
}

int OVFileHandler::getLines(vector<string>& outStringVectorRef)
{
	//string fileString = getFileStringBySTL();
	string fileString = getFileStringByMMAP();	
	return OVStringToolKit::getLines(fileString, outStringVectorRef);
}