#include "Vocabulary.h"

Vocabulary::Vocabulary()
{
	word = "";
	freq = 0;
	length = 0;
	order = -1;
}

Vocabulary::~Vocabulary()
{
	word.erase();
}

bool Vocabulary::isFreqGreater(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.freq > voc2.freq;
}

bool Vocabulary::isOrderHigher(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.order < voc2.order;
}

bool Vocabulary::isWordLonger(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.word.length() > voc2.word.length();
}