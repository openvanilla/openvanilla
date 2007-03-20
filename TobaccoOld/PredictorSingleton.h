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

	bool setTokenVector(
		string keystrokes, string currentSequence,
		size_t position, bool doReplace);
	void setFixedToken(
	   string currentSequence, string currentWord, size_t position);
	void setCandidateVector(size_t position);
	void setSelectedCandidate(size_t position, size_t selectedCandidateIndex);

	void removeWord(size_t position, bool delFlag);
	void clearAll();

	void rotateTopCandidates(size_t position);

protected:
	void addCandidates(string characters, size_t head, int type);
    void setTokenVectorByBigram();
    void setComposedString();

	PredictorSingleton(const char* dbFilePath);
	~PredictorSingleton();

private:
	const static size_t ROTATE_LIMIT = 3;
	const static size_t MAX_CONTEXT_LENGTH = 5;
	static PredictorSingleton* itsInstance;
	DictionarySingleton* dictionary;
};

#endif //#ifndef PREDICTORSINGLETON_H