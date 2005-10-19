#ifndef WORDSEGMENTER_H
#define WORDSEGMENTER_H

#include <map>
#include <string>

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
	bool segment(string& inputString, string& outputString);

private:
	map<string, Bin*> lm_;
};

#endif