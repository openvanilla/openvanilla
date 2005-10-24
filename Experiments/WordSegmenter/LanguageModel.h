#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <string>

using namespace std;

class LanguageModel
{
public:
	virtual double getLogProb(string&) = 0;
	virtual double getBackOff(string&) = 0;
};

#endif