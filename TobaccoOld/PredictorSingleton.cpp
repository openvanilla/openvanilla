//#define OV_DEBUG

#include "BiGram.h"
#include "PredictorSingleton.h"

#ifndef WIN32
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OpenVanilla.h"
#endif

#include <algorithm>

using namespace std;

PredictorSingleton* PredictorSingleton::itsInstance = NULL;

PredictorSingleton::PredictorSingleton(const char* dbFilePath)
{
	dictionary =
	   DictionarySingleton::getInstance(dbFilePath);
}

PredictorSingleton::~PredictorSingleton()
{
	clearAll();
	dictionary->lostInstance();
}

void PredictorSingleton::lostInstance()
{
	if(itsInstance)
	{
	   delete itsInstance;
	}
}

void PredictorSingleton::clearAll()
{
    composedString.erase();
	vectorOfCharacterVector.clear();
	tokenVector.clear();
	candidateVector.clear();
	candidatePositionVector.clear();
}

void PredictorSingleton::removeWord(size_t position, bool delFlag)
{
	size_t tokenIndex;
	if(delFlag)	// true	: DEL
		tokenIndex = position;
	else		// false: BackSpace
	{
		if(position > 0)
			tokenIndex = position - 1;
		else
			tokenIndex = position;
	}

	tokenVector.erase(
		tokenVector.begin() + tokenIndex);

	if(tokenIndex > 0)
		tokenVector[tokenIndex - 1].withSuffix = false;
	if(static_cast<int>(tokenIndex) <
		static_cast<int>(tokenVector.size()) - 1)
		tokenVector[tokenIndex + 1].withPrefix = false;

	setTokenVectorByBigram();
}

bool PredictorSingleton::setTokenVector(
    string keystrokes, string currentSequence,
	size_t position, bool doReplace)
{
    if(!dictionary->isVocabulary(currentSequence))
        return false;

	Token currentToken;
	currentToken.withPrefix = false;
	currentToken.withSuffix = false;
	currentToken.characterStringVector.push_back(currentSequence);
	currentToken.keystrokes = keystrokes;

    if(doReplace)
    {
        if(tokenVector.size() > position)
			tokenVector[position] = currentToken;
        else if(tokenVector.size() == position)
            tokenVector.insert(
                tokenVector.begin() + position,
                currentToken);            
        else
            return false;
    }
    else
        tokenVector.insert(
			tokenVector.begin() + position, currentToken);

    PredictorSingleton::setTokenVectorByBigram();
    
    return true;
}

void PredictorSingleton::setFixedToken(
    string currentSequence, string currentWord, size_t position)
{
	Token currentToken;
	currentToken.withPrefix = false;
	currentToken.withSuffix = false;
	currentToken.isFixed = true;
	currentToken.isBoundary = true;
	currentToken.characterStringVector.push_back(currentSequence);
	currentToken.word = currentWord;

    tokenVector.insert(
		tokenVector.begin() + position, currentToken);

    setTokenVectorByBigram();
}

void PredictorSingleton::rotateTopCandidates(size_t position)
{
	vector<Vocabulary> vocabularies;
	dictionary->getVocablesByCharacters(
		tokenVector[position].keystrokes, vocabularies);
	if(vocabularies.size() < ROTATE_LIMIT - 1)
		return;

	/*
	sort(
		vocabularies.begin(),
		vocabularies.end(),
		Vocabulary::isFreqGreater);
	*/

	size_t index = ROTATE_LIMIT;
	for(size_t j = 0;
		j < vocabularies.size() && j < ROTATE_LIMIT;
		j++)
		if(tokenVector[position].word == vocabularies[j].word)
			index = j;
	if(index == ROTATE_LIMIT || index == ROTATE_LIMIT - 1)
		tokenVector[position].word = vocabularies[0].word;
	else
		tokenVector[position].word = vocabularies[index + 1].word;

	tokenVector[position].isFixed = true;
    setTokenVectorByBigram();
	for(size_t k = 0; k < position; k++)
		tokenVector[k].isFixed = true;

    setComposedString();
}

void PredictorSingleton::addCandidates(
	string characters, size_t head, int type)
{
	vector<Vocabulary> vocabularies;

	bool hasCandidate = false;
	switch(type) {
		case CandidateType::WORD:
			hasCandidate =
				dictionary->getWordsByCharacters(
					characters, vocabularies);
			break;
		case CandidateType::VOCABLE:
		default:
			hasCandidate =
				dictionary->getVocablesByCharacters(
					characters, vocabularies);
			break;
	}

	if(hasCandidate)
	{
		for(size_t i = 0; i < vocabularies.size(); i++)
		{
			Candidate currentCandidate(vocabularies[i]);
			currentCandidate.position = head;
			candidateVector.push_back(currentCandidate);
			candidatePositionVector.push_back(head);
		}
	}
	
	/*
	sort(
	   candidateVector.begin(),
	   candidateVector.end(),
	   Vocabulary::isFreqGreater);

	stable_sort(
	   candidateVector.begin(),
	   candidateVector.end(),
	   Vocabulary::isWordLonger);
   */
}

