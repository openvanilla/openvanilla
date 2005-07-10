#include <algorithm>	// stable_sort()

#include "BiGram.h"
#include "PredictorSingleton.h"

using namespace std;

PredictorSingleton* PredictorSingleton::itsInstance = NULL;

PredictorSingleton::PredictorSingleton()
{
	PredictorSingleton::dictionary = DictionarySingleton::getInstance();
}

PredictorSingleton::~PredictorSingleton()
{
	PredictorSingleton::clearAll();
	PredictorSingleton::dictionary->lostInstance();
}

void PredictorSingleton::lostInstance()
{
	if(PredictorSingleton::itsInstance)
	{
	   delete PredictorSingleton::itsInstance;
	}
}

void PredictorSingleton::clearAll()
{
    PredictorSingleton::composedString.clear();
	PredictorSingleton::vectorOfCharacterVector.clear();
	PredictorSingleton::tokenVector.clear();
	PredictorSingleton::candidateVector.clear();
	PredictorSingleton::candidatePositionVector.clear();
}

void PredictorSingleton::removeWord(int position, bool delFlag)
{
	int tokenIndex = -1;
	if(delFlag)	// true	: DEL
		tokenIndex = position;
	else		// false: BackSpace
	{
		if(position != 0)
			tokenIndex = position - 1;
		else
			tokenIndex = position;
	}

	PredictorSingleton::tokenVector.erase(
	   PredictorSingleton::tokenVector.begin() + tokenIndex);

	if(tokenIndex > 0)
		PredictorSingleton::tokenVector[tokenIndex - 1].withSuffix = false;
	if(tokenIndex < PredictorSingleton::tokenVector.size() - 1)
		PredictorSingleton::tokenVector[tokenIndex + 1].withPrefix = false;

	PredictorSingleton::setTokenVectorByBigram();
}

void PredictorSingleton::setTokenVector(
    string currentSequence, int position)
{
	Token currentToken;
	currentToken.withPrefix = false;
	currentToken.withSuffix = false;
	currentToken.characterStringVector.push_back(currentSequence);

    PredictorSingleton::tokenVector.insert(
        PredictorSingleton::tokenVector.begin() + position, currentToken);

    PredictorSingleton::setTokenVectorByBigram();    
}

void PredictorSingleton::addCandidates(string characters, int head)
{
    vector<Vocabulary> vocabularies;
	if(PredictorSingleton::dictionary->getVocabularyVectorByCharacters(
	   characters, vocabularies))
	{
		for(int i = 0; i < vocabularies.size(); i++)
		{
			Candidate currentCandidate(vocabularies[i]);
			currentCandidate.position = head;
			PredictorSingleton::candidateVector.push_back(currentCandidate);
			PredictorSingleton::candidatePositionVector.push_back(head);
		}
	}
}

void PredictorSingleton::setCandidateVector(int position)
{	
	PredictorSingleton::candidateVector.clear();
	PredictorSingleton::candidatePositionVector.clear();

	int candidateKeyCount =
	   PredictorSingleton::tokenVector[position].characterStringVector.size();
	for(int candidateKeyIndex = 0; candidateKeyIndex < candidateKeyCount; ++candidateKeyIndex)
	{
		string currentCharacterString =
		  PredictorSingleton::tokenVector[position].characterStringVector[candidateKeyIndex];
		PredictorSingleton::addCandidates(currentCharacterString, position);
	}

	BiGram biGram;
	vector<string> currentCharacterCombinationVector =
		PredictorSingleton::tokenVector[position].characterStringVector;

	int backwardPosition = position;
	int bound = position - 4 > 0 ? position - 4 : 0;
	while(backwardPosition > bound) //&& !PredictorSingleton::tokenVector[backwardPosition - 1].isBoundary) //&& PredictorSingleton::tokenVector[backwardPosition].withPrefix)
	{
		vector<string> newCharacterCombinationVector;
		biGram.getCharacterCombination(
			PredictorSingleton::tokenVector[--backwardPosition].characterStringVector,
			currentCharacterCombinationVector,
			newCharacterCombinationVector);
		currentCharacterCombinationVector = newCharacterCombinationVector;

		for(int backwardStep = 0; backwardStep < currentCharacterCombinationVector.size(); ++backwardStep)
			PredictorSingleton::addCandidates(currentCharacterCombinationVector[backwardStep], backwardPosition);
	}

	stable_sort(PredictorSingleton::candidateVector.begin(),
	   PredictorSingleton::candidateVector.end(),
	   Vocabulary::isFreqGreater);
	stable_sort(PredictorSingleton::candidateVector.begin(),
	   PredictorSingleton::candidateVector.end(),
	   Vocabulary::isWordLonger);
}

void PredictorSingleton::setSelectedCandidate(
    int index, int selectedCandidateIndex)
{
	string selectedCandidateWordString =
		PredictorSingleton::candidateVector[selectedCandidateIndex].word;
	int head =
	   PredictorSingleton::candidateVector[selectedCandidateIndex].position;
	for(int i = head, offset = 0; i <= index; ++i, ++offset)
	{
		PredictorSingleton::tokenVector[i].isFixed = true;
		PredictorSingleton::tokenVector[i].word =
			selectedCandidateWordString.substr(offset, 1);
	}
	
	PredictorSingleton::setComposedString();
}

void PredictorSingleton::setTokenVectorByBigram()
{
	BiGram biGram;
	int begin = 0;
	int end = 0;
	while(end <= PredictorSingleton::tokenVector.size())
	{
		if(end == PredictorSingleton::tokenVector.size() && end > begin)
		{
			vector<Token> forwardTokenVector(PredictorSingleton::tokenVector);
			int backwardScore = biGram.maximumMatching(PredictorSingleton::tokenVector, begin, end, true);
			int forwardScore = biGram.maximumMatching(forwardTokenVector, begin, end, false);
			if(forwardScore > backwardScore)
				PredictorSingleton::tokenVector = forwardTokenVector;
		}
		else if(PredictorSingleton::tokenVector[end].isFixed ||
				PredictorSingleton::tokenVector[end].isBoundary)
		{
			int length = -1;
			if(PredictorSingleton::tokenVector[end].isFixed)
				length = end;
			else
				length = end + 1;

			if(length > begin)
			{
				vector<Token> forwardTokenVector(PredictorSingleton::tokenVector);
				int backwardScore = biGram.maximumMatching(PredictorSingleton::tokenVector, begin, end, true);
				int forwardScore = biGram.maximumMatching(forwardTokenVector, begin, end, false);
				if(forwardScore > backwardScore)
					PredictorSingleton::tokenVector = forwardTokenVector;
			}

			begin = end + 1;
		}

		++end;
	}
	
	PredictorSingleton::setComposedString();
}

void PredictorSingleton::setComposedString()
{
    PredictorSingleton::composedString.clear();
    for(int i = 0; i < PredictorSingleton::tokenVector.size(); i++)
        PredictorSingleton::composedString +=
            PredictorSingleton::tokenVector[i].word;
}