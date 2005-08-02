#define OV_DEBUG

#include <vector>

#include "DictionarySingleton.h"

//#include <OpenVanilla/OVUtility.h>
#include "OVUtility.h"

using namespace std;

DictionarySingleton* DictionarySingleton::itsInstance = NULL;
SQLite3* DictionarySingleton::dictionaryDB = NULL;

DictionarySingleton::DictionarySingleton(
    const char* dbFilePath, string inputMethodId)
{
    murmur("new DictionarySingleton");

    DictionarySingleton::dictionaryDB = new SQLite3;
    DictionarySingleton::inputMethodId = inputMethodId;
    if (int err = DictionarySingleton::dictionaryDB->open(dbFilePath)) {
        murmur("SQLite3 error! code=%d", err);
    }
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
    string strTableName = DictionarySingleton::inputMethodId;
    strTableName += "_char2word_table";
    string strColumnWordID = strTableName + ".wordID";    
    string strColumnCharacters = strTableName + ".characters";
      
    string selectString("SELECT count(" + strColumnWordID + ")");
    string fromString(" FROM ");
    fromString += strTableName;
    string whereString(" WHERE ");
    whereString += strColumnCharacters + " = '" + characters + "'";
    string commandString = selectString + fromString + whereString;

    SQLite3Statement *sth =
        DictionarySingleton::dictionaryDB->prepare(commandString.c_str());
    if (!sth) {
        murmur("illegal SQL statement[%s]?", commandString.c_str());
        return false;
    }

    if (sth->step() == SQLITE_ROW) {
        int count = sth->column_int(0);
        murmur("found[%d]", count);
        delete sth;
        
        if (count > 0)  return true;
        else            return false;
    }
    else
    {
        delete sth;
        murmur("found count but encountered error?");
        return false;
    }
}

bool DictionarySingleton::getVocabularyVectorByCharacters(string characters,
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
    /// FROM phone_char2word_table, word_table, generic_freq_table
    /// WHERE phone_char2word_table.characters = 'ㄅㄚˋ'
    ///     AND word_table.wordID = phone_char2word_table.wordID
    ///     AND word_table.wordID = generic_freq_table.wordID
    /// ORDER BY generic_freq_table.freq DESC"
    ///
    /// Since there're two inner joins,
    /// the order of tables and columns are very very important.

    string strTableName = DictionarySingleton::inputMethodId;
    strTableName += "_char2word_table";
    string strColumnWordID = strTableName + ".wordID";    
    string strColumnCharacters = strTableName + ".characters";
      
    /// bind_foo seems not work on table/column name (sure it can't!),
    /// so use stupid concat...
    string selectString("SELECT word_table.word, generic_freq_table.freq");
    string fromString(" FROM ");
    fromString += strTableName + ", word_table, generic_freq_table";
    string whereString(" WHERE ");
    whereString += strColumnCharacters + " = '" + characters + "'";
    whereString += " AND word_table.wordID = " + strColumnWordID +
        " AND word_table.wordID = generic_freq_table.wordID";
    whereString += " ORDER BY generic_freq_table.freq DESC";
    string commandString = selectString + fromString + whereString;

    SQLite3Statement *sth =
        DictionarySingleton::dictionaryDB->prepare(commandString.c_str());
    if (!sth) {
        murmur("illegal SQL statement[%s]?", commandString.c_str());
        return false;
    }
    
    int rows = 0;
    while (sth->step() == SQLITE_ROW) rows++;

    murmur("query string=%s, number of candidates=%d",
        characters.c_str(), rows);
    if (!rows) {
        delete sth;
        return false;
    }

    sth->reset();
    while (sth->step() == SQLITE_ROW) {
        const char* word = sth->column_text(0);
        murmur("found[%s]", word);
        int freq = sth->column_int(1);
        Vocabulary currentVocabulary;
        currentVocabulary.word = string(word);
        currentVocabulary.freq = freq;
        
        vocabularyVectorRef.push_back(currentVocabulary);
    }
    delete sth;
    return true;
}