// GrammarUnit.h

#ifndef __GrammarUnit_h
#define __GrammarUnit_h

class AutoPhraseObject {
public:
    virtual ~AutoPhraseObject() {}
};

class Service : public AutoPhraseObject {
public:
    virtual void notify (const char *msg)=0;
    virtual void beep()=0;
    
    // returns the number of codepoints
    virtual int u8strlen(const char *str)=0;   
};

typedef int TextEventType;

enum {
    T_IGNORE    = 0,
    T_CLEAR     = 1,
    T_UPDATE    = 2,        // update display
    T_COMMIT    = 3,        // a unit is composed
    T_CANDIDATE = 4,
    T_NEXT      = 5,        // cursor focus movement
    T_PREVIOUS  = 6,
    T_SPLIT     = 7         // a sibling unit is created
};

typedef int KeyModifier;
enum {
    K_SHIFT     = 1,
    K_CTRL      = 2,
    K_ALT       = 4,
    K_OPT       = 4,
    K_COMMAND   = 8
};

class TextBlock : public AutoPhraseObject {
public:
    TextBlock(Service *s) { srv=s; }
    virtual int empty()     { return !strlen(codeString()); }
    virtual int length()    { return srv->u8strlen(composeDisplay()); }
    virtual int cursor()    { return length(); }
    virtual int hiliteStart()   { return 0; }
    virtual int hiliteLength()  { return 0 ; }
    virtual const char *composeDisplay()=0;
    virtual const char *codeString()=0;
protected:
    Service *srv;
};

class CandidateItem : public AutoPhraseObject { 
public:
    CandidateItem(const char *s) {
        if (!s) str=NULL;
        str=(char*)calloc(1, strlen(s)+1);
        strcpy(str, s);
    }
    virtual ~CandidateItem() { if (str) free(str); }
    const char *dataString() { return str; }
    char *str;
};

class CandidateList {
public:
    CandidateList(int c=0) {
        ct=c;
        items = ct ? new CandidateItem*[ct] : NULL;
    }
    ~CandidateList() {
        if (ct) { for (int i=0;i<ct;i++) if (items[i]) delete items[i]; }
        if (items) delete[] items;
    }
    CandidateItem *setItem(int i, CandidateItem *o) {
        if (i<0 || i>=ct) return NULL;
        return items[i]=o;
    }
    CandidateItem *item(int i) { 
        if (i<0 || i>=ct) return NULL; 
        return items[i];
    }
    int count() { return ct; }
    
    int ct;
    CandidateItem **items;
};

class GrammarUnit;

class TEvent : public AutoPhraseObject {
public:
    TEvent(TextEventType t=T_IGNORE) { type=t; data=NULL; }
    TextEventType   type;
    GrammarUnit     *data;
};

class GrammarUnit : public TextBlock {
public:
    GrammarUnit(Service *s) : TextBlock(s) {}
    virtual const TEvent enterRightBound()=0;    // enter the bound
    virtual const TEvent enterLeftBound()=0;
    virtual const TEvent clear()=0;
    virtual const TEvent key(char c, KeyModifier m)=0;
    virtual const TEvent keyBackspace()=0;
    virtual const TEvent keySpace()=0;
    virtual const TEvent keyEnter()=0;
    virtual const TEvent keyEsc()=0;
    virtual const TEvent keyLeft()=0;
    virtual const TEvent keyRight()=0;
    virtual const TEvent keyUp()=0;
    virtual const TEvent keyDown()=0;
    virtual const TEvent keyHome()=0;
    virtual const TEvent keyEnd()=0;
    virtual const TEvent chooseCandidate(int idx, CandidateItem *item)=0;
    virtual const TEvent cancelCandidate()=0;
    virtual CandidateList *getCandidateList()=0;
};

#endif
