#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <string>
#include <map>

using namespace std;

class Gram
{
public:	
	string gram;
	double logprob;
	double backoff;
};

class LanguageModel
{
public:
	LanguageModel(string& lmFilePath);
	~LanguageModel();
	double getLogProb(string& gram);
	double getBackOff(string& gram);
	double getUnkLogProb();
	size_t getCount(string& gram);
	bool has(string& gram);

private:
	map<string, Gram*> lm_;
	string unknownWordLabel;
};

#endif