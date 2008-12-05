// OVFileHandler.cpp: A fast file "slurping" class
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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

#ifdef __linux__
	#include <string.h>
#endif

using namespace std;


OVFileHandler::OVFileHandler(const char* filename)
{
	//inFile.open(fileName, ios_base::binary);
#if defined (WIN32) && !defined (__CYGWIN__)
	m_mmapHandle = 0;
#endif
	m_mmapPtr = openFileByMMAP(filename);
}

OVFileHandler::~OVFileHandler()
{
	//inFile.close();
	closeFileByMMAP();
}

bool OVFileHandler::isOpenedByMMAP()
{
	return m_mmapPtr != 0 ? true : false;
}

/*
bool OVFileHandler::isOpenedBySTL()
{
	return inFile != 0 ? true : false;
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
*/

string OVFileHandler::getFileStringByMMAP()
{
	string fileString(static_cast<char*>(m_mmapPtr));

	return fileString;
}

int OVFileHandler::getLines(vector<string>& outStringVectorRef)
{
	string fileString;
	fileString = getFileStringByMMAP();	

	return OVStringToolKit::getLines(fileString, outStringVectorRef);
}

void* OVFileHandler::openFileByMMAP (const char* file_name)
{
    void* mmap_ptr;
#if defined (WIN32) && !defined (__CYGWIN__)
	MmapHandles tmp;
	int len;
	
	tmp.hFile = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (tmp.hFile == INVALID_HANDLE_VALUE) {
        return NULL;
    }
	len = GetFileSize(tmp.hFile, NULL);
	tmp.hMap = CreateFileMapping(tmp.hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (tmp.hMap == NULL) {
        CloseHandle(tmp.hFile);
        return NULL;
    }
	mmap_ptr = (char*)MapViewOfFile(tmp.hMap, FILE_MAP_READ, 0, 0, 0);
	if (mmap_ptr == NULL) {    
		CloseHandle(tmp.hFile);
        CloseHandle(tmp.hMap);
	}
	if (!m_mmapHandle) {
		m_mmapHandle = (MmapHandles*)malloc(sizeof(MmapHandles));
	}
	m_mmapHandle->hFile = tmp.hFile;
	m_mmapHandle->hMap  = tmp.hMap;
#else
	int fd;
    struct stat stat;
    if ((fd = open (file_name, O_RDONLY)) < 0)  {
		return NULL;
    }
    if (fstat(fd, &stat) < 0) {
		return NULL;
    }
    mmap_ptr = mmap(0 /* select by system */,
	    stat.st_size /* len */,
	    PROT_READ /* Read */,
	    MAP_SHARED /* use no flag */,
	    fd /* file pointer */,
	    0 /* offset */
	    );
    close(fd);
    if (MAP_FAILED == mmap_ptr) {
		return NULL;
    }
#endif
    return mmap_ptr;
}

int OVFileHandler::closeFileByMMAP ()
{
#if defined (WIN32) && !defined (__CYGWIN__)
	CloseHandle(m_mmapHandle->hFile);
    CloseHandle(m_mmapHandle->hMap);
	free (m_mmapHandle);
	return UnmapViewOfFile(m_mmapPtr);
#else 
    return munmap(m_mmapPtr, strlen(static_cast<char*>(m_mmapPtr)));
#endif
}
