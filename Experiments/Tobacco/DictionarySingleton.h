#ifndef DICTIONARYSINGLETON_H
#define DICTIONARYSINGLETON_H

#include <string>
#include <vector>

#ifndef WIN32
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OVUtility.h"
	#pragma warning(disable:4996)
#endif

#include "OVSQLite3.h"

#include "Vocabulary.h"
#include "ProfileManager.h"

using namespace std;

class DictionarySingleton
{
public:
	static DictionarySingleton* getInstance(
		const char* dbFilePath)
	{
		if(itsInstance == NULL)
			itsInstance = new DictionarySingleton(dbFilePath);

		return itsInstance;
	}

	static void lostInstance();

	void setInputMethodId(string id) {
		inputMethodId = id;
		if(inputMethodId == "PhoneticHsu")
			inputMethodId  = "BoPoMoFo";

		sprintf(
			viewString,
			"SELECT w.word, c.logProb, c.backOff FROM %s_char2word_table c, word_table w WHERE c.characters=?1 AND c.wordId = w.wordId ORDER BY c.logProb DESC",
			inputMethodId.c_str());
		sprintf(
			viewLimitString,
			"SELECT w.word, c.logProb, c.backOff FROM %s_char2word_table c, word_table w WHERE c.characters=?1 AND c.wordId = w.wordId ORDER BY c.logProb DESC LIMIT %u",
			inputMethodId.c_str(),
			N_BEST);
	}

	void setImTableId(string id) {
		imTableId = id;
		sprintf(
			checkString,
			"SELECT value FROM %s WHERE key=?1 LIMIT 1",
			imTableId.c_str());
		sprintf(
			vocableString,
			"SELECT value, ord FROM %s WHERE key=?1 ORDER BY ord",
			imTableId.c_str());
	}

	bool isVocabulary(string keystrokes);
	bool getWordsByCharacters(string characters,
		vector<Vocabulary>& vocabularyVectorRef, bool isLimited);
	bool getVocablesByKeystrokes(string keystrokes,
		vector<Vocabulary>& vocabularyVectorRef);

	const static size_t N_BEST = 3;

protected:
	DictionarySingleton(const char* dbFilePath);
	~DictionarySingleton();
	
private:
	static DictionarySingleton* itsInstance;
	static SQLite3 *dictionaryDB;
	static SQLite3 *imTableDB;

	string inputMethodId;
	string imTableId;

	char checkString[256];
	char viewString[256];
	char viewLimitString[256];
	char vocableString[128];

	ProfileManager* m_profileManager;
};

#endif
