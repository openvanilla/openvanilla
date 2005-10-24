#ifndef WORDSEGMENTER_H
#define WORDSEGMENTER_H

#include <map>
#include <string>

#include "Token.h"

using namespace std;

class Bin
{
public:	
	string gram;
	double logprob;
	double backoff;
};

class WordSegmenter
{
public:
	bool loadModel(map<string, Bin*>& lm);
	bool segment(vector<Token*>& inputTokens, vector<Token*>& outputTokens);

private:
	map<string, Bin*> lm_;
};

#endif