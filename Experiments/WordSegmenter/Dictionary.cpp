#include "Dictionary.h"

#include "OVStringToolKit.h"
#include "OVFileHandler.h"

#include <vector>
#include <iostream>

using namespace std;

Dictionary::Dictionary (string& dicFilePath)
{
	cerr << "Loading the dictionary file: " << dicFilePath << endl;
	OVFileHandler *fh = new OVFileHandler(dicFilePath.c_str());
	if(!fh->isOpened()) {
		cerr << "The dictionary file " << dicFilePath;
		cerr << " not found." << endl;
		exit(-1);
	}

	vector<string> fileStringVector;
	if(fh->getLines(fileStringVector) > 0) {
		for(size_t i = 0; i < fileStringVector.size(); i++) {
			string row = fileStringVector[i];
			size_t valueLen = row.find(' ', 0);
			string value = row.substr(0, valueLen);
			size_t freqStart = valueLen + 1;
			size_t freqEnd = row.find(' ', freqStart);
			size_t keyStart = freqEnd + 1;
			size_t keyLen = row.length() - keyStart;
			string key = row.substr(keyStart, keyLen);
			if(dic_.count(key) > 0)
				dic_[key].push_back(value);
			else {
				vector<string> values;
				values.push_back(value);
				dic_.insert(make_pair(key, values));
			}
		}	
	}

	cerr << "The dictionary file " << dicFilePath << " loaded." << endl;
	delete fh;
}

Dictionary::~Dictionary()
{
	dic_.clear();
}

size_t Dictionary::getValueVector(string& key, vector<string>& valueVectorRef)
{
	valueVectorRef = dic_[key];
	return valueVectorRef.size();
}

size_t Dictionary::getCount(string& key)
{
	return dic_[key].size();
}

bool Dictionary::has(string& key)
{
	return dic_[key].size() > 0 ? true : false;
}
