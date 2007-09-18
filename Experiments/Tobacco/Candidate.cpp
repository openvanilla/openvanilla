#include "Candidate.h"

Candidate::Candidate(const string& aWord) : Vocabulary(aWord), position(0)
{}

Candidate::Candidate(Vocabulary& aVocabulary) :
	Vocabulary(aVocabulary.word),
	position(0)
{
	this->order = aVocabulary.order;
	this->logProb = aVocabulary.logProb;
	this->backOff = aVocabulary.backOff;
}
