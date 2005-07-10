#ifndef BIGRAM_H
#define BIGRAM_H

#include <vector>

#include "Token.h"
#include "Vocabulary.h"

class BiGram
{
public:
	BiGram(void);
	~BiGram(void);

	int maximumMatching(
	   vector<Token>& tokenVectorRef, int index, int length, bool doBackward);
	void getCharacterCombination(
	   vector<string>& leftRef, vector<string>& rightRef,
	   vector<string>& combinedRef);
	void getVocabularyCombination(
	   vector<Vocabulary>& leftRef, vector<Vocabulary>& rightRef,
	   vector<Vocabulary>& combinedRef);
};

#endif