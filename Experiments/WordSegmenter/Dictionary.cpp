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
	vector<string> fileStringVector;
	vector<string> delimiters;
	string delimiter("\t");
	delimiters.push_back(delimiter);
	if(!fh->isOpened()) {
		cerr << "The dictionary file " << dicFilePath;
		cerr << " not found." << endl;
		exit(-1);
	}

	if(fh->getLines(fileStringVector) > 0) {
		for(size_t i = 0; i < fileStringVector.size(); i++) {
			string row = fileStringVector[i];
			vector<string> pair;
			if(OVStringToolKit::splitString(row, pair, delimiters, false) > 2) {
				if(dic_.count(pair[2]) > 0)
					dic_[pair[2]].push_back(pair[0]);
				else {
					vector<string> values;
					values.push_back(pair[0]);
					dic_.insert(make_pair(pair[2], values));
				}
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
	
	return dic_.count(key);
}

size_t Dictionary::getCount(string& key)
{
	return dic_.count(key);
}

bool Dictionary::has(string& key)
{
	return dic_.count(key) > 0 ? true : false;
}
