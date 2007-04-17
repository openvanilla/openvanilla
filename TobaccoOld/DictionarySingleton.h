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
		strTableName = inputMethodId+"_char2word_table";
		strColumnWordID = strTableName+".wordID";
		strColumnCharacters = strTableName+".characters";

		checkString = "SELECT "+strColumnWordID + " FROM "+strTableName;

		selectString = "SELECT word_table.word,generic_freq_table.freq";
		fromString = " FROM "+strTableName+",word_table,generic_freq_table";
		joinString =
			" AND "+strColumnWordID+"=word_table.wordID"+
			" AND "+strColumnWordID+"=generic_freq_table.wordID";
		orderString = " ORDER BY generic_freq_table.freq DESC";
		orderLimitString = orderString+" LIMIT 10";
	}
	void setImTableId(string id) { imTableId = id; }

	bool isVocabulary(string characters);
	bool getWordsByCharacters(string characters,
		vector<Vocabulary>& vocabularyVectorRef, bool isLimited);
	bool getVocablesByCharacters(string characters,
		vector<Vocabulary>& vocabularyVectorRef);

protected:
	DictionarySingleton(const char* dbFilePath);
	~DictionarySingleton();
	
private:
	static DictionarySingleton* itsInstance;
	static SQLite3 *dictionaryDB;
	static SQLite3 *imTableDB;
	string inputMethodId;
	string imTableId;
    string strTableName;
    string strColumnWordID;
    string strColumnCharacters;

	string checkString;

	string selectString;
	string fromString;
	string joinString;
	string orderString;
	string orderLimitString;
};

#endif
