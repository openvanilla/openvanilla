#ifndef _OVIMARRAY_H
#define _OVIMARRAY_H

#include <OpenVanilla/OpenVanilla.h>
#include "ArrayKeySequence.h"
#include "OVCandidateList.h"
#include "OVCIN.h"


enum ARRAY_STATE {
    STATE_WAIT_KEY1 = 0,
    STATE_WAIT_KEY2,
    STATE_WAIT_KEY3,
    STATE_WAIT_CANDIDATE
};

enum {
    ARRAY_MAIN_TAB = 0,
    ARRAY_SHORT_TAB = 1,
    ARRAY_SPECIAL_TAB = 2
};

class OVIMArray;
class OVIMArrayContext : public OVInputMethodContext
{
private:
    OVIMArray* parent;
    OVCIN **tabs;
    ArrayKeySequence keyseq;
    ARRAY_STATE state;
    OVCandidateList candi;
    std::vector<std::string> candidateStringVector;
public:
    OVIMArrayContext(OVIMArray* p, OVCIN** t)
        : parent(p), tabs(t), keyseq(t[ARRAY_MAIN_TAB]) { 
        state = STATE_WAIT_KEY1;
    }
    virtual int keyEvent(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
private:
    void changeState(ARRAY_STATE s);
    void changeBackState(ARRAY_STATE s);
    virtual void updateDisplay(OVBuffer*);
    void dispatchStateHandler(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitKey1(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitKey2(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitKey3(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitCandidate(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int updateCandidate(OVCIN *tab,OVBuffer *buf, OVCandidate *candibar);
    void clearAll(OVBuffer* buf, OVCandidate* candi_bar);
    void clearCandidate(OVCandidate *candi_bar);
    int selectCandidate(int num, string& out);
    bool isWSeq(char a, char b) const {  return a=='w' && isdigit(b);    }
};

class OVIMArray : public OVInputMethod
{
public:
    virtual int initialize(OVDictionary *, OVService*, const char *mp);
    virtual const char* identifier() { return "OVIMArray"; }
    virtual OVInputMethodContext *newContext() { 
        return new OVIMArrayContext(this,tabs); 
    }
    virtual const char *localizedName(const char *locale){  
        if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN"))
            return "OV 行列";
        else
            return "OV Array";
    }

private:
    OVCIN *tabs[3];    // main, short-code, special-code
};

#endif
