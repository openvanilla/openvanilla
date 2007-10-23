#ifndef PREDICTORSINGLETON_H
#define PREDICTORSINGLETON_H

#include <vector>
#include <string>
#include <map>
#include <queue>

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
	map< size_t, queue<string> > top3CandidateQueueMap;

	static PredictorSingleton* getInstance(const char* dbFilePath)
	{
		if(itsInstance == NULL)
			itsInstance = new PredictorSingleton(dbFilePath);

		return itsInstance;
	}

	static void lostInstance();
    
	void setInputMethodId(string id) { dictionary->setInputMethodId(id); }
	void setImTableId(string id) { dictionary->setImTableId(id); }

	bool setTokenVector(string, string, size_t, bool);
	void setFixedToken(string, string, size_t);
	void setCandidateVector(size_t);
	void setSelectedCandidate(size_t, size_t);

	void removeWord(size_t, bool);
	void clearAll();

	void rotateTopCandidates(size_t);

protected:
	void addCandidates(string, size_t, int);
    void setTokenVectorByBigram();
    void setComposedString();

	PredictorSingleton(const char*);
	~PredictorSingleton();

private:
	const static size_t ROTATE_LIMIT = 3;
	static PredictorSingleton* itsInstance;
	DictionarySingleton* dictionary;
};

#endif //#ifndef PREDICTORSINGLETON_H