//#define OV_DEBUG

#include <vector>

#include "DictionarySingleton.h"

#ifndef WIN32
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OVUtility.h"
#endif

using namespace std;

DictionarySingleton* DictionarySingleton::itsInstance = 0;
SQLite3* DictionarySingleton::dictionaryDB = 0;
SQLite3* DictionarySingleton::imTableDB = 0;

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

	m_profileManager = ProfileManager::getInstance();
}

DictionarySingleton::~DictionarySingleton()
{
    delete DictionarySingleton::dictionaryDB;

	delete [] &vocableString;
	delete [] &viewLimitString;
	delete [] &viewString;
	delete [] &checkString;

	m_profileManager->releaseInstance();
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
    SQLite3Statement *sth =
        DictionarySingleton::dictionaryDB->prepare(checkString);
    if (!sth) {
        murmur("illegal SQL statement[%s]?", checkString);
        return false;
    }
	
	sth->bind_text(1, characters.c_str());

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
    vector<Vocabulary>& vocabularyVectorRef, bool isLimited)
{
	//@{
	//@warning first fetches learned data here, not finished yet.
	vector<Profile>* profiles = m_profileManager->fetch(characters);
	if(profiles) {
		for(vector<Profile>::const_iterator i = profiles->begin();
			i != profiles->end();
			++i)
		{
			Vocabulary currentVocabulary(i->id.pattern);
			currentVocabulary.freq = i->hitRate; //< this is not good enough.

			vocabularyVectorRef.push_back(currentVocabulary);
		}
	}
	//@}

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

	SQLite3Statement *sth = NULL;
	const char* command = NULL;
	if(isLimited)
		command = viewLimitString;
	else
		command = viewString;	
	sth = dictionaryDB->prepare(command);
    if (!sth) {
        murmur("illegal SQL statement[%s]?", command);
		return false;
    }
	sth->bind_text(1, characters.c_str());

    int rows = 0;
    while (sth->step() == SQLITE_ROW) {
		rows++;
        const char* word = sth->column_text(0);
        murmur("found[%s]", word);
        int freq = sth->column_int(1);
        Vocabulary currentVocabulary(word);
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
    SQLite3Statement *sth =
		imTableDB->prepare(vocableString);
    if (!sth) return false;
	sth->bind_text(1, characters.c_str());
       
    int rows=0;
    while (sth->step()==SQLITE_ROW) {
		rows++;
        const char* word = sth->column_text(0);
        murmur("found[%s]", word);
        int order = sth->column_int(1);
        Vocabulary currentVocabulary(word);
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
