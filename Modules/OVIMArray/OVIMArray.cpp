#define OV_DEBUG
#include"OVIMArray.h"
#include<OpenVanilla/OVLibrary.h>
#include<OpenVanilla/OVUtility.h>

using namespace std;

void OVIMArrayContext::updateDisplay(OVBuffer* buf){
    buf->clear();
    if (keyseq.length()){   
        string str;
        keyseq.compose(str);
        buf->append(str.c_str());
    }
    buf->update();
}

int OVIMArrayContext::updateCandidate(OVBuffer *buf, OVCandidate *textbar, OVService *srv){
    cintab->getWordVectorByChar(keyseq.getSeq(), candidateStringVector);
    string currentSelKey = cintab->getSelKey();

    candi_list.prepare(&candidateStringVector,
                          const_cast<char*>(currentSelKey.c_str()), textbar);
    return 1;
}

int OVIMArrayContext::compose(OVBuffer *buf, OVCandidate *textbar, OVService *srv)
{
    if( candidateStringVector.size() == 1 ){
    
    }
    return 1;
}

int OVIMArrayContext::candidateEvent(OVKeyCode* key, OVBuffer* buf, 
                                     OVCandidate* candi_bar, OVService* srv)
{
    // enter == first candidate
    // space (when candidate list has only one page) == first candidate
    char c=key->code();
    if (key->code() == ovkReturn || (candi_list.onePage() && key->code()==ovkSpace)) 
        c=candi_list.getSelKey()[0];

    string output;
    if (candi_list.select(c, output)) {
        buf->clear()->append(output.c_str())->send();
        clearAll(buf, candi_bar);
        return 1;
    }
    return 0;
}

void OVIMArrayContext::clearAll(OVBuffer* buf, OVCandidate* candi_bar){
    clearCandidate(candi_bar);
    buf->clear()->update();
    keyseq.clear();
}

void OVIMArrayContext::clearCandidate(OVCandidate *candi_bar){           
//    autocomposing=0;
    candi_list.cancel();
    candi_bar->hide()->clear();
}   

void OVIMArrayContext::backEvent(OVBuffer* buf, OVCandidate* candi_bar, OVService* srv){
    keyseq.remove();
    updateDisplay(buf);
    if( keyseq.length() == 0 ) 
        clearCandidate(candi_bar);
    else{   
        if( cintab->getWordVectorByChar(keyseq.getSeq(), candidateStringVector) )
            updateCandidate(buf, candi_bar, srv);
        else if (candi_list.onDuty()) 
            clearCandidate(candi_bar);
    }

}

int OVIMArrayContext::keyEvent(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candi_bar, OVService* srv)
{
    const char keycode = key->code();
    if( candi_list.onDuty() && candidateEvent(key, buf, candi_bar, srv) )
        return 1;
    
    if(key->code()==ovkEsc){
        clearAll(buf, candi_bar);
        return 1;
    }

    if (key->code() == ovkDelete || key->code() == ovkBackspace){   
        backEvent(buf, candi_bar, srv); 
        return 1;
    }


    if (keyseq.length() && keycode == ovkSpace){
        if (keyseq.length()==1 && keyseq.getSeq()[0]=='t'){
            buf->clear()->append((char*)"的")->send();
            keyseq.clear();
            //cancelAutoCompose(textbar);
            return 1;
        }
//        return compose(buf, candi_bar, srv);
    }
    
    if( isprint(keycode) && keyseq.valid(keycode) &&
        !(key->isCtrl() || key->isOpt() || key->isCommand()) ){
        if( keyseq.length() == 4 && tolower(keycode) != 'i' ){
            return 1;
        }
        keyseq.add(keycode);
        updateDisplay(buf);
        if(cintab->getWordVectorByChar(keyseq.getSeq(), candidateStringVector)){
            updateCandidate(buf, candi_bar, srv);
        }
//        else if (candi.onDuty()) 
//            cancelAutoCompose(textbar);
    }
    return 0;
}


int OVIMArray::initialize(OVDictionary *, OVService*, const char *mp){
    cintab = new OVCIN("/tmp/array30.cin"); // FIXME: correct the path
    return 1;
}

OV_SINGLE_MODULE_WRAPPER(OVIMArray);
