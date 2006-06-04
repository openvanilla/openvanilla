#ifndef PREDICTORSINGLETON_H
#define PREDICTORSINGLETON_H

#include <vector>
#include <string>

#include "DictionarySingleton.h"
#include "Candidate.h"
#include "Token.h"

using namespace std;

class PredictorSingleton
{
public:
    string composedString;
	vector< vector<string> > vectorOfCharacterVector;
	vector<Token> tokenVector;
	vector<Candidate> candidateVector;
	vector<size_t> candidatePositionVector;

	static PredictorSingleton* getInstance(const char* dbFilePath)
	{
		if(itsInstance == NULL)
			itsInstance = new PredictorSingleton(dbFilePath);

		return itsInstance;
	}

	static void lostInstance();
    
	void setInputMethodId(string id) { dictionary->setInputMethodId(id); }
	bool setTokenVector(string currentSequence, size_t position, bool doReplace);
	void setFixedToken(
	   string currentSequence, string currentWord, size_t position);
	void setSingleCharacterWordCandidateVector(size_t position);
	void setMultiCharacterWordCandidateVector(size_t position);
	void setSelectedCandidate(size_t position, size_t selectedCandidateIndex);

	void removeWord(size_t position, bool delFlag);
	void clearAll();

protected:
	void addCandidates(string characters, size_t head, size_t length = 1);
    void setTokenVectorByBigram();
    void setComposedString();

	PredictorSingleton(const char* dbFilePath);
	~PredictorSingleton();

private:
	static PredictorSingleton* itsInstance;
	DictionarySingleton* dictionary;
};

#endif //#ifndef PREDICTORSINGLETON_H