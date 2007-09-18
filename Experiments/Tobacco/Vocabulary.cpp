#include "Vocabulary.h"

Vocabulary::Vocabulary(const string& aWord) :
	word(aWord), order(0), logProb(0.0f), backOff(0.0f)
{}

bool Vocabulary::isOrderPrior(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.order < voc2.order;
}

bool Vocabulary::isProbGreater(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.logProb < voc2.logProb;
}

bool Vocabulary::isWordLonger(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.word.length() > voc2.word.length();
}