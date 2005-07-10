#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <string>

using namespace std;

class Vocabulary
{
public:
	Vocabulary();
	~Vocabulary();

	string word;
	int freq;

	static bool isFreqGreater(const Vocabulary& voc1, const Vocabulary& voc2);
	static bool isWordLonger(const Vocabulary& voc1, const Vocabulary& voc2);
};

#endif