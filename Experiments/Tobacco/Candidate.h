#ifndef CANDIDATE_H
#define CANDIDATE_H

#include "Vocabulary.h"

class Candidate : public Vocabulary
{
public:
	Candidate(const string& aWord);
	Candidate(Vocabulary& aVocabulary);

	size_t position;
};

struct CandidateType
{
	static const int WORD = 0;
	static const int VOCABLE = 1;
};
#endif