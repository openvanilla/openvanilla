#ifndef _OVIMARRAY_H
#define _OVIMARRAY_H

#include "OpenVanilla.h"
#include "ArrayKeySequence.h"
#include "OVCandidateList.h"
#include "OVCIN.h"
#include <cstring>

namespace OV_Array {
    enum STATE {
        STATE_WAIT_KEY1 = 0,
        STATE_WAIT_KEY2,
        STATE_WAIT_KEY3,
        STATE_WAIT_CANDIDATE
    };

    enum {
        MAIN_TAB = 0,
        SHORT_TAB = 1,
        SPECIAL_TAB = 2
    };
    const int RET_PASS = 0;
    const int RET_DONE = 1;
    const int RET_CONTINUE = 2;
};


class OVIMArray;
class OVIMArrayContext : public OVInputMethodContext
{
private:
    OVIMArray* parent;
    OVCIN **tabs;
    ArrayKeySequence keyseq;
    OV_Array::STATE state;
    OVCandidateList candi;
    std::vector<std::string> candidateStringVector, specialCodeVector;

    //OVBuffer* _buf;
    //OVCandidate* _candibar;
    //OVService* _srv;
public:
    OVIMArrayContext(OVIMArray* p, OVCIN** t)
        : parent(p), tabs(t), keyseq(t[OV_Array::MAIN_TAB]) {
        state = OV_Array::STATE_WAIT_KEY1;
    }
    virtual int keyEvent(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    virtual void clear();
private:
    void changeState(OV_Array::STATE s);
    void changeBackState(OV_Array::STATE s);
    virtual void updateDisplay(OVBuffer*);
    void dispatchStateHandler(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitKey1(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitKey2(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitKey3(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int WaitCandidate(OVKeyCode* , OVBuffer* , OVCandidate* , OVService* );
    int updateCandidate(OVCIN *tab,OVBuffer *buf, OVCandidate *candibar);
    void sendAndReset(const char *, OVBuffer* , OVCandidate* , OVService* );
    void clearAll(OVBuffer* buf, OVCandidate* candi_bar);
    void clearCandidate(OVCandidate *candi_bar);
    int selectCandidate(int num, string& out);
    bool isWSeq(char a, char b) const {  return a=='w' && isdigit(b);    }
    bool isForceSPSeq() {
        return keyseq.length() == 4 && !memcmp(keyseq.getSeq(),",,sp",4);
    }
    void queryKeyName(const char *keys, std::string& outKeyNames);
};

class OVIMArray : public OVInputMethod
{
private:
    char cname[128], ename[128];
    OVCIN *tabs[3];    // main, short-code, special-code
    int cfgAutoSP, cfgForceSP;

public:
    virtual int initialize(OVDictionary *, OVService*, const char *mp);
    virtual const char* identifier() { return "OVIMArray"; }
    virtual OVInputMethodContext *newContext() { 
        return new OVIMArrayContext(this,tabs); 
    }
    int updateConfig(OVDictionary *conf);
    virtual const char *localizedName(const char *locale){  
        if (!strcmp(locale, "zh_TW") || !strcmp(locale, "zh_CN")){
            strcpy(cname,"行列");
            //if( isForceSP() )   strcat(cname, "(快)");
            return cname;
        }
        else{
            strcpy(ename,"Array 30");
            //if( isForceSP() )   strcat(ename, "(Q)");
            return ename;
        }
    }
    virtual int isAutoSP() const    {   return cfgAutoSP;   }
    virtual int isForceSP() const   {   return cfgForceSP;   }
    virtual void setForceSP(bool value) { 
        cfgForceSP = value; 
    }
};

#endif
