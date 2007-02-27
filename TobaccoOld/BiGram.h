#ifndef BIGRAM_H
#define BIGRAM_H

#include <vector>

#include "Token.h"
#include "Vocabulary.h"
#include "DictionarySingleton.h"

class BiGram
{
public:
	BiGram();
	~BiGram();

	int maximumMatching(
	   DictionarySingleton* dictionary, vector<Token>& tokenVectorRef,
	   size_t index, size_t length, bool doBackward);
	void getCharacterCombination(
	   vector<string>& leftRef, vector<string>& rightRef,
	   vector<string>& combinedRef);
	void getVocabularyCombination(
	   vector<Vocabulary>& leftRef, vector<Vocabulary>& rightRef,
	   vector<Vocabulary>& combinedRef);

private:
	const static size_t N_BEST = 5;
};

#endif