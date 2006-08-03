#ifndef KEYSTROKETOWORDCONVERTER_H
#define KEYSTROKETOWORDCONVERTER_H

#include <string>

#include "LanguageModel.h"
#include "Dictionary.h"
#include "Token.h"

using namespace std;

class KeystrokeToWordConverter
{
public:
	bool loadModel(string& lmFilePath);
	bool loadDictionary(string& dicFilePath);
	bool convert(vector<Token*>& inputTokens, vector<Token*>& outputTokens);

private:
	LanguageModel* lm_;
	Dictionary* dic_;
};

#endif
