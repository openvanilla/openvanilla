#include <vector>

#include "DictionarySingleton.h"

using namespace std;

DictionarySingleton* DictionarySingleton::itsInstance = NULL;

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

bool getVocabularyVectorByCharacters(string characters,
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
}

const char *QueryForCommand(SQLite3 *db, const char *command) {
    char *r=NULL;
    static char valuebuffer[256];

    murmur("executing SQL command: %s", command);

    SQLite3Statement *sth=db->prepare(command);
    if (sth) {
        if (sth->step()==SQLITE_ROW) {
            strcpy(valuebuffer, sth->column_text(0));
            murmur("return value=%s", valuebuffer);
            r=valuebuffer;
        }
        delete sth;
    }
    return r;
}

const char *QueryForKey(SQLite3 *db, const char *tbl, const char *key) {
    char cmd[256];
    char keyescaped[128];
    const char *kp=key;
    char *ep=keyescaped;
    while (*kp) {
        if (*kp=='\'') { *ep++='\''; *ep++='\''; }
        else *ep++=*kp;
        kp++;
    }
    *ep=0;
    sprintf(cmd, "select value from %s where key='%s';", tbl, keyescaped);
    return QueryForCommand(db, cmd);
}