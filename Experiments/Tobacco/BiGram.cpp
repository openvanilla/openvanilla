//#define OV_DEBUG

#include <algorithm>
#include <math.h>
#include <limits>

#include "BiGram.h"

#ifndef WIN32
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OVUtility.h"
#endif

BiGram::BiGram()
{
}

BiGram::~BiGram()
{
}

double BiGram::viterbi(
	DictionarySingleton* dictionary, vector<Token>& tokenVectorRef,
	int begin, int end)
{
	int length = end - begin;
	vector<double> scores(length + 1, 0.0f);
	vector<int> tracks(length + 1, -1);
	vector<string> words(length + 1, "");
	
	murmur("begin:%i", begin);
	murmur("length:%i", length);

	for(int index = begin + 1; index <= end; index++)
	{
		murmur("index:%i", index);
		int innerIndex = index - begin;
		double bestScore = 0.0f;
		int bestPrefix = -1;
		string bestWord = "";
		for (int prefix = index - 1; prefix >= begin; prefix--)
		{
			murmur("prefix:%i", prefix);
			int innerPrefix = prefix - begin;
			string rightKey("");
			for (int shift = prefix; shift < index; shift++)
				rightKey +=
					tokenVectorRef[shift].characterStringVector[0] + '-';
			rightKey = rightKey.substr(0, rightKey.length() - 1);
			murmur("rightKey:%s", rightKey.c_str());

			vector<Vocabulary> rightGrams;
			dictionary->getWordsByCharacters(rightKey, rightGrams, true);
			size_t rightGramCount = rightGrams.size();
			murmur("rightGramCount:%i, original", rightGramCount);
			if(prefix == index - 1 && rightGramCount  == 0)
				dictionary->getVocablesByKeystrokes(
					tokenVectorRef[prefix].keystrokes, rightGrams);
			rightGramCount = rightGrams.size();
			murmur("rightGramCount:%i, single char", rightGramCount);
			vector<Vocabulary> filteredRightGrams;
			for (int shift = prefix; shift < index; shift++) {
				if (tokenVectorRef[shift].isFixed) {
					for (size_t k = 0; k < rightGrams.size(); k++)
						if (tokenVectorRef[shift].word ==
								rightGrams[k].word.substr(
									(shift - prefix) * 3, 3))
							filteredRightGrams.push_back(rightGrams[k]);
				}
			}
			if (filteredRightGrams.size() > 0)
				rightGrams = filteredRightGrams;
			rightGramCount = rightGrams.size();
			murmur("rightGramCount:%i, filtered", rightGramCount);

			for (size_t i = 0; i < rightGramCount; i++)
			{
				//string rightGram = rightGrams[i];
				//if(lm_->has(rightGram)) {
					int left = tracks[innerPrefix] + begin;
					murmur("left:%i", left);
					if(left >= 0 && left != prefix)
					{
						string leftKey("");
						for(int shift = left; shift < prefix; shift++)
							leftKey +=
								tokenVectorRef[shift].characterStringVector[0]
									+ '-';
						leftKey = leftKey.substr(0, leftKey.length() - 1);
						murmur("leftKey:%s", leftKey.c_str());

						vector<Vocabulary> leftGrams;
						dictionary->getWordsByCharacters(
							leftKey, leftGrams, true);
						size_t leftGramCount = leftGrams.size();
						murmur("leftGramCount:%i, original", leftGramCount);
						if (left == prefix - 1 && leftGramCount  == 0)
							dictionary->getVocablesByKeystrokes(
								tokenVectorRef[left].keystrokes, leftGrams);
						leftGramCount = leftGrams.size();
						murmur("leftGramCount:%i, single char", leftGramCount);
						vector<Vocabulary> filteredLeftGrams;
						for (int shift = left; shift < prefix; shift++) {
							if (tokenVectorRef[shift].isFixed) {
								for (size_t k = 0; k < leftGrams.size(); k++)
									if (tokenVectorRef[shift].word ==
											leftGrams[k].word.substr(
												(shift - left) * 3, 3))
										filteredLeftGrams.push_back(leftGrams[k]);
							}
						}
						if (filteredLeftGrams.size() > 0)
							leftGrams = filteredLeftGrams;
						leftGramCount = leftGrams.size();
						murmur("leftGramCount:%i, filtered", leftGramCount);

						for (size_t j = 0; j < leftGramCount; j++)
						{
							double tempScore = 0.0f;
							//string leftGram = leftGrams[j];
							//string bigram = leftGram + " " + rightGram;
							//if(lm_->has(bigram)) {
							//	double bigramScore = lm_->getLogProb(bigram);
							//	cerr << bigramScore << "(bigram) + ";
							//	tempScore += bigramScore;
							//}
							//else if(dic_->has(leftGram)) {
								double bigramBackOff =
									//lm_->getLogProb(leftGram) +
									//leftGrams[j].logProb +
									//lm_->getBackOff(rightGram);
									//rightGrams[i].backOff;
									rightGrams[i].logProb;
								tempScore += bigramBackOff;
								murmur("tempScore:%f", tempScore);								
							//}
							//else {
							//	tempScore +=
							//		lm_->getUnkLogProb() +
							//		lm_->getBackOff(rightGram);
							//}
							
							tempScore += scores[innerPrefix];
							if(bestScore == 0.0f || tempScore > bestScore)
							{
								bestScore = tempScore;
								bestPrefix = innerPrefix;
								bestWord = rightGrams[i].word;
							}
						}
					}
					else {
						murmur("else!");
						double tempScore =
							//lm_->getLogProb(rightGram);
							rightGrams[i].logProb;
						murmur("tempScore:%f", tempScore);
						if(bestScore == 0.0f || tempScore > bestScore)
						{
							bestScore = tempScore;
							bestPrefix = innerPrefix;
							bestWord = rightGrams[i].word;
						}				
					} 
				/*
				}
				else
				{
					double tempScore = lm_->getUnkLogProb() + scores[prefix];
					cerr << "unknown single char word:" << tempScore << endl;
					if(bestScore == 0.0f || tempScore > bestScore)
					{
						bestScore = tempScore;
						bestPrefix = prefix;
						bestWord = rightGram;
						cerr << "argmax=" << prefix;
						cerr << "," << bestScore;
						cerr << "," << bestWord << endl;
					}
				}
				*/
			}
		}
		scores[innerIndex] = bestScore;
		tracks[innerIndex] = bestPrefix;
		words[innerIndex] = bestWord;
		murmur("best score[%i]: %f", innerIndex, bestScore);
		murmur("best prefix[%i]: %i", innerIndex, bestPrefix);
		murmur("best word[%i]: %s", innerIndex, bestWord.c_str());

		murmur("innerIndex:%i", innerIndex);
		if(tracks[innerIndex] == -1)
			tracks[innerIndex] = innerIndex - 1;
		murmur("tracks[%i]:%i", innerIndex, tracks[innerIndex]);
	}
	
	/*
	size_t boundary = length;
	while(boundary > 0) {
		Token* token = new Token();
		token->word = words[boundary];
		outputTokens.push_back(token);
		boundary = tracks[boundary];
	}
	*/
	
	int rightBound = length;
	int leftBound = tracks[rightBound];
	murmur("init rightBound:%i", rightBound);
	murmur("init leftBound:%i", leftBound);
	while(leftBound >= 0) {
		murmur("bounds[left:%i, right:%i]", leftBound, rightBound);
		for(int back = rightBound - 1; back > leftBound - 1; back--) {
			if(back > leftBound)
				tokenVectorRef[back + begin].withPrefix = true;
			else
				tokenVectorRef[back + begin].withPrefix = false;
			if(back < rightBound - 1)
				tokenVectorRef[back + begin].withSuffix = true;
			else
				tokenVectorRef[back + begin].withSuffix = false;

			tokenVectorRef[back + begin].word =
				words[rightBound].substr((back - leftBound)*3, 3);
			murmur(
				"token[%i].word: %s",
				back + begin,
				tokenVectorRef[back + begin].word.c_str());
		}

		rightBound = leftBound;
		leftBound = tracks[rightBound];
	}
	murmur("=======================");

	return scores[length];
}

