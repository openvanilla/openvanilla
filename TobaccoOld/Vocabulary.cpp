#include "Vocabulary.h"

Vocabulary::Vocabulary(const string& aWord) :
	word(aWord), freq(0), order(0), prob(0.0)
{}

bool Vocabulary::isOrderPrior(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.order < voc2.order;
}

bool Vocabulary::isFreqGreater(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.freq > voc2.freq;
}

bool Vocabulary::isProbGreater(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.prob < voc2.prob;
}

bool Vocabulary::isWordLonger(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.word.length() > voc2.word.length();
}