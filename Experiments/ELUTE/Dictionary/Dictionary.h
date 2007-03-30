#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class Dictionary
{
public:
	Dictionary(string& dicFilePath);
	~Dictionary();
	size_t getValueVector(string& key, vector<string>& valueVectorRef);
	size_t getCount(string& key);
	bool has(string& key);

private:
	map<string, vector<string> > dic_;
};

#endif