#if 0
size_t BiGram::maximumMatching(
    DictionarySingleton* dictionary, vector<Token>& tokenVectorRef,
    size_t index, size_t stop, bool doBackward)
{
	vector<int> boundaryVector;
	boundaryVector.reserve(MAX_CONTEXT_LENGTH);
	vector< vector<Vocabulary> > vectorOfVocabularyVector;
	vectorOfVocabularyVector.reserve(MAX_CONTEXT_LENGTH);
	size_t begin = index;
	size_t end = stop;
	if(doBackward)
	{
		reverse(tokenVectorRef.begin(), tokenVectorRef.end());
		begin = tokenVectorRef.size() - stop;
		end = tokenVectorRef.size() - index;
		index = begin;
		stop = end;
	}

	while(index < stop)
	{
		size_t offsetBound = stop - index;
		if(offsetBound > MAX_CONTEXT_LENGTH)
			offsetBound = MAX_CONTEXT_LENGTH;
		bool foundFlag = false;
		vector<string> initialCharacterStringVector =
            tokenVectorRef[index].characterStringVector;
		vector<string> foundCharacterStringVector;
		size_t currentIndex = index;

		while(!foundFlag)
		{
			size_t bound = 1;
			vector<string> currentCharacterStringVector = 
                initialCharacterStringVector;

			for(size_t offset = 1; offset < offsetBound; ++offset)
			{
				vector<string> newCharacterStringVector;
				currentIndex = index + offset;
				if(doBackward)
				{
					BiGram::getCharacterCombination(
						tokenVectorRef[currentIndex].characterStringVector,
						currentCharacterStringVector,
						newCharacterStringVector);
				}
				else
				{
					BiGram::getCharacterCombination(
						currentCharacterStringVector,
						tokenVectorRef[currentIndex].characterStringVector,
						newCharacterStringVector);
				}
				currentCharacterStringVector = newCharacterStringVector;

				bound = offset + 1;
			}

			for(size_t i = 0; i < currentCharacterStringVector.size(); ++i)
			{
				string tokenSequence = currentCharacterStringVector[i];
				if(dictionary->isVocabulary(tokenSequence))
				{
					foundFlag = true;
					foundCharacterStringVector.push_back(tokenSequence);
				}
			}

			if(foundFlag)
			{
				if(bound < MAX_CONTEXT_LENGTH)
				{
					for(size_t round = 0; round < bound; ++round)
					{
						size_t tokenIndex = currentIndex - round;
						if(round == bound - 1)
						{
							if(doBackward)
								tokenVectorRef[tokenIndex].withSuffix = false;
							else
								tokenVectorRef[tokenIndex].withPrefix = false;
						}
						else
						{
							if(doBackward)
								tokenVectorRef[tokenIndex].withSuffix = true;
							else
								tokenVectorRef[tokenIndex].withPrefix = true;
						}

						if(round == 0)
						{
							if(doBackward)
								tokenVectorRef[tokenIndex].withPrefix = false;
							else
								tokenVectorRef[tokenIndex].withSuffix = false;
						}
						else
						{
							if(doBackward)
								tokenVectorRef[tokenIndex].withPrefix = true;
							else
								tokenVectorRef[tokenIndex].withSuffix = true;
						}
					}
				}

				break;
			}
			else
			{
				--currentIndex;
				--offsetBound;

				if(offsetBound < 0 || currentIndex < 0)
				{
					if(doBackward)
						reverse(tokenVectorRef.begin(), tokenVectorRef.end());

					return 0;
				}
			}
		}

		vector<Vocabulary> currentVocabularyVector;
		currentVocabularyVector.reserve(
			DictionarySingleton::N_BEST * foundCharacterStringVector.size());
		for(size_t j = 0; j < foundCharacterStringVector.size(); ++j)
		{
			string tokenSequence = foundCharacterStringVector[j];
			vector<Vocabulary> vocabularies;
			vocabularies.reserve(DictionarySingleton::N_BEST);
			bool found = dictionary->getWordsByCharacters(
			    tokenSequence, vocabularies, true);

			//<comment author='b6s'> Shit happeds on Vista!
			if(!found) return -1;
			//</comment>

            for(size_t k = 0; k < vocabularies.size(); k++)
				currentVocabularyVector.push_back(vocabularies[k]);
		}

		size_t threshold = 0;
		if(currentVocabularyVector[0].freq > 199) {
			for(size_t step = 0; step < currentVocabularyVector.size(); step++)
			{
				if(currentVocabularyVector[step].freq < 200) {
					threshold = step;
					break;
				}
			}
		}	
		if(threshold > 0)
			currentVocabularyVector.erase(
				currentVocabularyVector.begin() + threshold,
				currentVocabularyVector.end());

		vectorOfVocabularyVector.push_back(currentVocabularyVector);

		index = currentIndex + 1;
	}

	vector<Vocabulary> combinedVocabularyVector = vectorOfVocabularyVector[0];
	for(size_t step = 1; step < vectorOfVocabularyVector.size(); ++step)
	{
		vector<Vocabulary> rightVocabularyVector =
            vectorOfVocabularyVector[step];

        vector<Vocabulary> leftVocabularyVector =
		  combinedVocabularyVector;
		if(doBackward)
            BiGram::getVocabularyCombination(
                rightVocabularyVector, leftVocabularyVector,
				combinedVocabularyVector);
		else
			BiGram::getVocabularyCombination(
				leftVocabularyVector, rightVocabularyVector,
				combinedVocabularyVector);
	}

	Vocabulary bestVocabularyCombination = combinedVocabularyVector.front();

	size_t from = begin;
	size_t to = end;
	if(doBackward)
	{
		reverse(tokenVectorRef.begin(), tokenVectorRef.end());
		to = tokenVectorRef.size() - begin;
		from = tokenVectorRef.size() - end;
	}
	size_t shift = 0;
	for(size_t pos = from; pos < to; ++pos)
	{
		tokenVectorRef[pos].word =
            bestVocabularyCombination.word.substr(shift, 3);
            
        shift += 3;
        /// It is BAD, really BAD... for UTF-8 Chinese only.
    }

	return bestVocabularyCombination.freq;
}
#endif

