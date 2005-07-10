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
	vector< vector<string> > vectorOfCharacterVector;
	vector<Token> tokenVector;
	vector<Candidate> candidateVector;
	vector<int> candidatePositionVector;

	static PredictorSingleton* getInstance()
	{
		if(itsInstance == NULL)
			itsInstance = new PredictorSingleton();

		return itsInstance;
	}

	void lostInstance();

	void setCompositionString(string currentSequence, int position);
	void setCandidateVector(int position);
	void removeWord(int position, bool delFlag);
	void clearAll();
	void setSelectedCandidate(int position, int selectedCandidateIndex);
    void setTokenVectorByBigram();

protected:
	void addCandidates(string characters, int head);

	PredictorSingleton();
	~PredictorSingleton();

private:
	static PredictorSingleton* itsInstance;
	DictionarySingleton* dictionary;
};

#endif //#ifndef PREDICTORSINGLETON_H