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
	   string currentSequence, string currentWord, int position);
	void setSingleCharacterWordCandidateVector(int position);
	void setMultiCharacterWordCandidateVector(int position);
	void setSelectedCandidate(int position, int selectedCandidateIndex);

	void removeWord(size_t position, bool delFlag);
	void clearAll();

protected:
	void addCandidates(string characters, int head, int length = 1);
    void setTokenVectorByBigram();
    void setComposedString();

	PredictorSingleton(const char* dbFilePath);
	~PredictorSingleton();

private:
	static PredictorSingleton* itsInstance;
	DictionarySingleton* dictionary;
};

#endif //#ifndef PREDICTORSINGLETON_H