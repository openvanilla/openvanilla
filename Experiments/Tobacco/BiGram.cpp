#include <algorithm>

#include "DictionarySingleton.h"
#include "BiGram.h"

BiGram::BiGram(void)
{
}

BiGram::~BiGram(void)
{
}

int BiGram::maximumMatching(vector<Token>& tokenVectorRef,
    int index, int stop, bool doBackward)
{
	DictionarySingleton* dictionary = DictionarySingleton::getInstance();

	vector<int> boundaryVector;
	vector< vector<Vocabulary> > vectorOfVocabularyVector;
	int begin = index;
	int end = stop;
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
		int offsetBound = stop - index;
		if(offsetBound > 7)
			offsetBound = 7;
		bool foundFlag = false;
		vector<string> initialCharacterStringVector = tokenVectorRef[index].characterStringVector;
		vector<string> foundCharacterStringVector;
		int currentIndex = index;

		while(!foundFlag)
		{
			int bound = 1;
			vector<string> currentCharacterStringVector = initialCharacterStringVector;

			for(int offset = 1; offset < offsetBound; ++offset)
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

			for(int i = 0; i < currentCharacterStringVector.size(); ++i)
			{
				string tokenSequence = currentCharacterStringVector[i];
				vector<Vocabulary> tempVocabularies;
				if(dictionary->getVocabularyVectorByCharacters(tokenSequence,
				    tempVocabularies))
				{
					foundFlag = true;
					foundCharacterStringVector.push_back(tokenSequence);
				}
			}

			if(foundFlag)
			{
				if(bound < 5)
				{
					for(int round = 0; round < bound; ++round)
					{
						int tokenIndex = currentIndex - round;
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
		for(int j = 0; j < foundCharacterStringVector.size(); ++j)
		{
			string tokenSequence = foundCharacterStringVector[j];
			vector<Vocabulary> vocabularies;
			dictionary->getVocabularyVectorByCharacters(tokenSequence,
			     vocabularies);

            for(int k = 0; k < vocabularies.size(); k++)
				currentVocabularyVector.push_back(vocabularies[k]);
		}

		stable_sort(currentVocabularyVector.begin(), currentVocabularyVector.end(), Vocabulary::isFreqGreater);
		if(currentVocabularyVector[0].word.length() == 1 && currentVocabularyVector[0].freq > 99) {
			int thrashold = 0;
			for(int step = 0; step < currentVocabularyVector.size(); step++)
			{
				if(currentVocabularyVector[step].freq < 100) {
					thrashold = step;
					break;
				}
			}
			
			if(thrashold > 0)
				currentVocabularyVector.erase(currentVocabularyVector.begin() + thrashold,
												currentVocabularyVector.end());
		}

		vectorOfVocabularyVector.push_back(currentVocabularyVector);

		index = currentIndex + 1;
	}

	vector<Vocabulary> combinedVocabularyVector = vectorOfVocabularyVector[0];
	for(int step = 1; step < vectorOfVocabularyVector.size(); ++step)
	{
		stable_sort(combinedVocabularyVector.begin(), combinedVocabularyVector.end(),
			Vocabulary::isFreqGreater);

		vector<Vocabulary> rightVocabularyVector = vectorOfVocabularyVector[step];
		stable_sort(rightVocabularyVector.begin(), rightVocabularyVector.end(),
			Vocabulary::isFreqGreater);

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

	stable_sort(combinedVocabularyVector.begin(), combinedVocabularyVector.end(),
		Vocabulary::isFreqGreater);

	Vocabulary bestVocabularyCombination = combinedVocabularyVector.front();

	int from = begin;
	int to = end;
	if(doBackward)
	{
		reverse(tokenVectorRef.begin(), tokenVectorRef.end());
		to = tokenVectorRef.size() - begin;
		from = tokenVectorRef.size() - end;
	}
	for(int pos = from; pos < to; ++pos)
		tokenVectorRef[pos].word = bestVocabularyCombination.word.at(pos - from);

	return bestVocabularyCombination.freq;
}

void BiGram::getCharacterCombination(
    vector<string>& leftRef, vector<string>& rightRef,
    vector<string>& combinedRef)
{
	for(int i = 0; i < leftRef.size(); ++i)
		for(int j = 0; j < rightRef.size(); ++j)
			combinedRef.push_back(leftRef[i] + "-" + rightRef[j]);
}

void BiGram::getVocabularyCombination(
    vector<Vocabulary>& leftRef, vector<Vocabulary>& rightRef,
    vector<Vocabulary>& combinedRef)
{
    vector<Vocabulary> combinedVocabularyVector;
	int leftBound = leftRef.size();
	if(leftBound > 5)
		leftBound = 5;
	int rightBound = rightRef.size();
	if(rightBound > 5)
		rightBound = 5;

	for(int i = 0; i < leftBound; ++i)
	{
		int matrix = 0;
		for(int j = 0; j < rightBound; ++j)
		{
			Vocabulary combinedVocabulary;
			combinedVocabulary.word = leftRef[i].word + rightRef[j].word;

			int leftFreq = leftRef[i].freq;
			int rightFreq = rightRef[j].freq;
			int wordCount = combinedVocabulary.word.length();
			matrix = leftFreq + rightFreq;
			if(matrix == 0)
				break;
			combinedVocabulary.freq = ((leftFreq * rightFreq) / (matrix * wordCount)) +
										(matrix / wordCount);
			combinedVocabularyVector.push_back(combinedVocabulary);
		}

		if(matrix == 0)
			break;
	}
	
	combinedRef = combinedVocabularyVector;
}