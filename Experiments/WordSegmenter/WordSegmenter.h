#ifndef WORDSEGMENTER_H
#define WORDSEGMENTER_H

#include <map>
#include <string>

using namespace std;

class WordSegmenter
{
public:
	bool loadModel(map<string, int>& lm);
	bool segment(string& inputString, string& outputString);

private:
	map<string, int> lm_;
};

#endif