// OVIMPOJ.cpp: prototype for POJ IM

// Copyright (c) 2004 by Lukhnos D. Liu, Kang-ming Liu, and Pek-tiong Tan

#define OVDEBUG 1
#include "OVIMPOJ.h"

OVLOADABLEWRAPPER(OVIMPOJ);
OVLOADABLEOBJCWRAPPER;

const char *optASCIIOutput="ASCIIOutput";
const char *optKeyLayout="keyboardLayout";
const char *optBeep="warningBeep";

int OVIMPOJ::identifier(char* s)
{
    return strlen(strcpy(s, "OVIMPOJ"));
}

int OVIMPOJ::name(char *locale, void *s, OVEncoding *enc)
{
    *enc=ovEncodingUTF8;
    if (!strcasecmp(locale, "zh_TW"))
        return strlen(strcpy((char*)s, "OV 白話字(POJ)漢羅"));
    if (!strcasecmp(locale, "zh_CN"))
        return strlen(strcpy((char*)s, "OV 白話字(POJ)汉罗"));
    return strlen(strcpy((char*)s, "OV Pe̍h-ōe-jī (POJ) IM"));
}

int OVIMPOJ::initialize(OVDictionary* global, OVDictionary* local,
    OVService* srv, char* path)
{
    char cinpath[PATH_MAX];
    
    if (!local->keyExist(optASCIIOutput))
        local->setInt(optASCIIOutput, 0);
    if (!local->keyExist(optKeyLayout))
        local->setInt(optKeyLayout, pojToneByNumber);
        
    strcpy(cinpath, path);
    strcat(cinpath, "OVIMPOJ/poj.cin");        
    murmur ("OVIMPOJ: loading poj.cin at %s", cinpath);
    
    update(global, local);
    
    cintab=new VXCIN;
    cintab->read(cinpath);

    return 1;
}

int OVIMPOJ::terminate(OVDictionary*, OVDictionary*, OVService*)
{
    if (cintab) delete cintab;
    return 1;
}

int OVIMPOJ::update(OVDictionary* global, OVDictionary* local)
{
    asciioutput=local->getInt(optASCIIOutput);
    keylayout=local->getInt(optKeyLayout);
    beep=global->getInt(optBeep);
    return 1;
}

OVIMContext *OVIMPOJ::newContext()
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
    
int OVIMPOJContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
OVService *srv) 
{
    int ascii=parent->isAsciiOutput();
    char composebuf[256];

    if (candi.onDuty()) return candidateEvent(key, buf, textbar, srv);

    
    // if backspace of delete key is hit
    if (key->isCode(2, ovkBackspace, ovkDelete) && buf->length())
    { 
        seq.remove();
        buf->clear()->append(seq.compose(composebuf, 0))->update();
        return 1;            
    }

    // if enter or compose key is hit
    if ((key->code()==ovkReturn || seq.isCompose(key->code())) && buf->length()) 
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
    if (key->isAlpha() || (parent->getKeyLayout() && tonemark(key->code())))
    {
        seq.add(key->code(), parent->getKeyLayout());
        char *b=seq.compose(composebuf, 0);
        buf->clear()->append(b)->update();
        return 1;
    }

    // we find it's not a "valid" character, so we send up the buffer,
    // then tell the app the process the character itself    
    if (buf->length())
    {
        seq.normalize();
        buf->clear()->append(seq.compose(composebuf, ascii))->send();
    }

    seq.clear();        
    return 0;
}

void OVIMPOJContext::queryAndCompose(char *qstr, char *disp, OVBuffer *buf, 
    OVTextBar *textbar, OVService *srv)
{
    NSArray *result=cintab->find(qstr);
    
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

int OVIMPOJContext::candidateEvent(OVKeyCode *key, OVBuffer *buf, 
    OVTextBar *textbar, OVService *srv)
{
    if (key->isCode(2, ovkEscape, ovkBackspace))
    {
        textbar->hide()->clear();
        candi.cancel();
        buf->clear()->update();
        return 1;
    }

    if (key->isCode(3, ovkDown, ovkLeft, '>') ||
        (!candi.onePage() && key->code()==ovkSpace))
    {
        candi.pageDown()->update(textbar);
        return 1;
    }

    if (key->isCode(3, ovkUp, ovkRight, '<'))
    {
        candi.pageUp()->update(textbar);
        return 1;
    }

    // enter == first candidate
    // space (when candidate list has only one page) == first candidate
    char c=key->code();
    if (key->isCode(2, ovkReturn, ovkMacEnter) || 
        (candi.onePage() && key->code()==ovkSpace)
        || key->code()=='-') c=*(cintab->selKey());
    
    NSString *output;
    
    if ((output=candi.select(c)))
    {
        buf->clear()->append((void*)[output UTF8String]);
        if (candi.onePage() && key->code()==ovkSpace) buf->append((char*)" ");
        else if (key->code()=='-') buf->append((char*)"-");
        buf->send();
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }
    
    if ((output=cintab->getKey(c)))
    {
        buf->clear()->append((void*)[candi.select(*(cintab->selKey())) UTF8String])->send();
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

