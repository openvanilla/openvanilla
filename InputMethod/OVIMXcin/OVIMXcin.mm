// OVIMXcin.mm

#define OVDEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLoadable.h>
#include <stdlib.h>
#include "OVIMXcin.h"
#include "XcinCinList.h"

OVLOADABLEOBJCWRAPPER;

// functions that help load all .cin --------------------------------

CinList cinlist;
#ifdef OVIMARRAY
	char arraypath[PATH_MAX];
#endif
// OpenVanilla Loadable IM interface functions -------------------------------

extern "C" int OVLoadableAvailableIMCount(char* p)
{
	#ifndef OVIMARRAY
	    cinlist.load(p);
    	return cinlist.index;
    #else
    	strcpy(arraypath, p);
    	return 1;
    #endif
}

extern "C" unsigned int OVLoadableVersion()
{
    return ovVersion;
}

extern "C" OVInputMethod* OVLoadableNewIM(int x)
{
	#ifndef OVIMARRAY
	    return new OVIMXcin(cinlist.cinpath, cinlist.list[x].filename,
    	    cinlist.list[x].ename, cinlist.list[x].cname, cinlist.list[x].encoding);
    #else
    	return new OVIMXcin(arraypath, "array30.cin", NULL, NULL, ovEncodingUTF8);
    #endif
}

extern "C" void OVLoadableDeleteIM(OVInputMethod *im)
{
    delete im;
}

XcinKeySequence::XcinKeySequence(VXCIN* cintab)
{
    cinTable=cintab;
}
    
int XcinKeySequence::valid(char c)
{
    if (!cinTable->getKey(c)) return 0;
    return 1;
}
    
int XcinKeySequence::add(char c)
{
//	we're pretty sure that IM will only feed valid chars to this function,
//  so we ignore the check
//  if (!cinTable->getKey(c)) return 0;
    return OVKeySequenceSimple::add(c);
}
    
