#define OV_DEBUG

#include <vector>
#include <algorithm>

#include "DictionarySingleton.h"

#ifndef WIN32
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OVUtility.h"
#endif

using namespace std;

DictionarySingleton* DictionarySingleton::itsInstance = NULL;
SQLite3* DictionarySingleton::dictionaryDB = NULL;

DictionarySingleton::DictionarySingleton(
    const char* dbFilePath)
{
    murmur("new DictionarySingleton");

    DictionarySingleton::dictionaryDB = new SQLite3;
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

bool DictionarySingleton::getVocabulariesByKeystrokes(
	string keystrokes, vector<Vocabulary>& vocabulariesRef)
{
    /// key2word table schema:  |key|wordID|
    /// word table schema:      |wordID|word|
    /// frequency table schema: |wordID|freq|
    ///
    /// The reasons why to separate into 3 tables are:
    /// 1. Characters can be different by different input methods.
    /// 2. Different frequency table represents different "context" or
    ///    "user profile," for example, "freq_zh_Hant_contextBar." 
    /// 3. Support words in zh_Hans and zh_Holo, etc.
    ///
    /// A SQL statement for example:
    /// SELECT word_zh_Hant.word, freq_zh_Hant_generic.freq, key2word_bpmf.ord
    /// FROM key2word_bpmf, word_zh_Hant, freq_zh_Hant_generic
    /// WHERE key2word_bpmf.key = 'foo'
    ///     AND word_zh_Hant.wordID = key2word_bpmf.wordID
    ///     AND word_zh_Hant.wordID = freq_zh_Hant_generic.wordID
    /// ORDER BY freq_zh_Hant_generic.freq DESC"
    ///
    /// Since there're two inner joins,
    /// the order of tables and columns are very very important.
      
    /// bind_foo seems not work on table/column name (sure it can't!),
    /// so use stupid concat...
    string tableIM = "key2word_" + DictionarySingleton::inputMethodId;
    string lang = "zh_Hant";
    string tableWord = "word_" + lang;
    string tableFreq = "freq_" + lang;
    string selectString("SELECT ");
    selectString +=
    	tableWord + ".word, " + tableFreq + ".freq, " + tableIM + ".ord";
    string fromString(" FROM ");
    fromString += tableIM + ", " + tableWord + ", " + tableFreq;
    string whereString(" WHERE ");
    whereString += tableIM + ".key = '" + keystrokes + "'";
    whereString += " AND " + tableWord + ".wordID = " + tableIM + ".wordID" +
        " AND " + tableWord + ".wordID = " + tableFreq + ".wordID";
    whereString += " ORDER BY " + tableFreq + ".freq DESC";
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
        keystrokes.c_str(), rows);
    if (!rows) {
        delete sth;
        return false;
    }

    sth->reset();
    
    // count token numbers by '\t'.
    int length = 1;
    string::size_type index = 0;
    while(index != string::npos) {
	    index = keystrokes.find('\t', index);
	    if(index != string::npos)
	    	length++;
	}

    while (sth->step() == SQLITE_ROW) {
        const char* word = sth->column_text(0);
        murmur("found[%s]", word);
        int freq = sth->column_int(1);
        int order = sth->column_int(2);
        Vocabulary currentVocabulary;
        currentVocabulary.word = string(word);
        currentVocabulary.freq = freq;
        currentVocabulary.order = order;
        currentVocabulary.length = length;
        
        vocabulariesRef.push_back(currentVocabulary);
    }
    delete sth;
    return true;
}