void PredictorSingleton::setCandidateVector(size_t position)
{	
	candidateVector.clear();
	candidatePositionVector.clear();

	BiGram biGram;
	vector<string> currentCharacterCombinationVector =
		tokenVector[position].characterStringVector;

	size_t backwardPosition = position;
	size_t bound =
		position >= BiGram::MAX_CONTEXT_LENGTH ?
			position - BiGram::MAX_CONTEXT_LENGTH : 0;
	while(backwardPosition > bound)
		//&& !tokenVector[backwardPosition - 1].isBoundary)
		//&& tokenVector[backwardPosition].withPrefix)
	{
		vector<string> newCharacterCombinationVector;
		biGram.getCharacterCombination(
			tokenVector[--backwardPosition].characterStringVector,
			currentCharacterCombinationVector,
			newCharacterCombinationVector);
		currentCharacterCombinationVector = newCharacterCombinationVector;

		for(size_t backwardStep = 0;
			backwardStep < currentCharacterCombinationVector.size();
			++backwardStep)
			addCandidates(
				currentCharacterCombinationVector[backwardStep],
				backwardPosition,
				CandidateType::WORD);
	}

	addCandidates(
		tokenVector[position].keystrokes,
		position,
		CandidateType::VOCABLE);
}

void PredictorSingleton::setSelectedCandidate(
    size_t index, size_t selectedCandidateIndex)
{
	string selectedCandidateWordString =
		candidateVector[selectedCandidateIndex].word;
	size_t head =
		candidateVector[selectedCandidateIndex].position;
	for(size_t i = head, offset = 0;
		i <= index;
		i++, offset += BiGram::MAX_CONTEXT_LENGTH - 1)
	{
		tokenVector[i].isFixed = true;
		tokenVector[i].word =
			selectedCandidateWordString.substr(
				offset, BiGram::MAX_CONTEXT_LENGTH - 1);
	}
	
	setComposedString();
}

void PredictorSingleton::setTokenVectorByBigram()
{
	BiGram biGram;
	size_t begin = 0;
	size_t end = 0;
	while(end <= tokenVector.size())
	{
		if(end == tokenVector.size() && end > begin)
		{
			//vector<Token> forwardTokenVector(PredictorSingleton::tokenVector);
			/*
			int backwardScore =
                biGram.maximumMatching(
                    dictionary,
                    tokenVector,
                    begin, end, true);
			*/
			int forwardScore =
                biGram.maximumMatching(
                    PredictorSingleton::dictionary,
                    tokenVector,//forwardTokenVector,
                    begin, end, false);
			/*
			if(forwardScore > backwardScore)
				PredictorSingleton::tokenVector = forwardTokenVector;
            else
                forwardTokenVector.clear();
			*/
		}
		else if(end == tokenVector.size())
		{
            break;
		}
		else if(tokenVector[end].isFixed ||
				tokenVector[end].isBoundary)
		{
			size_t length;
			if(PredictorSingleton::tokenVector[end].isFixed)
				length = end;
			else
				length = end + 1;

			if(length > begin)
			{
				//vector<Token> forwardTokenVector(
				//    PredictorSingleton::tokenVector);
				/*
    			int backwardScore =
                    biGram.maximumMatching(
                        dictionary,
                        tokenVector,
                        begin, length, true);
				*/

    			int forwardScore =
                    biGram.maximumMatching(
                        PredictorSingleton::dictionary,
                        tokenVector,//forwardTokenVector,
                        begin, length, false);
				/*
    			if(forwardScore > backwardScore)
	       			PredictorSingleton::tokenVector = forwardTokenVector;
                else
                    forwardTokenVector.clear();
				*/
			}

			begin = end + 1;
		}

		++end;
	}

	//<comment author='b6s' date='20070320'>
	// Set tokens fixed when the vector size reachs MAX_CONTEXT_LENGTH.
	if(tokenVector.size() == BiGram::MAX_CONTEXT_LENGTH ||
		(tokenVector.size() > BiGram::MAX_CONTEXT_LENGTH &&
		tokenVector.size() % BiGram::MAX_CONTEXT_LENGTH == 0)) {
		int step = BiGram::MAX_CONTEXT_LENGTH;
		vector<Token>::iterator iter = tokenVector.end();
		while(step > 0) {
			iter--;
			iter->isFixed = true;
			step--;			
		}
	}
	//</comment>

	setComposedString();
}

void PredictorSingleton::setComposedString()
{
    composedString.erase();
    for(size_t i = 0; i < tokenVector.size(); i++)
		composedString +=
			tokenVector[i].word;
}
