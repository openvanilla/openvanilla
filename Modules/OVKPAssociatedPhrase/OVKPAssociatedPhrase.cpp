// OVKPAssociatedPhrases.cpp: Generic tool for associated phrases
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

#include "OVKPAssociatedPhrase.h"
#include "OVCIN.h"
#include <vector>

OVCIN* _OVKPAPSharedCIN = 0;
vector<string> _OVKPAPSharedResults;

extern "C" unsigned int OVGetLibraryVersion()
{
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService*s, const char* libpath)
{
	return 1;
}

extern "C" OVModule* OVGetModuleFromLibrary(int x)
{
	switch(x) {
		case 0:
			return new OVOFAssociatedPhraseReceptor;
			
		case 1:
			return new OVKPAssociatedPhrasePrompter;
	}
	
	return 0;
}

void OVKPAssociatedPhrasePrompterContext::start(OVBuffer*, OVCandidate*, OVService*)
{
	clear();
}

void OVKPAssociatedPhrasePrompterContext::clear()
{
	_OVKPAPSharedResults.clear();
}

void OVKPAssociatedPhrasePrompterContext::end()
{
	clear();
}

int OVKPAssociatedPhrasePrompterContext::keyEvent(OVKeyCode* keyCode, OVBuffer* buffer, OVCandidate*, OVService*)
{
	if (!_OVKPAPSharedResults.size()) {
		clear();
		return 0;
	}
	
	if (keyCode->isCommand()) {
		clear();
		return 0;
	}	
	
	size_t index;
	char *selectKeys = "!@#$%^&*(";
	int selected = -1;
	
	for (index = 0; index < strlen(selectKeys); index++) {
		if (keyCode->code() == selectKeys[index]) {
			selected = index;
			break;
		}
	}
	
	if (selected == -1 || selected >= (int)_OVKPAPSharedResults.size()) {
		clear();
		return 0;
	}
	
	string notifyString("Sent string: ");
	notifyString += _OVKPAPSharedResults[selected];
	buffer->clear()->append(_OVKPAPSharedResults[selected].c_str())->send();
	return 1;
}

int OVOFAssociatedPhraseReceptor::initialize(OVDictionary*, OVService* service, const char* modulePath)
{
	if (!_OVKPAPSharedCIN) {
		string dataPath(modulePath);
		dataPath += service->pathSeparator();
		dataPath += "OVKPAssociatedPhrase";
		dataPath += service->pathSeparator();
		dataPath += "associated-phrases.cin";
		
		murmur("loading data table %s", dataPath.c_str());
		_OVKPAPSharedCIN = new OVCIN(dataPath.c_str());
	}
	
	return 1;
}

const char* OVOFAssociatedPhraseReceptor::process(const char* inputString, OVService* service)
{
	_OVKPAPSharedResults.clear();
	_OVKPAPSharedCIN->getWordVectorByChar(string(inputString), _OVKPAPSharedResults);
	
	if (_OVKPAPSharedResults.size()) {
		string notifyString("SHIFT + ");
		
		for (size_t i = 0; i < _OVKPAPSharedResults.size(); i++) {		
			notifyString += string(1, '1' + i);
			notifyString += ".";
			notifyString += _OVKPAPSharedResults[i];
			notifyString += " ";
		}
		
		service->notify(notifyString.c_str());
	}
	
	return inputString;
}
