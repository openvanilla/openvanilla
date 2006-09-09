#include "Candidate.h"

Candidate::Candidate()
{
	position = 0;
}

Candidate::Candidate(Vocabulary& voc)
{
	word = voc.word;
	freq = voc.freq;
	position = 0;
}

Candidate::~Candidate()
{
}
