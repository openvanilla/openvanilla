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

	double viterbi(
		DictionarySingleton* dictionary, vector<Token>& tokenVectorRef,
		int begin, int end);
	size_t maximumMatching(
		DictionarySingleton* dictionary, vector<Token>& tokenVectorRef,
		size_t begin, size_t end, bool doBackward);
	void getCharacterCombination(
		vector<string>& leftRef, vector<string>& rightRef,
		vector<string>& combinedRef);
	void getVocabularyCombination(
		vector<Vocabulary>& leftRef, vector<Vocabulary>& rightRef,
		vector<Vocabulary>& combinedRef);

	const static size_t MAX_CONTEXT_LENGTH = 7;
};

#endif