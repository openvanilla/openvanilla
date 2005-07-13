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
	static DictionarySingleton* getInstance(
	   const char* dbFilePath, string inputMethodId)
	{
		if(itsInstance == NULL)
			itsInstance = new DictionarySingleton(dbFilePath, inputMethodId);

		return itsInstance;
	}

	void lostInstance();

    bool isVocabulary(string characters);
	bool getVocabularyVectorByCharacters(string characters,
	   vector<Vocabulary>& vocabularyVectorRef);

protected:
	DictionarySingleton(const char* dbFilePath, string inputMethodId);
	~DictionarySingleton();
	
private:
	static DictionarySingleton* itsInstance;
    static SQLite3 *dictionaryDB;
    string inputMethodId;
};

#endif