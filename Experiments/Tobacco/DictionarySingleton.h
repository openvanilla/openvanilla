#ifndef DICTIONARYSINGLETON_H
#define DICTIONARYSINGLETON_H

#include <string>
#include <vector>

#include "OVUtility.h"
#include "OVSQLite3.h"

#include "Vocabulary.h"

using namespace std;

class DictionarySingleton
{
public:
	static DictionarySingleton* getInstance()
	{
		if(itsInstance == NULL)
			itsInstance = new DictionarySingleton();

		return itsInstance;
	}

	void lostInstance();

	bool getVocabularyVectorByCharacters(string characters,
	   vector<Vocabulary>& vocabularyVectorRef);

protected:
	DictionarySingleton();
	~DictionarySingleton();

private:
	static DictionarySingleton* itsInstance;
	SQLite3 *dictionaryDB;
    const char *QueryForCommand(SQLite3 *db, const char *command);
    const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key);	
};

#endif