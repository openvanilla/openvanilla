#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "Vocabulary.h"

class Candidate : public Vocabulary
{
public:
	Candidate();
	Candidate(Vocabulary& voc);
	~Candidate();

	size_t position;
};

struct CandidateType
{
	static const int WORD = 0;
	static const int VOCABLE = 1;
};
#endif