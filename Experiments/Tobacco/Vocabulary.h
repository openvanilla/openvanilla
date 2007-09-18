#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <string>

using namespace std;

class Vocabulary
{
public:
	Vocabulary(const string& aWord);

	string word;
	size_t order;

	double logProb;
	double backOff;

	static bool isOrderPrior(const Vocabulary& voc1, const Vocabulary& voc2);
	static bool isProbGreater(const Vocabulary& voc1, const Vocabulary& voc2);
	static bool isWordLonger(const Vocabulary& voc1, const Vocabulary& voc2);
};

#endif