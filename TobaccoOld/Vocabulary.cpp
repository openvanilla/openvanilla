#include "Vocabulary.h"

Vocabulary::Vocabulary()
{
	word = "";
	freq = 0;
}

Vocabulary::~Vocabulary()
{
	word.erase();
}

bool Vocabulary::isFreqGreater(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.freq > voc2.freq;
}

bool Vocabulary::isWordLonger(const Vocabulary& voc1, const Vocabulary& voc2)
{
	return voc1.word.length() > voc2.word.length();
}