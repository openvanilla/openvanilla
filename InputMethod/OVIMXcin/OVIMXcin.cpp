// OVIMXcin.cpp

#define OVDEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLoadable.h>
#include <OpenVanilla/OVUtility.h>
#include <stdlib.h>
#include "OVIMXcin.h"
#include "OVCIN.h"
#include "XcinCinList.h"
#include <string>
#include <iostream>

using namespace std;

//OVLOADABLEOBJCWRAPPER;

// functions that help load all .cin --------------------------------

CinList cinlist;

// OpenVanilla Loadable IM interface functions -------------------------------

extern "C" int OVLoadableAvailableIMCount(char* p)
{
    cinlist.load(p);
    return cinlist.index;
}

extern "C" unsigned int OVLoadableVersion()
{
    return ovVersion;
}

extern "C" OVInputMethod* OVLoadableNewIM(int x)
{
	cerr << "loading..." << endl;
    return new OVIMXcin(cinlist.cinpath, cinlist.list[x].filename,
        cinlist.list[x].ename, cinlist.list[x].cname, cinlist.list[x].encoding);
}

extern "C" void OVLoadableDeleteIM(OVInputMethod *im)
{
    delete im;
}

XcinKeySequence::XcinKeySequence(OVCIN* cintab)
{
    cinTable=cintab;
}
    
int XcinKeySequence::valid(char c)
{
	string inKey;
	inKey.push_back(c);
    if (!cinTable->isValidKey(inKey)) return 0;
    return 1;
}
    
int XcinKeySequence::add(char c)
{
    if (valid(c) == 0) return 0;
    return OVKeySequenceSimple::add(c);
}
    
string *XcinKeySequence::compose(string *s)
{
    for (int i=0; i<len; i++)
    {
		string inKey;
		inKey.push_back(seq[i]);
		vector<string> outStringVectorRef;
        if (cinTable->getCharVectorByKey(inKey, outStringVectorRef) > 0)
			s->append(outStringVectorRef[0]);
    }
    return s;
}

OVIMXcin::OVIMXcin(char *lpath, char *cfile, char *en, char *cn,
    OVEncoding enc)
{
    strcpy(loadpath, lpath);
    strcpy(cinfile, cfile);
    cintab=NULL;
   
    cnameencoding=enc;
    sprintf(ename, "OpenVanilla xcin (%s)", en ? en : cfile);
    sprintf(cname, "OpenVanilla xcin (%s)", cn ? cn : cfile);
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
    if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN"))
    {
        *enc=cnameencoding;
        cerr << "asking ename=" << ename;
		cerr << ", cname=" << cname;
		cerr << ", encoding" << *enc;
		cerr << endl;
        return strlen(strcpy((char*)s, cname));
    }
        
    return strlen(strcpy((char*)s, ename));
}

int OVIMXcin::initialize(OVDictionary* global, OVDictionary* local, OVService*, char*)
{
	cerr << "initializing" << endl;
    if (cintab) return 0;
    
    const char *sk="shiftSelectionKey";
    const char *encoding="encoding";
    char buf[256];
    OVEncoding enc=ovEncodingUTF8;
    if (!local->keyExist(sk)) local->setInt(sk, 0);
//  if (!local->keyExist(encoding)) local->setString(encoding, "big5");

    update(global, local);  // run-time configurable settings    
    local->getString(encoding, buf);
    //enc=VXEncodingMapper(buf);
	enc = ovEncodingUTF8;
	//<comment author='b6s'>encoding should be taken care by iconv?</comment>
	
    char cinfilename[PATH_MAX];
    strcpy (cinfilename, loadpath);
    if (cinfilename[strlen(cinfilename)-1]!='/') strcat(cinfilename, "/");
    strcat(cinfilename, cinfile);
    //cintab->read(cinfilename, enc, selkeyshift);
    cintab=new OVCIN(cinfilename);
	
	cerr << "initialized." << endl;
    return 1;
}

int OVIMXcin::update(OVDictionary* global, OVDictionary* local)
{
    const char *warningBeep="warningBeep";
    const char *autoCompose="autoCompose";
    const char *maxSeqLen="maxKeySequenceLength";
    const char *hitMax="hitMaxAndCompose";

    if (!global->keyExist(warningBeep)) global->setInt(warningBeep, 1);
    if (!local->keyExist(maxSeqLen)) local->setInt(maxSeqLen, 5);
    if (!local->keyExist(autoCompose)) local->setInt(autoCompose, 0);
    if (!local->keyExist(hitMax)) local->setInt(hitMax, 0);

    cfgMaxSeqLen=local->getInt(maxSeqLen);
    cfgBeep=global->getInt(warningBeep);
    cfgAutoCompose=local->getInt(autoCompose);
    cfgHitMaxAndCompose=local->getInt(hitMax);
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
        string *ms= new string;
        keyseq.compose(ms);
		cerr << "OVXcinContext::updateDisplay:" << ms->data() << endl;
		char* s = const_cast<char*>(ms->c_str());
        buf->append((void*)s);
        delete ms;
    }
    buf->update();
}


int OVXcinContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, 
    OVService *srv)
{
	cerr << "OVXcinContext::keyEvent" << endl;
    if (candi.onDuty())
    {
		cerr << "OVXcinContext::KeyEvent - if(candi.onDuty())" << endl;
		
        if (!autocomposing) return candidateEvent(key, buf, textbar, srv);
        
        if (string* output=candi.select(key->code()))
        {
            buf->clear()->append((void*)output)->send();
            keyseq.clear();
            cancelAutoCompose(textbar);
            return 1;
        }
    }


    if (!keyseq.length() && !key->isPrintable()) return 0;
    
    if (key->code()==ovkEscape)
    {
        cancelAutoCompose(textbar);
        buf->clear()->update();
        keyseq.clear();
        return 1;
    }
    
    if (key->isCode(2, ovkDelete, ovkBackspace))
    {
        keyseq.remove();
        updateDisplay(buf);
        if (!keyseq.length() && autocomposing) cancelAutoCompose(textbar);
        
        // if autocomposing is on
        if (keyseq.length() && parent->isAutoCompose())
        {
			string inKey(keyseq.getSeq());
			vector<string> outStringVectorRef;
            if (cintab->getWordVectorByChar(inKey, outStringVectorRef) > 0)
            {
                autocomposing=1;
                compose(buf, textbar, srv);
            }
            else if (candi.onDuty()) cancelAutoCompose(textbar);
        }

        return 1;
    }

    if (keyseq.length() && key->code()==ovkSpace)
    {
        // there the candiate window has only one page, space key
        // will send out the first candidate
        if (autocomposing && candi.onDuty())
        {
            keyseq.clear();
            autocomposing=0;
            return candidateEvent(key, buf, textbar, srv);
        }

        autocomposing=0;        
        return compose(buf, textbar, srv);
    }
    
    // shift and capslock processing

    if (key->isPrintable() && (key->isCapslock() || key->isShift()))
    {    
        if (key->isCapslock())
        {
            if (key->isShift()) buf->appendChar(key->upper());
            else buf->appendChar(key->lower());
        }
        else if (key->isShift()) buf->appendChar(key->lower());
        cancelAutoCompose(textbar);
        keyseq.clear();
        buf->send();
        return 1;
    }
    
    if (key->isPrintable() && keyseq.valid(key->code()) &&
        !key->isShift() && !key->isCapslock())
    {
		cerr << "Show keystroke -> character" << endl;
		cerr << "length=" << keyseq.length() << endl;
		cerr << "maxSeqLen=" << parent->maxSeqLen() << endl;
        if (keyseq.length() == parent->maxSeqLen())
        {
            updateDisplay(buf);
            if (parent->isBeep()) srv->beep();
            return 1;
        }
        
        keyseq.add(key->code());
        if (keyseq.length() == parent->maxSeqLen() && parent->isHitMaxAndCompose())
        {
            autocomposing=0;
            return compose(buf, textbar, srv);
        }
		
        updateDisplay(buf);
        if (cintab->isEndKey(key->code()))
        {
            autocomposing=0;
            return compose(buf, textbar, srv);
        }
        
        // if autocomposing is on
        if (parent->isAutoCompose())
        {
			string inKey(keyseq.getSeq());
			vector<string> outStringVectorRef;
            if (cintab->getWordVectorByChar(inKey, outStringVectorRef) > 0)
            {
                autocomposing=1;
                compose(buf, textbar, srv);
            }
            else if (candi.onDuty()) cancelAutoCompose(textbar);
        }
        
        return 1;
    }

    cancelAutoCompose(textbar);
    keyseq.clear();
    if (buf->length()) buf->send();
    
    return 0;
}

void OVXcinContext::cancelAutoCompose(OVTextBar *textbar)
{
    autocomposing=0;
    candi.cancel();
    textbar->hide()->clear();
}
    
        
int OVXcinContext::compose(OVBuffer *buf, OVTextBar *textbar, OVService *srv)
{
    if (!keyseq.length()) return 0;

	vector<string> array;
	int size = cintab->getWordVectorByChar(keyseq.getSeq(), array);
    
    if (size == 0)
    {
        if (parent->isBeep()) srv->beep();
        return 1;
    }

    if (size ==1 && !autocomposing)
    {
        buf->clear()->append((void*)array[0].c_str())->send();
        keyseq.clear();
        return 1;
    }

    if (!autocomposing)
    {    
        buf->clear()->append((void*)array[0].c_str())
            ->update();
        keyseq.clear();
    }
    
    candi.prepare(&array, const_cast<char*>(cintab->getSelKey().c_str()), textbar);    

    return 1;
}

int OVXcinContext::candidateEvent(OVKeyCode *key, OVBuffer *buf, 
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
        (candi.onePage() && key->code()==ovkSpace)) c=cintab->getSelKey()[0];
    
    string *output = new string;
    
    if ((output=candi.select(c)))
    {
        buf->clear()->append((void*)output)->send();
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }
    
	string inKey;
	inKey.push_back(c);
	vector<string> outStringVectorRef;
    if (cintab->getCharVectorByKey(inKey, outStringVectorRef) > 0)
    {
        buf->clear()->append((void*)candi.select(cintab->getSelKey()[0]))->send();
        keyseq.add(c);
        updateDisplay(buf);
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }    

    if (parent->isBeep()) srv->beep();

    return 1;
}
