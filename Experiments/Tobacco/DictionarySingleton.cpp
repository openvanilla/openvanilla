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
    if ((err = dictionaryDB->open(tsiDbFilePath)))
        murmur("SQLite3 error! code=%d", err);

	char imDbFilePath[256];
	sprintf(imDbFilePath, "%s/imtables.db", dbFilePath);
	imTableDB = new SQLite3;
    if ((err = imTableDB->open(imDbFilePath)))
        murmur("SQLite3 error! code=%d", err);

	m_profileManager = ProfileManager::getInstance();
}

DictionarySingleton::~DictionarySingleton()
{
    delete DictionarySingleton::dictionaryDB;

	m_profileManager->releaseInstance();
}

void DictionarySingleton::lostInstance()
{
	if(DictionarySingleton::itsInstance)
	{
		delete DictionarySingleton::itsInstance;
	}
}

bool DictionarySingleton::isVocabulary(string keystrokes)
{      
    SQLite3Statement *sth = imTableDB->prepare(checkString);
    if (!sth) {
        murmur("illegal SQL statement[%s]?", checkString);
        return false;
    }
	sth->bind_text(1, keystrokes.c_str());
    murmur("query string=%s", keystrokes.c_str());

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

bool DictionarySingleton::getWordsByCharacters(
	string characters, vector<Vocabulary>& vocabularyVectorRef, bool isLimited)
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
			currentVocabulary.logProb = i->hitRate; //< this is not good enough.

			vocabularyVectorRef.push_back(currentVocabulary);
		}
	}
	//@}

    /// characters-word table schema:    |characters|wordId|logProb|backOff
    /// word table schema:               |wordId|word|
    /// frequency table schema:          |wordId|freq|
	const char* command = 0;
	if(isLimited)
		command = viewLimitString;
	else
		command = viewString;	
	SQLite3Statement *sth = dictionaryDB->prepare(command);
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
        double logProb = sth->column_double(1);
        double backOff = sth->column_double(2);
        Vocabulary currentVocabulary(word);
        currentVocabulary.logProb = logProb;
        currentVocabulary.backOff = backOff;
        
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

bool DictionarySingleton::getVocablesByKeystrokes(
	string keystrokes, vector<Vocabulary>& vocabularyVectorRef)
{
    SQLite3Statement *sth =
		imTableDB->prepare(vocableString);
    if (!sth) return false;
	sth->bind_text(1, keystrokes.c_str());
       
    int rows=0;
    while (sth->step()==SQLITE_ROW) {
		rows++;
        const char* word = sth->column_text(0);
        murmur("found[%s]", word);
        int order = sth->column_int(1);
        Vocabulary currentVocabulary(word);
        currentVocabulary.word = string(word);
		currentVocabulary.order = order;
		//@warning The log prob. of <unk> is hard-coded here temporarily.
		currentVocabulary.logProb = -5.515997f; //* 0.9f + order * 0.1f;
		currentVocabulary.backOff = 0.0f;
        
        vocabularyVectorRef.push_back(currentVocabulary);
    }
    if (!rows) {
        delete sth;
        return false;
    }

	delete sth;
    return true;
}
