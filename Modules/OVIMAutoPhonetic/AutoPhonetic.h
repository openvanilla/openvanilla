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

class PhoneticService {
public:
    PhoneticService(SQLite3 *d, OVService *s);
    void beep();
    size_t codePointCount(const string &s);
    const CandidateList fetchBPMFCandidate(PhoneticSyllable &q);
    size_t queryBPMF(PhoneticSyllable &q, string& first);
    size_t queryBPMF(const string &q, string& first);
protected:
    SQLite3 *db;
    OVService *srv;
};

class PhoneticSyllable : public GrammarUnit {
protected:
    bool fixed;                 // already has Han character representation
    bool han;                   // if the fixed chr is a han character
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
    
    virtual const PhoneticConfig& getConfig();
    virtual const string bpmfString();
    virtual const string toneString();
    
    virtual const string& code();
    virtual const string& presentation();
    virtual bool vacant();
    virtual size_t width();
    virtual size_t cursor();
    
    virtual const TEvent enterRightBound();
    virtual const TEvent enterLeftBound();
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

#endif

