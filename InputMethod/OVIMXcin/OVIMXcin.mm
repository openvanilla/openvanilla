// OVIMXcin.mm

#define OVDEBUG
#include "OVIMXcin.h"
#include <sys/param.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <OpenVanilla/OVLoadable.h>

OVLOADABLEOBJCWRAPPER;

const int vxMaxCINFiles=32;

int cinpos=0;
char cinpath[PATH_MAX];
char cinfile[vxMaxCINFiles][PATH_MAX];

char selectfilter[PATH_MAX];

int file_select(struct dirent *entry)
{
    int p=strlen(entry->d_name)-strlen(selectfilter);
    if (p<0) return 0;
    if (!strcmp(entry->d_name+p, selectfilter)) return 1; 
    return 0;
}

void addglob(char *libpath, char *ext)
{
    struct dirent **files;
    char searchpath[PATH_MAX];

    if (ext) strcpy(selectfilter, ext); else strcpy(selectfilter, ".cin");

    strcpy(searchpath, libpath);
    int l=strlen(searchpath);
    if (l) if (searchpath[l-1]=='/') searchpath[l-1]=0;

    murmur ("VXLibraryList::addglob, path=%s, ext=%s", searchpath, ext);    
    int count=scandir(searchpath, &files, file_select, alphasort); 
    if (count<=0) return;
    
    for (int i=0; i<count; i++)
    {
        strcpy (cinfile[cinpos++], files[i]->d_name);
    	murmur ("OVIMXcin: adding .cin file %s", files[i]->d_name);
        free(files[i]);
    }
    free(files);
}

extern "C" int OVLoadableAvailableIMCount(char* p)
{
    strcpy(cinpath, p);
    addglob(cinpath, ".cin");
    return cinpos;
}

extern "C" unsigned int OVLoadableVersion()
{
    return ovVersion;
}

extern "C" OVInputMethod* OVLoadableNewIM(int x)
{
    return new OVIMXcin(cinpath, cinfile[x]);
}

extern "C" void OVLoadableDeleteIM(OVInputMethod *im)
{
    delete im;
}

void XcinCandidate::prepare(NSArray *l, char *skey, OVTextBar *textbar)
{
    onduty=1;
    list=l;
    strcpy(selkey, skey);
    perpage=strlen(selkey);
    pos=0;
    count=[list count];
    
    murmur ("prepare, selkey=%s, perpage=%d, pos=%d, count=%d",
        selkey, perpage, pos, count);
    
    update(textbar);
    textbar->show();
}

void XcinCandidate::update(OVTextBar *textbar)
{
    char buf[256];
    int bound=pos+perpage;
    if (bound > count) bound=count;
    
    textbar->clear();
    
    for (int i=pos, j=0; i<bound; i++, j++)
    {
        sprintf (buf, "%c.", selkey[j]);
        textbar->append(buf)->append((void*)[[list objectAtIndex: i] UTF8String])->
            append((void*)" ");
    }
    
    int totalpage=(count/perpage)+1;
    int currentpage=(pos/perpage)+1;
    sprintf (buf, "(%d/%d)", currentpage, totalpage);
    textbar->append(buf);
    textbar->update();   
}

XcinCandidate* XcinCandidate::pageUp()
{
    pos-=perpage;
    if (pos < 0) pos=0;
    return this;
}

XcinCandidate* XcinCandidate::pageDown()
{
    int oldpos=pos;
    pos+=perpage;  
    if (pos > count) pos=0;
    return this;
}

NSString* XcinCandidate::select(char c)
{
    int i;
    for (i=0; i<perpage; i++)
        if (selkey[i]==c)
        {
            onduty=0;
            return [list objectAtIndex: pos+i];
        }
        
    return NULL;        
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
    if (candi.onDuty()) return candidateEvent(key, buf, textbar);

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

    if (keyseq.length() && key->code()==ovkSpace) return compose(buf, textbar);
    
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
            if (cintab->isEndKey(key->code())) return compose(buf, textbar);
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
        
int OVXcinContext::compose(OVBuffer *buf, OVTextBar *textbar)
{
    if (!keyseq.length()) return 0;

    NSArray *array=cintab->find(keyseq.getSeq());
    
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

    buf->clear()->append((void*)[[array objectAtIndex: 0] UTF8String])->update();
    candi.prepare(array, cintab->selKey(), textbar);    
    keyseq.clear();
    return 1;
}

int OVXcinContext::candidateEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar)
{
    if (key->isCode(2, ovkEscape, ovkBackspace))
    {
        textbar->hide()->clear();
        candi.cancel();
        buf->clear()->update();
        return 1;
    }

    if (key->isCode(4, ovkSpace, ovkDown, ovkLeft, '>'))
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
    char c=key->code();
    if (key->isCode(2, ovkReturn, ovkMacEnter)) c=*(cintab->selKey());
    
    NSString *output;
    
    if (output=candi.select(c))
    {
        buf->clear()->append((void*)[output UTF8String])->send();
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }
    
    if (output=cintab->getKey(c))
    {
        buf->clear()->append((void*)[candi.select(*(cintab->selKey())) UTF8String])->send();
        keyseq.add(c);
        updateDisplay(buf);
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }    

    // invalid key, let's beep?

    return 1;
}