void BiGram::getCharacterCombination(
    vector<string>& leftRef, vector<string>& rightRef,
    vector<string>& combinedRef)
{
	for(size_t i = 0; i < leftRef.size(); ++i)
		for(size_t j = 0; j < rightRef.size(); ++j)
			combinedRef.push_back(leftRef[i] + "-" + rightRef[j]);
}

#if 0
void BiGram::getVocabularyCombination(
    vector<Vocabulary>& leftRef, vector<Vocabulary>& rightRef,
    vector<Vocabulary>& combinedRef)
{
    vector<Vocabulary> combinedVocabularyVector;
	//<comment author='b6s'> No needs to combine currently...
	size_t leftBound = 1;
	size_t rightBound = 1;
	//</comment>
	//size_t leftBound = leftRef.size();
	//if(leftBound > N_BEST)
	//	leftBound = N_BEST;
	//size_t rightBound = rightRef.size();
	//if(rightBound > N_BEST)
	//	rightBound = N_BEST;

	for(size_t i = 0; i < leftBound; ++i)
	{
		for(size_t j = 0; j < rightBound; ++j)
		{
			Vocabulary combinedVocabulary(
				leftRef[i].word + rightRef[j].word);

			size_t leftFreq = leftRef[i].freq;
			size_t rightFreq = rightRef[j].freq;
			size_t matrix = leftFreq + rightFreq;
			
			double score = 0.0;
			if(matrix > 0) {
				combinedVocabulary.freq = matrix;
				score = matrix;
			} else {
				combinedVocabulary.freq = 1;
				score = 0.001f;
			}

			combinedVocabulary.prob =
				-(log(score) / log(2.0));
			combinedVocabularyVector.push_back(combinedVocabulary);
		}
	}
	

	//<comment author='b6s'> The order was prepared by SQL and won't change
	//sort(
	//	combinedVocabularyVector.begin(),
	//	combinedVocabularyVector.end(),
	//	Vocabulary::isProbGreater);
	//</comment>

	combinedRef = combinedVocabularyVector;
}
#endif