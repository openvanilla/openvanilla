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

int OVIMArrayContext::compose(OVBuffer *buf, OVCandidate *textbar, OVService *srv)
{
    if (!keyseq.length()) return 0;

    int size = cintab->getWordVectorByChar(keyseq.getSeq(), candidateStringVector);

/*    
    if( size==0 ){
        if (parent->isBeep()) srv->beep();
        return 1;
    }

    if( size==1 && !autocomposing){
        buf->clear()->append(candidateStringVector[0].c_str())->send();
        keyseq.clear();
        return 1;
    }

    if (!autocomposing)
    {    
        buf->clear()->append((void*)[[array objectAtIndex: 0] UTF8String])
            ->update();
        keyseq.clear();
    }
*/
    string currentSelKey = cintab->getSelKey();

    candi_list.prepare(&candidateStringVector,
                          const_cast<char*>(currentSelKey.c_str()), textbar);

    return 1;
}

int OVIMArrayContext::candidateEvent(OVKeyCode* key, OVBuffer* buf, 
                                     OVCandidate* candi_bar, OVService* srv)
{
    // enter == first candidate
    // space (when candidate list has only one page) == first candidate
    char c=key->code();
    murmur("candidateEvent: %c\n", c);
    if (key->code() == ovkReturn || (candi_list.onePage() && key->code()==ovkSpace)) 
        c=candi_list.getSelKey()[0];
    murmur("candidateEvent2: %c\n", c);

    string output;
    if (candi_list.select(c, output)) {
        buf->clear()->append(output.c_str())->send();
        candi_list.cancel();
        candi_bar->hide()->clear();
        return 1;
    }
    return 1;
}

int OVIMArrayContext::keyEvent(OVKeyCode* key, OVBuffer* buf, 
                               OVCandidate* candi_bar, OVService* srv)
{
    const char keycode = key->code();
    if( candi_list.onDuty() )
        candidateEvent(key, buf, candi_bar, srv);
    
    if (keyseq.length() && keycode == ovkSpace){
        if (keyseq.length()==1 && keyseq.getSeq()[0]=='t'){
            buf->clear()->append((char*)"的")->send();
            keyseq.clear();
            //cancelAutoCompose(textbar);
            return 1;
        }
        return compose(buf, candi_bar, srv);
    }
    
    if( isprint(keycode) && keyseq.valid(keycode) &&
        !(key->isCtrl() || key->isOpt() || key->isCommand()) ){
        if( keyseq.length() == 4 && tolower(keycode) != 'i' ){
            return 1;
        }
        keyseq.add(keycode);
        updateDisplay(buf);
    }
    return 0;
}


int OVIMArray::initialize(OVDictionary *, OVService*, const char *mp){
    cintab = new OVCIN("/tmp/array30.cin"); // FIXME: correct the path
    return 1;
}

OV_SINGLE_MODULE_WRAPPER(OVIMArray);
