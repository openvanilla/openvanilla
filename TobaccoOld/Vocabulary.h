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
	int order;

	static bool isOrderPrior(const Vocabulary& voc1, const Vocabulary& voc2);
	static bool isFreqGreater(const Vocabulary& voc1, const Vocabulary& voc2);
	static bool isWordLonger(const Vocabulary& voc1, const Vocabulary& voc2);
};

#endif