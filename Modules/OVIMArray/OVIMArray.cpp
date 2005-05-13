#define OV_DEBUG
#include"OVIMArray.h"
#include<OpenVanilla/OVLibrary.h>
#include<OpenVanilla/OVUtility.h>
#include<utility>

using namespace std;
using namespace OV_Array;

void OVIMArrayContext::updateDisplay(OVBuffer* buf){
    buf->clear();
    if (keyseq.length()){   
        string str;
        keyseq.compose(str);
        buf->append(str.c_str());
    }
    buf->update();
}

int OVIMArrayContext::updateCandidate(OVCIN *tab,OVBuffer *buf, OVCandidate *candibar){
    tab->getWordVectorByChar(keyseq.getSeq(), candidateStringVector);
    string currentSelKey = tab->getSelKey();
    if( candidateStringVector.size() == 0 )
        clearCandidate(candibar);
    else
        candi.prepare(&candidateStringVector,
                          const_cast<char*>(currentSelKey.c_str()), candibar);
    return 1;
}

int OVIMArrayContext::WaitKey1(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candibar, OVService* srv){
    updateCandidate(tabs[SHORT_TAB], buf, candibar);
    if( key->code() == 't' )
        buf->clear()->append((char*)"的")->update();
    changeState(STATE_WAIT_KEY2);
    return 1;    
}

int OVIMArrayContext::WaitKey2(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candibar, OVService* srv){
    if( isWSeq(keyseq.getSeq()[0], keyseq.getSeq()[1]) ){
        updateCandidate(tabs[MAIN_TAB], buf, candibar);
        buf->clear()->append(candidateStringVector[0].c_str())->update();
        changeState(STATE_WAIT_CANDIDATE);
    }
    else{
        updateCandidate(tabs[SHORT_TAB], buf, candibar);
        if( isprint(key->code()) && keyseq.valid(key->code()) )
            changeState(STATE_WAIT_KEY3);
    }
    return 1;    
}

int OVIMArrayContext::WaitKey3(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candibar, OVService* srv){
    updateCandidate(tabs[MAIN_TAB], buf, candibar);
    return 1;    
}


int OVIMArrayContext::WaitCandidate(OVKeyCode* key, OVBuffer* buf, 
                                    OVCandidate* candibar, OVService* srv){
    const char keycode = key->code();
    if (keycode == ovkEsc || keycode == ovkBackspace){
        candibar->hide()->clear();
        candi.cancel();
        buf->clear()->update();
        return 1;
    }

    if (keycode == ovkDown || keycode == ovkRight ||
        (!candi.onePage() && keycode==ovkSpace) ){
        candi.pageDown()->update(candibar);
        return 1;
    }

    if (keycode == ovkUp || keycode == ovkLeft){
        candi.pageUp()->update(candibar);
        return 1;
    }

    // enter == first candidate
    // space (when candidate list has only one page) == first candidate
    char c=key->code();
    if (c == ovkReturn || (candi.onePage() && c==ovkSpace)) 
        c=candi.getSelKey()[0];

    string output;
    if (candi.select(c, output)) {
        sendAndReset(output.c_str(), buf, candibar, srv);
        return 1;
    }
    return 0;
}


void OVIMArrayContext::queryKeyName(const char *keys, std::string& outKeyNames){
    int len = strlen(keys);
    for(int i=0;i<len;i++){
        string inKey(keys, i, 1);
        vector<string> keyName;
        if( tabs[MAIN_TAB]->getCharVectorByKey(inKey, keyName) > 0)
            outKeyNames.append(keyName[0]);
    }
}

void OVIMArrayContext::sendAndReset(const char *ch, OVBuffer* buf, 
                                    OVCandidate* candibar, OVService* srv){
    bool notifySP = false;
    // lookup special code
    if((parent->isAutoSP() || parent->isForceSP()) &&
       tabs[SPECIAL_TAB]->getWordVectorByChar(ch, specialCodeVector)>0)
    {
        int splen = specialCodeVector[0].length();
        const char *spcode = specialCodeVector[0].c_str();
        if( !(splen == keyseq.length() && 
              equal(spcode, spcode+splen, keyseq.getSeq())) ){
            char buf[16];
            string keynames;
            queryKeyName(specialCodeVector[0].c_str(), keynames);
            sprintf(buf, "%s: %s", ch, keynames.c_str());
            srv->notify(buf);
            notifySP = true;
        }
    }
    if( !(parent->isForceSP() && notifySP) )
        buf->clear()->append(ch)->send();
    else
        srv->beep();
    clearAll(buf, candibar);
    changeState(STATE_WAIT_KEY1);
}

void OVIMArrayContext::clearAll(OVBuffer* buf, OVCandidate* candi_bar){
    clearCandidate(candi_bar);
    buf->clear()->update();
    keyseq.clear();
}

void OVIMArrayContext::clearCandidate(OVCandidate *candi_bar){           
    candi.cancel();
    candi_bar->hide()->clear();
}   