NSString *XcinKeySequence::compose(NSMutableString *s)
{
    for (int i=0; i<len; i++)
    {
        NSString *ks;
        if ((ks=cinTable->getKey(seq[i]))) [s appendString: ks];
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
#ifndef OVIMARRAY    
    sprintf(ename, "OpenVanilla xcin (%s)", en ? en : cfile);
    sprintf(cname, "OpenVanilla xcin (%s)", cn ? cn : cfile);
#else
	strcpy(ename, "OpenVanilla Array30");
	strcpy(cname, "OpenVanilla 行列輸入法");
#endif
}

OVIMXcin::~OVIMXcin()
{
    if (cintab) delete cintab;
}

int OVIMXcin::identifier(char *s)
{
    char idbuf[256];
#ifndef OVIMARRAY        
    strcpy(idbuf, "OVIMXcin-");
    strcat(idbuf, cinfile);
#else
	strcpy(idbuf, "OVIMArray");http://the.taoofmac.com/space/blog/2004-12-04.16%3A24
#endif
    return strlen(strcpy(s, idbuf));
}

int OVIMXcin::name(char *locale, void *s, OVEncoding *enc)
{
    *enc=ovEncodingUTF8;
    if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN"))
    {
        *enc=cnameencoding;
        murmur ("asking ename=%s, cname=%s, encoding=%d", ename, cname, *enc);
        return strlen(strcpy((char*)s, cname));
    }
        
    return strlen(strcpy((char*)s, ename));
}

int OVIMXcin::initialize(OVDictionary* global, OVDictionary* local, OVService*, char*)
{
    if (cintab) return 0;
    
    const char *sk="shiftSelectionKey";
    const char *encoding="encoding";
    char buf[256];
    OVEncoding enc=ovEncodingUTF8;
    if (!local->keyExist(sk)) local->setInt(sk, 0);
//  if (!local->keyExist(encoding)) local->setString(encoding, "big5");

    update(global, local);  // run-time configurable settings    
    int selkeyshift=local->getInt(sk);
    local->getString(encoding, buf);
    enc=VXEncodingMapper(buf);

    char cinfilename[PATH_MAX];
    strcpy (cinfilename, loadpath);
    if (cinfilename[strlen(cinfilename)-1]!='/') strcat(cinfilename, "/");
    strcat(cinfilename, cinfile);
    cintab=new VXCIN;
    cintab->read(cinfilename, enc, selkeyshift);
    return 1;
}

int OVIMXcin::update(OVDictionary* global, OVDictionary* local)
{
    const char *warningBeep="warningBeep";
    const char *autoCompose="autoCompose";
    const char *maxSeqLen="maxKeySequenceLength";
    const char *hitMax="hitMaxAndCompose";
    const char *sendSpaceWhenAutoCompose="sendSpaceWhenAutoCompose";
#ifndef OVIMARRAY
    if (!global->keyExist(warningBeep)) global->setInt(warningBeep, 1);
    if (!local->keyExist(maxSeqLen)) local->setInt(maxSeqLen, 5);
    if (!local->keyExist(autoCompose)) local->setInt(autoCompose, 0);
    if (!local->keyExist(hitMax)) local->setInt(hitMax, 0);
	if (!local->keyExist(sendSpaceWhenAutoCompose))
		local->setInt(sendSpaceWhenAutoCompose, 1);
#else
    if (!global->keyExist(warningBeep)) global->setInt(warningBeep, 1);
    if (!local->keyExist(maxSeqLen)) local->setInt(maxSeqLen, 5);
    if (!local->keyExist(autoCompose)) local->setInt(autoCompose, 1);
    if (!local->keyExist(hitMax)) local->setInt(hitMax, 0);
	if (!local->keyExist(sendSpaceWhenAutoCompose))
		local->setInt(sendSpaceWhenAutoCompose, 0);
#endif
    cfgMaxSeqLen=local->getInt(maxSeqLen);
    cfgBeep=global->getInt(warningBeep);
    cfgAutoCompose=local->getInt(autoCompose);
    cfgHitMaxAndCompose=local->getInt(hitMax);
    cfgSendSpaceWhenAutoCompose=local->getInt(sendSpaceWhenAutoCompose);
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

#ifdef OVIMARRAY
	if (keyseq.length()==1)
	{
		if (*(keyseq.getSeq())=='t')
		{
			buf->append((char*)"的")->update();
			return;
		}
		if (*(keyseq.getSeq())=='w')
		{
			buf->append((char*)"女")->update();
			return;
		}		
	}
	if (keyseq.length()==2 && *(keyseq.getSeq())=='w')
	{
		char c=(keyseq.getSeq())[1];
		if (c >= '1' && c <= '9')
		{
			buf->append(keyseq.getSeq())->update();
			return;
		}
	}
#endif

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
    if (candi.onDuty())
    {
        if (!autocomposing) return candidateEvent(key, buf, textbar, srv);
        
        if (NSString* output=candi.select(key->code()))
        {
            buf->clear()->append((void*)[output UTF8String])->send();
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
            if (cintab->find(keyseq.getSeq()))
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
#ifdef OVIMARRAY
		if (keyseq.length()==1 && *(keyseq.getSeq())=='t')
		{
			buf->clear()->append((char*)"的")->send();
        	keyseq.clear();
        	cancelAutoCompose(textbar);
        	return 1;
		}
#endif

        // if the candiate window has only one page, space key
        // will send out the first candidate
        if (autocomposing && candi.onDuty() &&
        	parent->isSendSpaceWhenAutoCompose())
        {
	    	autocomposing=0;
            keyseq.clear();
            return candidateEvent(key, buf, textbar, srv);
        }

    	autocomposing=0;
        return compose(buf, textbar, srv);
    }
    
    int validkey=keyseq.valid(key->code());

#ifdef OVIMARRAY
	if (keyseq.length()==1 && *(keyseq.getSeq())=='w' &&
	    key->code() >= '1' && key->code() <= '9') validkey=1;
#endif
    
    // shift and capslock processing
    if (key->isPrintable() && 
        (key->isCapslock() || 
         (key->isShift() && !validkey)
        ))
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
    
    if (key->isPrintable() && validkey &&
        !(key->isCtrl() || key->isOpt() || key->isCommand()))
    {
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
        
        
        
        if (cintab->isEndKey(key->code())
#ifdef OVIMARRAY
            || (keyseq.length()==2 && *(keyseq.getSeq())=='w' &&
                key->code() >= '1' && key->code() <= '9')
#endif
           )
        {
            autocomposing=0;
            return compose(buf, textbar, srv);
        }
        
        // if autocomposing is on
        if (parent->isAutoCompose()
#ifdef OVIMARRAY
		    && !(keyseq.length()==1 && *(keyseq.getSeq())=='w')
#endif       
           )
        {
            if (cintab->find(keyseq.getSeq()))
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

    NSArray *array=cintab->find(keyseq.getSeq());
    
    if (!array) 
    {
        if (parent->isBeep()) srv->beep();
        return 1;
    }

    if ([array count]==1 && !autocomposing)
    {
        buf->clear()->append((void*)[[array objectAtIndex: 0] UTF8String])->send();
        keyseq.clear();
        return 1;
    }

    if (!autocomposing)
    {    
        buf->clear()->append((void*)[[array objectAtIndex: 0] UTF8String])
            ->update();
        keyseq.clear();
    }
    
    candi.prepare(array, cintab->selKey(), textbar);    

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
        (candi.onePage() && key->code()==ovkSpace)) c=*(cintab->selKey());
    
    NSString *output;
    
    if ( (output=candi.select(c)) )
    {
        buf->clear()->append((void*)[output UTF8String])->send();
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }
    
    if ((output=cintab->getKey(c)))
    {
        buf->clear()->append((void*)[candi.select(*(cintab->selKey())) UTF8String])->send();
        keyseq.add(c);
        updateDisplay(buf);
        candi.cancel();
        textbar->hide()->clear();
        return 1;
    }    

    if (parent->isBeep()) srv->beep();

    return 1;
}
