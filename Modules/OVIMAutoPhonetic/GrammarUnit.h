// GrammarUnit.h

#ifndef __GrammarUnit_h
#define __GrammarUnit_h

#include <string>
#include <vector>

using namespace std;

enum EventType {
    T_IGNORE    = 0,        // the event is ignored
    T_CLEAR     = 1,        // the unit becomes empty
    T_UPDATE    = 2,        // update display
    T_COMMIT    = 3,        // a unit is composed
    T_CANDIDATE = 4,        // candidate must be fetched
    T_NEXT      = 5,        // cursor focus moved out to next unit
    T_PREVIOUS  = 6,        // cursor focus moved out to previous unit
    T_SPLIT     = 7         // a sibling unit is created
};                          // no merge (we can add to sibling & clear self)

typedef unsigned int KeyModifier;
enum {
    K_CTRL              = 1,
    K_ALT               = 2,
    K_OPT               = 2,
    K_COMMAND           = 4,
    K_FUNCTIONKEYMASK   = 7,
    K_SHIFT             = 8,
    K_CAPSLOCK          = 16
};

typedef int KeyCode;
enum {
    K_ESC=27, K_SPACE=32, K_RETURN=13, K_TAB=9, K_DELETE=127, K_BACKSPACE=8,
    K_UP=30, K_DOWN=31, K_LEFT=28, K_RIGHT=29,
    K_HOME=1, K_END=4, K_PAGEUP=11, K_PAGEDOWN=12,
};

typedef vector<string> CandidateList;

class GrammarUnit;

// if you pass data with TEvent, you have to manage ownership/casting yourself
class TEvent {
public:
    TEvent(EventType t=T_IGNORE) { type=t; data=NULL; }
    EventType type;
    void *data;
};

class GrammarUnit {
public:
    virtual ~GrammarUnit() {}

    // data storage and representation ("Document" and "View")
    virtual const string& code()=0;                 // code stiring
    virtual const string& presentation()=0;         // displayed "presentation"
    virtual bool vacant()=0;                        // if no code() available
    virtual size_t width()=0;                       // measured in code points
    virtual size_t cursor()     { return width(); } // cursor position
    virtual size_t markFrom()   { return 0; }       // highlight startlo
    virtual size_t markLength() { return 0; }       // highlight length
    
    // event dispatching ("Controller")
    virtual const TEvent keyEvent(KeyCode k, KeyModifier m)=0;
    virtual const CandidateList fetchCandidateList()=0;
    virtual const TEvent cancelCandidate()=0;
    virtual const TEvent chooseCandidate(size_t index, const string &item)=0;
};

#endif