int OVIMArrayContext::selectCandidate(int num, string& out){
   char c=candi.getSelKey()[num];
   return candi.select(c, out);
}

int OVIMArrayContext::keyEvent(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candi_bar, OVService* srv)
{
    int ret = 0;
    const char keycode = key->code();
    const bool validkey = keyseq.valid(keycode) || 
      ( keyseq.getSeq()[0]=='w' && isdigit(keycode) );
   
    murmur("OVIMArray state: %d\n", state);
    if (!keyseq.length() && !isprint(keycode)) return 0;

    if(keycode==ovkEsc){
        clearAll(buf, candi_bar);
        changeState(STATE_WAIT_KEY1);
        return 1;
    }

    if( state == STATE_WAIT_CANDIDATE )
        return WaitCandidate(key, buf, candi_bar, srv);
    if( candi.onDuty() && isdigit(keycode) && 
        !(keyseq.length() == 1 && isWSeq(keyseq.getSeq()[0],keycode)) ){
        string c;
        if( candi.select(keycode, c) ){
            if( c != "□"  ){
                sendAndReset(c.c_str(), buf, candi_bar, srv);
            }
            else{
                srv->beep();
                if( state <= STATE_WAIT_KEY3 ) //dirty hack to set duty=1
                    updateCandidate(tabs[SHORT_TAB], buf, candi_bar);
            }
        }
        return 1;
    }
    if (keyseq.length() && keycode == ovkSpace){
        tabs[MAIN_TAB]->getWordVectorByChar(keyseq.getSeq(), candidateStringVector);
        string c;
        if(candidateStringVector.size() == 1){
            if(selectCandidate(0, c))
                sendAndReset(c.c_str(), buf, candi_bar, srv);
        }
        else if(candidateStringVector.size() > 1){
            updateCandidate(tabs[MAIN_TAB], buf, candi_bar);
            if(selectCandidate(0, c))
                buf->clear()->append(c.c_str())->update();
            changeState(STATE_WAIT_CANDIDATE);
        }
        return 1;
    }
    if( isprint(keycode) && validkey ){
        if( keyseq.length() >= 5 ||
            (keyseq.length() == 4 && keycode != 'i') )
            return 1;
        keyseq.add(keycode);
        updateDisplay(buf);
        ret = 1;
    }
    else if (key->code() == ovkDelete || key->code() == ovkBackspace){   
        keyseq.remove();
        updateDisplay(buf);
        changeBackState(state);
        ret = 1;
    }
    dispatchStateHandler(key, buf, candi_bar, srv);
    return ret;
}

void OVIMArrayContext::dispatchStateHandler(OVKeyCode* key, OVBuffer* buf, 
                                            OVCandidate* candi_bar, OVService* srv)
{
    switch(state){
        case STATE_WAIT_KEY1:
            WaitKey1(key, buf, candi_bar, srv);
            break;
        case STATE_WAIT_KEY2:
            WaitKey2(key, buf, candi_bar, srv);
            break;
        case STATE_WAIT_KEY3:
            WaitKey3(key, buf, candi_bar, srv);
            break;
        default:
            break;
    }
}

void OVIMArrayContext::changeBackState(STATE s){
    switch(s){
        case STATE_WAIT_CANDIDATE:
        case STATE_WAIT_KEY2: 
            changeState(STATE_WAIT_KEY1); 
            break;
        case STATE_WAIT_KEY3: 
            if( keyseq.length() == 2)
                changeState(STATE_WAIT_KEY1); 
            break;
        default: break;
    }
}

void OVIMArrayContext::changeState(STATE s){    
    murmur("OVIMArray change state: %d -> %d\n", state, s);
    state = s;  
}

int OVIMArray::initialize(OVDictionary *conf, OVService*, const char *path){
    char buf[PATH_MAX];
    char *cinfiles[] = { 
        "%sOVIMArray/array30.cin",  
        "%sOVIMArray/ArrayShortCode.cin",
        "%sOVIMArray/ArraySpecial.cin"
    };
    for(int i=0;i<3;i++){
        sprintf(buf, cinfiles[i], path);
        murmur("OVIMArray: open cin %s", buf);
        tabs[i] = new OVCIN(buf); 
    }
    updateConfig(conf);
    return 1;
}

int OVIMArray::updateConfig(OVDictionary *conf){
    const char *AutoSP = "AutoQuerySpecialCode";
    const char *ForceSP = "ForceSpecialCode";

    if( !conf->keyExist(AutoSP) ) conf->setInteger(AutoSP, 1);
    if( !conf->keyExist(ForceSP) ) conf->setInteger(ForceSP, 0);

    cfgAutoSP = conf->getInteger(AutoSP);
    cfgForceSP = conf->getInteger(ForceSP);

    return 1;
}

OV_SINGLE_MODULE_WRAPPER(OVIMArray);

