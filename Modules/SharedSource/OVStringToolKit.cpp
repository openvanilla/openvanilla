// OVStringToolKit.cpp:: A fast string toolkit
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

#include "OVStringToolKit.h"
#include <iostream>

using namespace std;

OVStringToolKit::OVStringToolKit() {}

OVStringToolKit::~OVStringToolKit() {}

bool OVStringToolKit::hasLinebreakBy(string& inString, const char crOrLf)
{
	if(string::npos == inString.find_last_of(crOrLf))
		return false;
	else
		return true;
}

int OVStringToolKit::getLines(
	string& inString,
	vector<string>& outStringVectorRef)
{
	vector<string> delimiters;
	//<comment author='b6s'>An heuristic to check file end for linebreaks.
	if(hasLinebreakBy(inString, '\r'))
		delimiters.push_back("\r");
	if(hasLinebreakBy(inString, '\n'))
		delimiters.push_back("\n");
	//</comment>
	return splitString(inString, outStringVectorRef, delimiters, false);
}

int OVStringToolKit::splitString(
	string& inString,
	vector<string>& outStringVectorRef,
	vector<string>& delimiterVector,
	bool hasDelimiter)
{
	///* string::find()
	int previousPosition = 0, currentPosition = 0;
	string currentSubString;
	string matchedDelimiter;
	while(currentPosition > -1)
	{
		for(size_t i = 0; i < delimiterVector.size(); i++)
		{
			currentPosition =
				static_cast<int>(inString.find(delimiterVector[i], previousPosition));
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
	
	return static_cast<int>(outStringVectorRef.size());
}
