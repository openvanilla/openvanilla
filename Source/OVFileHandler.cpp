#include "OVFileHandler.h"
#include "OVStringToolKit.h"

using namespace std;

OVFileHandler::OVFileHandler(char* fileName)
{
	inFile.open(fileName, ios_base::binary);
#ifndef WIN32
	filePtr = open(fileName, O_RDONLY);
#endif
}

OVFileHandler::~OVFileHandler()
{
	inFile.close();
#ifndef WIN32
	close(filePtr);
#endif
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

#ifndef WIN32
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
#endif

int OVFileHandler::getLines(vector<string>& outStringVectorRef)
{
	string fileString;
#ifndef WIN32
	fileString = getFileStringByMMAP();	
#else
	fileString = getFileStringBySTL();
#endif

	return OVStringToolKit::getLines(fileString, outStringVectorRef);
}