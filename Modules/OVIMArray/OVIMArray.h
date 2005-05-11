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

class OVIMArray;
class OVIMArrayContext : public OVInputMethodContext
{
private:
    OVIMArray* parent;
    OVCIN *main_tab, *short_tab;
    ArrayKeySequence keyseq;
    ARRAY_STATE state;
    OVCandidateList candi;
    std::vector<std::string> candidateStringVector;
/*
    int autocomposing;
*/
public:
    OVIMArrayContext(OVIMArray* p, OVCIN* tab, OVCIN* tab2) 
        : parent(p), main_tab(tab), short_tab(tab2), keyseq(tab) { 
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
/*
    virtual int candidateEvent(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int compose(OVBuffer *buf, OVCandidate *textbar, OVService *srv);
    void backEvent(OVBuffer* buf, OVCandidate* candi_bar, OVService* srv);
*/
};

class OVIMArray : public OVInputMethod
{
public:
    virtual int initialize(OVDictionary *, OVService*, const char *mp);
    virtual const char* identifier() { return "OVIMArray"; }
    virtual OVInputMethodContext *newContext() { 
        return new OVIMArrayContext(this,main_tab, short_tab); 
    }
private:
    OVCIN *main_tab, *short_tab;;
};

#endif
