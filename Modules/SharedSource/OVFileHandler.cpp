// OVFileHandler.cpp: A fast file "slurping" class
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "OVFileHandler.h"
#include "OVStringToolKit.h"

using namespace std;

OVFileHandler::OVFileHandler(const char* fileName)
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

bool OVFileHandler::isOpened()
{
	return inFile != 0 ? true : false;
#ifndef WIN32
	return filePtr != 0 ? true : false;
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