// OVIMXcin.mm

#define OVDEBUG
#include <OpenVanilla/OVLoadable.h>
#include "OVIMXcin.h"

OVLOADABLEOBJCWRAPPER;

char cinpath[PATH_MAX];
extern "C" int OVLoadableAvailableIMCount(char* p)
{
    strcpy(cinpath, p);
    return 1;
}

extern "C" unsigned int OVLoadableVersion()
{
    return ovVersion;
}

extern "C" OVInputMethod* OVLoadableNewIM(int)
{
    return new OVIMXcin(cinpath, "test.cin");
}

extern "C" void OVLoadableDeleteIM(OVInputMethod *im)
{
    delete im;
}

XcinKeySequence::XcinKeySequence(VXCIN* cintab)
{
    cinTable=cintab;
}
    
int XcinKeySequence::add(char c)
{
    if (!cinTable->getKey(c)) return 0;
    return OVKeySequenceSimple::add(c);
}
    
NSString *XcinKeySequence::compose(NSMutableString *s)
{
    for (int i=0; i<len; i++)
    {
        NSString *ks;
        if (ks=cinTable->getKey(seq[i])) [s appendString: ks];
    }
    return s;
}

OVIMXcin::OVIMXcin(char *lpath, char *cfile)
{
    strcpy(loadpath, lpath);
    strcpy(cinfile, cfile);
    cintab=NULL;
}

OVIMXcin::~OVIMXcin()
{
    if (cintab) delete cintab;
}

int OVIMXcin::identifier(char *s)
{
    char idbuf[256];
    strcpy(idbuf, "OVIMXcin-");
    strcat(idbuf, cinfile);
    return strlen(strcpy(s, idbuf));
}

int OVIMXcin::name(char *locale, void *s, OVEncoding *enc)
{
    *enc=ovEncodingUTF8;
    if (!cintab)
    {
        strcpy((char*)s, "OVIMXCin (");
        strcat((char*)s, cinfile);
        strcat((char*)s, ")");
        return strlen((char*)s);
    }

    return cintab->name(locale, s, enc);
}

int OVIMXcin::initialize(OVDictionary*, OVDictionary* local, OVService*, char*)
{
    if (cintab) return 0;
    
    const char *sk="shiftSelectionKey";
    const char *encoding="encoding";
    char buf[256];
    OVEncoding enc=ovEncodingUTF8;
    if (!local->keyExist(sk)) local->setInt(sk, 0);
    if (!local->keyExist(encoding)) local->setString(encoding, "big5");
    
    int selkeyshift=local->getInt(sk);
    local->getString(encoding, buf);
    
    if (!strcasecmp(buf, "big5")) enc=ovEncodingBig5HKSCS;
    if (!strcasecmp(buf, "euc")) enc=ovEncodingEUC_CN;
    if (!strcasecmp(buf, "euc_cn")) enc=ovEncodingEUC_CN;

    char cinfilename[PATH_MAX];
    strcpy (cinfilename, loadpath);
    if (cinfilename[strlen(cinfilename)-1]!='/') strcat(cinfilename, "/");
    strcat(cinfilename, cinfile);
    cintab=new VXCIN;
    cintab->read(cinfilename, enc, selkeyshift);
    return 1;
}

int OVIMXcin::terminate(OVDictionary*, OVDictionary*, OVService*)
{
    delete cintab;
    cintab=NULL;
    return 1;
}

OVIMContext *OVIMXcin::newContext()
{
    return new OVXcinContext(this, cintab);
}

void OVXcinContext::updateDisplay(OVBuffer *buf)
{
    buf->clear();
    if (keyseq.length())
    {
        NSMutableString *ms=[NSMutableString new];
        keyseq.compose(ms);
        buf->append((void*)[ms UTF8String]);
        [ms release];
    }
    buf->update();
}


int OVXcinContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, 
    OVService *srv)
{
    if (!keyseq.length() && !key->isPrintable()) return 0;
    
    if (key->code()==ovkEscape)
    {
        buf->clear()->update();
        keyseq.clear();
        return 1;
    }
    
    if (key->isCode(2, ovkDelete, ovkBackspace))
    {
        keyseq.remove();
        updateDisplay(buf);
        return 1;
    }

    if (keyseq.length() && key->code()==ovkSpace) return compose(buf);
    
    if (key->isPrintable())
    {
        // process shift/capslock key
        if (key->isShift() || key->isCapslock())
        {
            if (keyseq.length()) 
            {
                keyseq.clear();
                buf->clear()->update();
            }
            return 0;
        }
        
        if (keyseq.add(key->code()))
        {
            updateDisplay(buf);
            if (cintab->isEndKey(key->code())) return compose(buf);
            return 1;
        }
    }

    if (buf->length()) 
    {
        keyseq.clear();
        buf->send();
    }
    
    return 0;
}
        
int OVXcinContext::compose(OVBuffer *buf)
{
    if (!keyseq.length()) return 0;

    NSMutableArray *array=cintab->find(keyseq.getSeq());
    
    if (!array) 
    {
        // should give a beep
        return 1;
    }

    if ([array count]==1)
    {
        buf->clear()->append((void*)[[array objectAtIndex: 0] UTF8String])->send();
        keyseq.clear();
        return 1;
    }

    for (int i=0; i<[array count]; i++)
    {
        murmur ("candidate %d = %s", i, [[array objectAtIndex: i] UTF8String]);
    }

    buf->clear()->append((void*)[[array objectAtIndex: 1] UTF8String])->send();
    
    keyseq.clear();
    return 1;
}

