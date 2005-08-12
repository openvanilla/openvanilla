// AutoPhonetic.h

#ifndef __AutoPhonetic_h
#define __AutoPhoneitc_h

#include <OpenVanilla/OpenVanilla.h>
#include <stdio.h>
#include <stdlib.h>
#include "OVSQLite3.h"
#include "OVPhoneticLib.h"
#include "GrammarUnit.h"
#include "UTF8Utility.h"

struct PhoneticConfig {
    PhoneticConfig() : layout(OVPStandardLayout), tonetolerance(false) {}
    int layout;
    bool tonetolerance;
};

class PhoneticSyllable;
class PhoneticPhrase;

typedef Pair<string, int> TsiFreqPair;
typedef vector<TsiFreqPair> TsiList;

class PhoneticService {
public:
    PhoneticService(SQLite3 *d, OVService *s);
    void beep();
    size_t codePointCount(const string &s);
    const CandidateList fetchBPMFCandidate(PhoneticSyllable &q);
    const CandidateList fetchTsiCandidate(PhoneticPhrase &q);
    size_t queryBPMF(PhoneticSyllable &q, string& first);
    size_t queryBPMF(const string &q, string& first);
    size_t queryTsi(PhoneticPhrase &q, TsiFreqPair& first);
protected:
    SQLite3 *db;
    OVService *srv;
};

class PhoneticSyllable : public GrammarUnit {
protected:
    bool fixedusrchr;           // user has designated a fixed candidate
    bool hascandi;              // has more than 1 candidate
    bool committed;             // already has usedb character representation
    bool usedb;                 // if the committed chr is a usedb character
    string seq;                 // "standard layout" string, or "query key"  
    string chr;                 // choosen Han character
    int cur;                    // cursor position
    OVPhoneticSyllable bpmf;    // composing syllable
    PhoneticService *srv;       // service object
    PhoneticConfig cfg;
public:
    PhoneticSyllable(PhoneticService *s, const PhoneticConfig &c);
    virtual void updateConfig(const PhoneticConfig &c);
    virtual void reset();

    virtual bool hasCandidate();    
    virtual bool fixedUserChar();
    virtual void changeChar(const string& c, bool fixusrchr=true);
    
    virtual const PhoneticConfig& getConfig();
    virtual const string BPMFString();
    virtual const string toneString();
    
    virtual const string& code();
    virtual const string& presentation();
    virtual bool vacant();
    virtual size_t width();
    virtual size_t cursor();
    
    virtual const TEvent keyEvent(KeyCode k, KeyModifier m);
    virtual const CandidateList fetchCandidateList();
    virtual const TEvent cancelCandidate();
    virtual const TEvent chooseCandidate(size_t, const string &item);

    virtual const string dump();
    
protected:
    virtual bool isPunctuationCombination(KeyCode k, KeyModifier m);
    virtual const TEvent keyPunctuation(KeyCode k, KeyModifier m);
    virtual const TEvent keyCapslock(KeyCode k, KeyModifier m);
    virtual const TEvent keyEsc();
    virtual const TEvent keyLeft();
    virtual const TEvent keyRight();
    virtual const TEvent keyBackspace();
    virtual const TEvent keyDelete();
    virtual const TEvent keyCompose(KeyCode k);
    virtual const TEvent keyPrintable(KeyCode k, KeyModifier m);
    virtual const TEvent keyNonBPMF(KeyCode k);
};


class PhoneticSentence : public GrammarUnit {
protected:
    vector<PhoneticSyllable> syllist;
    int focus;
    
    PhoneticService *srv;
    PhoneticConfig cfg;
public:
    PhoneticSentence(PhoneticService *s, const PhoneticConfig& c);
    virtual void updateConfig(const PhoneticConfig &c);
    virtual void reset();
    
    virtual const PhoneticConfig& getConfg();
    virtual const string BPMFString();
    virtual const string toneString();
    virtual const string matchMaskString();

    virtual const string& code();
    virtual const string& presentation();
    virtual bool vacant();
    virtual size_t width();
    virtual size_t cursor();
    virtual size_t markFrom();
    virtual size_t markLength();
    
    virtual const TEvent keyEvent(KeyCode k, KeyModifier m);
    virtual const CandidateList fetchCandidateList();
    virtual const TEvent cancelCandidate();
    virtual const TEvent chooseCandidate(size_t index, const string &item);

    virtual const TEvent keyEvent(KeyCode k, KeyModifier m, PhoneticPhrase *prev, PhoneticPhrase *next);
    
    virtual const TEvent addSyllable(const PhoneticPhrase* prev,
        const PhoneticPhrase* next);
protected:
    
};


#endif

