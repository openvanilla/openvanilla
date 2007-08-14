#include "Candidate.h"

Candidate::Candidate(const string& aWord) : Vocabulary(aWord), position(0)
{}

Candidate::Candidate(Vocabulary& aVocabulary) :
	Vocabulary(aVocabulary.word),
	position(0)
{
	this->freq = aVocabulary.freq;
	this->order = aVocabulary.order;
	this->prob = aVocabulary.prob;
}
