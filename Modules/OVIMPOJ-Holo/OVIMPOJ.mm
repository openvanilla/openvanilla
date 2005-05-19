// OVIMPOJ.cpp: prototype for POJ IM

// Copyright (c) 2004 by Lukhnos D. Liu, Kang-ming Liu, and Pek-tiong Tan

#define OV_DEBUG
#include "OVIMPOJ.h"
#include <ctype.h>

OV_SINGLE_MODULE_WRAPPER(OVIMPOJ);

const char *optASCIIOutput="ASCIIOutput";
const char *optKeyLayout="keyboardLayout";

OVIMPOJ::OVIMPOJ() {
    fullPOJOutput=asciioutput=beep=0;
    keylayout=pojToneByNumber;
    cintab=NULL;
}

const char* OVIMPOJ::identifier() {
    return "OVIMPOJ-Holo";
}

const char* OVIMPOJ::localizedName(const char *locale) {
    if (!strcasecmp(locale, "zh_TW")) return "河洛白話字漢羅 (POJ)";
    if (!strcasecmp(locale, "zh_CN")) return "河洛白話字汉罗 (POJ)";    
    return "OV Holo Pe̍h-ōe-jī (POJ)";
}

int OVIMPOJ::initialize(OVDictionary* local, OVService* srv, const char* path)
{
    char cinpath[PATH_MAX];
    
    if (!local->keyExist(optASCIIOutput)) local->setInteger(optASCIIOutput, 0);
    if (!local->keyExist(optKeyLayout)) local->setInteger(optKeyLayout, pojToneByNumber);
    local->setInteger("warningBeep", 1);
        
    strcpy(cinpath, path);
    strcat(cinpath, "OVIMPOJ-Holo/poj.cin");        
    murmur ("OVIMPOJ: loading poj.cin at %s", cinpath);
    
    update(local, srv);
    
    cintab=new VXCIN;
    cintab->read(cinpath);

    return 1;
}

void OVIMPOJ::update(OVDictionary* local, OVService*)
{
    asciioutput=local->getInteger(optASCIIOutput);
    keylayout=local->getInteger(optKeyLayout);
    beep=1;     // always use warning beep
    fullPOJOutput=local->getIntegerWithDefault("fullPOJOutput", 0);
}

OVInputMethodContext *OVIMPOJ::newContext()
{
    return new OVIMPOJContext(this, cintab);
}

OVIMPOJContext::OVIMPOJContext(OVIMPOJ *p, VXCIN *ctab)
{
    parent=p;
    cintab=ctab;
    list=[NSMutableArray new];
}

OVIMPOJContext::~OVIMPOJContext()
{
    [list release];
}
    
void OVIMPOJContext::clear()
{
    candi.cancel();
    seq.clear();
}

int OVIMPOJContext::keyEvent(OVKeyCode *key, OVBuffer *buf, 
    OVCandidate *textbar, OVService *srv) 
{
    int ascii=parent->isAsciiOutput();
    char composebuf[256];

    if (candi.onDuty()) return candidateEvent(key, buf, textbar, srv);
 
    //Ignore arrow keys.
    int c=key->code();
    if (c==ovkDown || c==ovkUp || c==ovkLeft || c==ovkRight) 
    	if(seq.length()) return 1;
    	else return 0;
    
    // if backspace of delete key is hit
    if ((c==ovkBackspace || c==ovkDelete) && !buf->isEmpty())
    { 
        seq.remove();
        buf->clear()->append(seq.compose(composebuf, 0))->update();
        return 1;            
    }

    // if enter or compose key is hit
    if ((key->code()==ovkReturn || seq.isCompose(key->code())) && !buf->isEmpty()) 
    {
        char final[256];
    
        if (key->code()!=ovkReturn) seq.add(key->code());
        seq.normalize();
        strcpy(final, seq.compose(composebuf, ascii));
        buf->clear()->append(final)->update();
    
        char query[256];
        strcpy (query, seq.finalize());

        queryAndCompose(query, final, buf, textbar, srv);
        seq.clear();        
        return 1;
    }

    // key=[a-z][A-Z], so add to our key sequence
    if (isalpha(key->code()) || (parent->getKeyLayout() && tonemark(key->code())))
    {
        seq.add(key->code(), parent->getKeyLayout());
        char *b=seq.compose(composebuf, 0);
        buf->clear()->append(b)->update();
        return 1;
    }

    // we find it's not a "valid" character, so we send up the buffer,
    // then tell the app the process the character itself    
    if (!buf->isEmpty())
    {
        seq.normalize();
        buf->clear()->append(seq.compose(composebuf, ascii))->send();
    }

    seq.clear();        
    return 0;
}

void OVIMPOJContext::queryAndCompose(char *qstr, char *disp, OVBuffer *buf, 
    OVCandidate *textbar, OVService *srv)
{
    // we have to "lowercase" every character in the query string
    char *p=qstr;
    while (*p) {
        *p=tolower(*p);
        p++;
    }
    
    NSArray *result=cintab->find(qstr);

    if (parent->isFullPOJ())
    {
        buf->send();
    }
    else
    {
        if (!result)
        {
            buf->send();
        }
        else
        {
            [list removeAllObjects];
            [list addObject: [NSString stringWithUTF8String: disp]];
            [list addObjectsFromArray: result];    
            candi.prepare(list, cintab->selKey(), textbar);
        }
    }
}

int OVIMPOJContext::candidateEvent(OVKeyCode *key, OVBuffer *buf, 
    OVCandidate *textbar, OVService *srv)
{
    int c=key->code();
    if (c==ovkEsc ||c==ovkBackspace)
    {
        textbar->hide()->clear();
        candi.cancel();
        buf->clear()->update();
        return 1;
    }

    if ((c==ovkDown || c==ovkLeft || c=='>') ||
        (!candi.onePage() && key->code()==ovkSpace))
    {
        candi.pageDown()->update(textbar);
        return 1;
    }

    if (c==ovkUp || c==ovkRight || c=='<')
    {
        candi.pageUp()->update(textbar);
        return 1;
    }

    // enter == first candidate
    // space (when candidate list has only one page) == first candidate

    if (c==ovkReturn || 
        (candi.onePage() && key->code()==ovkSpace)
        || key->code()=='-') c=*(cintab->selKey());
    
    NSString *output;
    
    if ((output=candi.select(c)))
    {
        buf->clear()->append([output UTF8String]);
        if (candi.onePage() && key->code()==ovkSpace) buf->append((char*)" ");
        else if (key->code()=='-') buf->append((char*)"-");
        buf->send();
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }
    
    if ((output=cintab->getKey(c)))
    {
        buf->clear()->append([candi.select(*(cintab->selKey())) UTF8String])->send();
        seq.add(c);
        
        char composebuf[256];
        char *b=seq.compose(composebuf, 0);
        buf->clear()->append(b)->update();

        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }    

    if (parent->isBeep()) srv->beep();

    return 1;
}

