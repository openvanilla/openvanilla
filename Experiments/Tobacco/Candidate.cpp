#include "Candidate.h"

Candidate::Candidate()
{
	position = -1;
}

Candidate::Candidate(Vocabulary& voc)
{
	word = voc.word;
	freq = voc.freq;
	position = -1;
}

Candidate::~Candidate()
{
}
