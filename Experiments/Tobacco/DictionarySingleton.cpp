#include <vector>

#include "DictionarySingleton.h"

using namespace std;

DictionarySingleton* DictionarySingleton::itsInstance = NULL;
SQLite3* DictionarySingleton::dictionaryDB = NULL;

DictionarySingleton::DictionarySingleton()
{
    DictionarySingleton::dictionaryDB = new SQLite3;
    const char* dbfile = "db path here";
    if (int err = DictionarySingleton::dictionaryDB->open(dbfile)) {
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
    /// SQL statement for example:
    /// SELECT wordID, word, freq
    /// FROM cj-word_table, word_table, generic_freq_table
    /// WHERE cj-word_table.characters = ?
    /// ON cj-word_table.wordID = word_table.wordID AND
    ///     cj-word_table.wordID = generic_freq_table.wordID
    ///
    /// Since there're two inner joins,
    /// it might be better to create a temporary table first.
    
    string commandString("SELECT wordID, word, freq FROM cj-word_table, word_table, generic_freq_table WHERE cj-word_table.characters = ? ON cj-word_table.wordID = word_table.wordID AND cj-word_table.wordID = generic_freq_table.wordID");
    SQLite3Statement *sth =
        DictionarySingleton::dictionaryDB->prepare(commandString.c_str());
    if (!sth) return false;
    
    const char* key = characters.c_str();
    sth->bind_text(0, key);
    
    int rows = 0;
    while (sth->step() == SQLITE_ROW) rows++;
    murmur("query string=%s, number of candidates=%d", key, rows);
    if (!rows) {
        delete sth;
        return false;
    }

    sth->reset();
    while (sth->step() == SQLITE_ROW) {
        const char* word = sth->column_text(1);
        int freq = sth->column_int(2);
        Vocabulary currentVocabulary;
        currentVocabulary.word = string(word);
        currentVocabulary.freq = freq;
        
        vocabularyVectorRef.push_back(currentVocabulary);
    }
    delete sth;
    return true;
}