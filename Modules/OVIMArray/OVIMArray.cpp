#define OV_DEBUG
#include"OVIMArray.h"
#include<OpenVanilla/OVLibrary.h>
#include<OpenVanilla/OVUtility.h>
#include <ctype.h>
#include <stdio.h>

int OVIMArrayContext::keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* srv)
{
    murmur("OVIMArray: key event!");
    if (isprint(k->code()))
    {
        char str[2];
        sprintf(str, "%c", k->code());
        b->append(str)->update();
        srv->notify(str);
        i->show();
        i->append(str)->update();
        return 1;
    }
    
    if (k->code()==13) {
        if (!b->isEmpty()) {
            b->send();
            i->hide();
            return 1;
        }
        return 0;
    }
    
    if (!b->isEmpty()) {
        b->send();
        i->hide();
    }
            
    return 0;
}


int OVIMArray::initialize(OVDictionary *, OVService*, const char *mp){
    cintab = new OVCIN("/tmp/array30.cin"); // FIXME: correct the path
    return 1;
}
//OV_SINGLE_MODULE_WRAPPER(OVIMArray);
