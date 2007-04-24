#ifndef DICTIONARYSINGLETON_H
#define DICTIONARYSINGLETON_H

#include <string>
#include <vector>

#ifndef WIN32
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OVUtility.h"
#endif

#include "OVSQLite3.h"

#include "Vocabulary.h"

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
			checkString,
			"SELECT wordID FROM %s_char2word_table WHERE characters=?1 LIMIT 1;",
			inputMethodId.c_str());
		sprintf(
			viewString,
			"SELECT word, freq FROM %s_view WHERE characters=?1",
			inputMethodId.c_str());
		sprintf(
			viewLimitString,
			"SELECT word, freq FROM %s_view WHERE characters=?1 LIMIT %s",
			inputMethodId.c_str(),
			N_BEST);
	}

	void setImTableId(string id) {
		imTableId = id;
		sprintf(
			vocableString,
			"SELECT value, ord FROM %s WHERE key=?1 ORDER BY ord;",
			imTableId.c_str());
	}

	bool isVocabulary(string characters);
	bool getWordsByCharacters(string characters,
		vector<Vocabulary>& vocabularyVectorRef, bool isLimited);
	bool getVocablesByCharacters(string characters,
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
};

#endif
