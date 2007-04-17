//#define OV_DEBUG

#include <vector>

#include "DictionarySingleton.h"

#ifndef WIN32
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OVUtility.h"
#endif

using namespace std;

DictionarySingleton* DictionarySingleton::itsInstance = NULL;
SQLite3* DictionarySingleton::dictionaryDB = NULL;
SQLite3* DictionarySingleton::imTableDB = NULL;

DictionarySingleton::DictionarySingleton(
    const char* dbFilePath)
{
    murmur("new DictionarySingleton");

	int err = 0;

	char tsiDbFilePath[256];
	sprintf(tsiDbFilePath, "%s/tsi.db", dbFilePath);
    dictionaryDB = new SQLite3;
    if (err = dictionaryDB->open(tsiDbFilePath))
        murmur("SQLite3 error! code=%d", err);

	char imDbFilePath[256];
	sprintf(imDbFilePath, "%s/imtables.db", dbFilePath);
	imTableDB = new SQLite3;
    if (err = imTableDB->open(imDbFilePath))
        murmur("SQLite3 error! code=%d", err);
}

DictionarySingleton::~DictionarySingleton()
{
    delete DictionarySingleton::dictionaryDB;
}

void DictionarySingleton::lostInstance()
{
	if(DictionarySingleton::itsInstance)
	{
		delete DictionarySingleton::itsInstance;
	}
}

bool DictionarySingleton::isVocabulary(string characters)
{
	//<comment author='b6s'> These strings should be set only once!
	//string strTableName = DictionarySingleton::inputMethodId;
	//strTableName += "_char2word_table";
	//string strColumnWordID = strTableName + ".wordID";
	//string strColumnCharacters = strTableName + ".characters";
	//</comment>
      
	string whereString = " WHERE "+strColumnCharacters+"='"+characters+"'";
    string commandString = checkString + whereString;

    SQLite3Statement *sth =
        DictionarySingleton::dictionaryDB->prepare(commandString.c_str());
    if (!sth) {
        murmur("illegal SQL statement[%s]?", commandString.c_str());
        return false;
    }

    if (sth->step() == SQLITE_ROW) {
		murmur("found!");
		delete sth;
		return true;
    }
    else
    {
		murmur("not found...");
		delete sth;
		return false;
    }
}

bool DictionarySingleton::getWordsByCharacters(string characters,
    vector<Vocabulary>& vocabularyVectorRef)
{
    /// characters-word table schema:    |characters|wordID|
    /// word table schema:               |wordID|word|
    /// frequency table schema:          |wordID|freq|
    ///
    /// The reasons why to separate into 3 tables are:
    /// 1. Characters can be different by different input methods.
    /// 2. Different frequency table represents different "context" or
    ///    "user profile".
    ///
    /// A SQL statement for example:
    /// SELECT word_table.word, generic_freq_table.freq
    /// FROM BoPoMoFo_char2word_table, word_table, generic_freq_table
    /// WHERE BoPoMoFo_char2word_table.characters = '£t£«£¿'
    ///     AND BoPoMoFo_char2word_table.wordID = word_table.wordID
    ///     AND BoPoMoFo_char2word_table.wordID = generic_freq_table.wordID
    ///
    /// Since there're two inner joins,
    /// the order of tables and columns are very very important.

	//<comment author='b6s'> These strings should be set only once!
	//string strTableName = DictionarySingleton::inputMethodId;
	//strTableName += "_char2word_table";
	//string strColumnWordID = strTableName + ".wordID";    
	//string strColumnCharacters = strTableName + ".characters";
	//<comment>
      
    /// bind_foo seems not work on table/column name (sure it can't!),
    /// so use stupid concat...
	string whereString = " WHERE "+strColumnCharacters+"='"+characters+"'";
	//<comment author='b6s'> Sort them later.
    //whereString += " ORDER BY generic_freq_table.freq DESC";
	//</comment>
	string commandString =
		selectString + fromString + whereString + joinString;

    SQLite3Statement *sth =
        dictionaryDB->prepare(commandString.c_str());
    if (!sth) {
        murmur("illegal SQL statement[%s]?", commandString.c_str());
        return false;
    }

    int rows = 0;
    while (sth->step() == SQLITE_ROW) {
		rows++;
        const char* word = sth->column_text(0);
        murmur("found[%s]", word);
        int freq = sth->column_int(1);
        Vocabulary currentVocabulary;
        currentVocabulary.word = string(word);
        currentVocabulary.freq = freq;
        
        vocabularyVectorRef.push_back(currentVocabulary);
    }

	if (!rows) {
		delete sth;
        return false;
    }
    murmur("query string=%s, number of candidates=%d",
        characters.c_str(), rows);

	delete sth;
    return true;
}

bool DictionarySingleton::getVocablesByCharacters(string characters,
    vector<Vocabulary>& vocabularyVectorRef)
{
	char cmd[512];
	sprintf(
		cmd,
		"SELECT value, ord FROM %s WHERE key=?1 ORDER BY ord;",
		imTableId.c_str());

    SQLite3Statement *sth =
		imTableDB->prepare(cmd);
    if (!sth) return false;
	sth->bind_text(1, characters.c_str());
       
    int rows=0;
    while (sth->step()==SQLITE_ROW) {
		rows++;
        const char* word = sth->column_text(0);
        murmur("found[%s]", word);
        int order = sth->column_int(1);
        Vocabulary currentVocabulary;
        currentVocabulary.word = string(word);
		currentVocabulary.order = order;
        
        vocabularyVectorRef.push_back(currentVocabulary);
    }
    if (!rows) {
        delete sth;
        return false;
    }

	delete sth;
    return true;
}
