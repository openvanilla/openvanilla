#include <vector>

#include "DictionarySingleton.h"

using namespace std;

DictionarySingleton* DictionarySingleton::itsInstance = NULL;
SQLite3* DictionarySingleton::dictionaryDB = NULL;

DictionarySingleton::DictionarySingleton(
    const char* dbFilePath, const char* inputMethodId)
{
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
    /// SELECT word_table.wordID, word_table.word, generic_freq_table.freq
    /// FROM word_table
    /// INNER JOIN cj_char2word_table
    ///     ON word_table.wordID = cj_char2word_table.wordID
    /// INNER JOIN generic_freq
    ///     ON word_table.wordID = generic_freq.wordID
    /// WHERE cj_char2word_table.characters = 'hda taj'
    /// ORDER BY generic_freq.freq DESC"
    ///
    /// Since there're two inner joins,
    /// it might be better to create a temporary table first.
    
    string commandString("SELECT word_table.wordID, word_table.word, generic_freq_table.freq FROM word_table INNER JOIN $char2wordTable ON word_table.wordID = $charactersColumn INNER JOIN generic_freq ON word_table.wordID = generic_freq.wordID WHERE $charactersColumn = '$key' ORDER BY generic_freq.freq DESC");
    SQLite3Statement *sth =
        DictionarySingleton::dictionaryDB->prepare(commandString.c_str());
    if (!sth) return false;
        
    string strTableName(DictionarySingleton::inputMethodId);
    strTableName += "-char2word_table";
    sth->bind_text(1, strTableName.c_str());
    /// e.g. "cj-char2word_table"
    
    string strCharactersColumn = strTableName + ".characters";
    sth->bind_text(2, strCharactersColumn.c_str());
    /// e.g. "cj-char2word_table.characters"

    const char* key = characters.c_str();
    sth->bind_text(3, key);
    
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