#ifndef WORDSEGMENTER_H
#define WORDSEGMENTER_H

#include <string>

#include "LanguageModel.h"
#include "Token.h"

using namespace std;

class WordSegmenter
{
public:
	bool loadModel(string& lmFilePath);
	bool segment(vector<Token*>& inputTokens, vector<Token*>& outputTokens);

private:
	LanguageModel* lm_;
};

#endif