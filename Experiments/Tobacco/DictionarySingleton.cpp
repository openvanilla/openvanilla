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
}

bool getCharacterByKeystroke(string keystroke, string& characterRef)
{
}

bool getCharacterByWord(string word, string& characterRef)
{
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