// OVIMPhone.cpp

#define OV_DEBUG

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLibrary.h"
#include "OpenVanilla/OVUtility.h"
#include "vanillaphone.h"

const int vimclMaxCandidate=256;
const int vimclCandidatePerPage=9;

class CandidateList
{
public:
	CandidateList();
	int load(VPUTF16* str, int len);
	int updatecontent(VPUTF16* str);
	int pageup();
	int pagedown();
	VPUTF16 select(int candidate);		// starting from 0, as this is not key sequence!!

protected:
	int appendutf16(VPUTF16 *us, char *cs);
	
	int size;
	VPUTF16 list[vimclMaxCandidate];
	int currentpage, totalpage;
};


class OVPhoneIM;

class OVPhoneContext : public OVInputMethodContext
{
public:
    OVPhoneContext(OVPhoneIM *p)
    {
        parent=p;

        vimPunctuationWindow=0;
        vimCandidateWindow=0;
        vimCurrentSymbol=0;
        vimKeyboardLayoutEten=1;
    }
    
    virtual ~OVPhoneContext()
    {
    }
    
    
    virtual void clear()
    {
        // textbar will be forced to close anyway, so we can just set them
        // to 0 without extra worries
        vimPunctuationWindow=vimCandidateWindow=0;
        vimCurrentSymbol=0;
    }
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar,
        OVService *srv);

    
protected:
    CandidateList vimCandidatelist;
    int vimPunctuationWindow;
    int vimCandidateWindow;
    VPSymbol vimCurrentSymbol;
    int vimKeyboardLayoutEten;

    OVPhoneIM *parent;
};

class OVPhoneIM : public OVInputMethod
{
public:
    OVPhoneIM()
    {
        keylayout=0;
    }
    
    virtual const char *identifier()
    {
        return "OVIMPhonetic";
    }
    
    virtual const char *localizedName(const char *locale)
    {
        if (!strcasecmp(locale, "zh_TW"))
            return "注音";
        else if (!strcasecmp(locale, "zh_CN"))
            return "正体注音";
        else
            return "Phonetic (Bopomofo)";
    }

    virtual int initialize(OVDictionary* l, OVService*, const char*)
    {
	    if (!l->keyExist("keyboardLayout")) l->setInteger("keyboardLayout", 0);
	    keylayout=l->getInteger("keyboardLayout");
        return 1;
    }
    
    virtual void update(OVDictionary* l, OVService*)
    {
	    if (!l->keyExist("keyboardLayout")) l->setInteger("keyboardLayout", 0);
	    keylayout=l->getInteger("keyboardLayout");
    }

    virtual OVInputMethodContext* newContext() 
    {
        return new OVPhoneContext(this); 
    }
    
    virtual int getKeyLayout() { return keylayout; }
    
protected:
    int keylayout;
};


// use these two wrappers
OV_SINGLE_MODULE_WRAPPER(OVPhoneIM);


int OVPhoneContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate 
    *textbar, OVService *srv)
{
    int inCharCode=key->code();
    
    if (parent->getKeyLayout()==1) vimKeyboardLayoutEten=1;
    else vimKeyboardLayoutEten=0;
    
    
	if (vimPunctuationWindow)
	{
		if (inCharCode==27)
		{
			vimPunctuationWindow=0;
			textbar->hide();
			return 1;
		}
		
		VPUTF16 uc=VPPunctuationWindowKeyToCode(inCharCode);
		if (uc)
		{
			vimPunctuationWindow=0;
			textbar->hide();
			const char *u8=srv->UTF16ToUTF8(&uc, 1);
			buf->clear()->append(u8)->send();
			return 1;
		}
		
		// none above
		return 1;
	}
	
	// if in candidate window mode
	// acceaptable key: ESC=cancel, SPACE/DOWN/LEFT=page down, UP/RIGHT=page up,
	//					'1'-'9'=choose candidate
	
	if (vimCandidateWindow)
	{
		int refresh=0;

		if (inCharCode==27 || inCharCode==0x8)		// backspace/esc cancles the window
		{
			vimCandidateWindow=0;
			textbar->hide();
			buf->clear()->update();
			return 1;
		}
		
		if (inCharCode==32 || inCharCode==0x1c || inCharCode==0x1f || inCharCode=='>')  // space/left/down/">"
		{
			vimCandidatelist.pagedown();
			refresh=1;
		}
		
		if (inCharCode==0x1d || inCharCode==0x1e || inCharCode=='<')   // right/up
		{
			vimCandidatelist.pageup();
			refresh=1;
		}
		
		// OK, this is really a special case and I still remember clearly that
		// Eten Company then claimed it to be an "accelerating" feature...
		// that is, in a candidate window, if you type a key that is MEANINGFUL,
		// the first candidate would be chosen, and then...
		// you have guessed correctly, the key will stay
		
		// TO-DO: THIS STICKY KEY HAS TO BE DISPLAYED IMMEDIATELY AFTER
		// THE FIRST CANDIDATE IS "AUTOMATICALLY" CHOSEN, BUT HOW TO DISPLAY IT?
		VPSymbol accs;
		int refreshacckey=0;
		if (vimKeyboardLayoutEten) accs=VPEtenKeyToSymbol(inCharCode);
		else accs=VPStandardKeyToSymbol(inCharCode);		
		
		if ((inCharCode < '1' || inCharCode > '9') && accs)
		{
			vimCurrentSymbol=accs;		// we'll leave it, stay!
			inCharCode='1';		// and cheat the system, hey, that's an _esprit du hacking_
			refreshacckey=1;
		}
		
		if (inCharCode == 0xd || inCharCode == 0x3)		// return/enter
		{
			inCharCode='1';
		}
		
		if (inCharCode >= '1' && inCharCode <= '9')
		{
			VPUTF16 uc=vimCandidatelist.select(inCharCode-'1');
			if (uc)
			{
				vimCandidateWindow=0;
				textbar->hide();
				const char *u8=srv->UTF16ToUTF8(&uc, 1);
				buf->clear()->append(u8)->send();
				
				// trying: display "accelertor" key -- WORKED!
				if (refreshacckey)
				{
					VPUTF16 symbolstr[16];
					int ssl=VPSymbolStringUTF16(vimCurrentSymbol, symbolstr);
					u8=srv->UTF16ToUTF8(symbolstr, ssl);
					buf->append(u8)->update();
				}

				return 1;
			}
		}
		
		if (refresh)
		{
			VPUTF16 string[256];
			int len=vimCandidatelist.updatecontent(string);
			const char *u8=srv->UTF16ToUTF8(string, len);
			textbar->clear()->append(u8)->update();
			return 1;
		}
		
		return 1;
	}
	

	// if no current symbol, process the following keys:
	//		CTRL-OPT-K
	//		CTRL-OPT-,
	//		TO SEE IF CAPSLOCK IS ON
	//		TO SEE IF IT'S AN EMPTY ENTER/SPACE/DELETE...
	if (!vimCurrentSymbol)
	{
	
		if (key->isCtrl() && key->isOpt())
		{
		
			if (inCharCode==',')
			{
			    buf->append((char*)"符")->update(); 
				vimPunctuationWindow=1;
				char msgstr[]="press a key to get punctuation...";
				int i, l=strlen(msgstr);
				VPUTF16 utfstr[256];
				for (i=0; i<l; i++) utfstr[i]=(VPUTF16)msgstr[i];
				const char *u8=srv->UTF16ToUTF8(utfstr, l);
                textbar->clear()->append(u8)->update()->show();

				return 1;		
			}
		}
		
		// sends back all CMD-key, CTRL-key, OPTION-key combinations
		if (key->isCommand() || key->isCtrl() || key->isOpt())
		{
		
			return 0;
		}
				
		// if CAPS LOCK is on
		if (key->isCapslock())
		{
			// if belongs to control keys, don't process it
			if (inCharCode < 0x20) return 0;
			
			char charbuf[2];
			charbuf[1]=0;
			charbuf[0]=inCharCode;
			if (key->isShift()) 
			     charbuf[0]=VPToUpper(inCharCode);
			else
	             charbuf[0]=VPToLower(inCharCode);
            buf->append(charbuf)->send();				
			return 1;
		}

		// "empty" enter/return/space/backspace (Mac Delete)/delete (Mac FN-DELETE)...
		// or EMPTY FUNCTION KEYS (UP, DOWN, ETC.), FOR THAT MATTER!
		if (inCharCode==0x3 || inCharCode==0xd || inCharCode==0x20 || inCharCode==0x8 || inCharCode==0x7f
		    || inCharCode < 0x20)
		{
//            buf->send();
			return 0;		// so it means an EMPTY ENTER/SPACE/DELETE...
		}

		// punctuation marks get processed...
		VPUTF16 pmstr[256];
		int pml=VPFindPunctuationCandidate(inCharCode, pmstr);
		
//		if (pml) debugf ("is punctation, candidates: %d", pml);
		
		if (pml==1)
		{
            const char *u8=srv->UTF16ToUTF8(&pmstr[0], 1);
            buf->append(u8)->send();
			return 1;
		}
		
		if (pml)		// POP UP CANDIDATE WINDOW NOW!
		{
			vimCandidatelist.load(pmstr, pml);
			VPUTF16 utfstr[256];
			int usl=vimCandidatelist.updatecontent(utfstr);
            const char *u8=srv->UTF16ToUTF8(utfstr, usl);
            textbar->clear()->append(u8)->update()->show();
            
			// strange, some applications still processes "candidatable"
			// punctuation marks itself... we have to "eat" these symbols
			u8=srv->UTF16ToUTF8(&pmstr[0], 1);
			buf->append(u8)->update();
			// buf.clear();
			// buf.refreshdisplay();			// works not
			// buf.send();		// we'll see if this works

			vimCandidateWindow=1;
			return 1;
		}
	}
	
	if (vimCurrentSymbol)
	{
		int hit=0;
		
		// this part is written, but we really just want to determine if the TONE key is hit 
		// when there's symbol(s) in...
		
		VPSymbol phoneMark;		
		if (vimKeyboardLayoutEten) phoneMark=VPEtenKeyToSymbol(inCharCode); else phoneMark=VPStandardKeyToSymbol(inCharCode);
		if (phoneMark & vpToneMask)			// tone key is hit!
		{
			hit=1;
			vimCurrentSymbol=VPCombineSymbol(vimCurrentSymbol, phoneMark);
			
			// we wanted to display the current symbol-string (bopomofo string)
			// as user hits SPACE/tone keys, but it's better if we simply
			// display the first candidate...
			
			VPUTF16 symbolstr[16];
			int ssl=VPSymbolStringUTF16(vimCurrentSymbol, symbolstr);
            const char *u8=srv->UTF16ToUTF8(symbolstr, ssl);
			buf->clear()->append(u8)->update();
		}
		
		if (hit || inCharCode==0x3 || inCharCode==0xd || inCharCode==0x20)
		{
			VPUTF16 candi[512];
			int cl=VPFindCandidate(vimCurrentSymbol, candi);
			if (!cl)
			{
//				if (BIMGetWarningSound()) SysBeep(30);
				return 1;
			}

			vimCurrentSymbol=0;		// IF WE DON'T WASH THIS AWAY, THINGS GONNA BE FUNNY...
			if (cl==1)
			{
			     const char *u8=srv->UTF16ToUTF8(&candi[0], 1);
			     buf->clear()->append(u8)->send();
			}
			else
			{
				// replace the current symbol string (bopomofo string) with the first candidate character
				// there is a rationale in doing this, that is, if Vanilla gets deactivated,
				// and if the application requests a "fix," then at least we'll have 
				// something to give out, instead of a funny symbol string...
				 const char *u8=srv->UTF16ToUTF8(&candi[0], 1);
			     buf->clear()->append(u8)->update();
				
				vimCandidateWindow=1;
				vimCandidatelist.load(candi, cl);
				
				VPUTF16 utfstr[256];
				int usl=vimCandidatelist.updatecontent(utfstr);
				
				u8=srv->UTF16ToUTF8(utfstr, usl);
                textbar->clear()->append(u8)->update()->show();
			}
			return 1;
		}
	}

	if (inCharCode==0x8)
	{
		vimCurrentSymbol=VPDeleteSymbolLastPart(vimCurrentSymbol);
		
		if (!vimCurrentSymbol)
		{
            buf->clear()->update();
			return 1;
		}
	}
	else
	{
		VPSymbol newsymbol;
		if (vimKeyboardLayoutEten) newsymbol=VPEtenKeyToSymbol(inCharCode);
		else newsymbol=VPStandardKeyToSymbol(inCharCode);
		
		vimCurrentSymbol=VPCombineSymbol(vimCurrentSymbol, newsymbol);
	}
	
	VPUTF16 symbolstr[16];
	int ssl=VPSymbolStringUTF16(vimCurrentSymbol, symbolstr);

    // fprintf (stderr, "ssl=%d\n", ssl);
	// if still no data, must be a unrecognizable key (e.g. "5" in Eten layout)

	if (vimCurrentSymbol)
	{
	   	// fprintf (stderr, "sending\n");
	   	const char *u8=srv->UTF16ToUTF8(symbolstr, ssl);
		buf->clear()->append(u8)->update();
	}
	else
	{
	   return 0;
/*	    char charbuf[2];
	    charbuf[1]=0;
	    charbuf[0]=inCharCode;
	    buf->append(charbuf)->send(); */
	}
	
	return 1;
}

int CandidateList::appendutf16(VPUTF16 *us, char *cs)
{
	int l=0;
	while (*cs)
	{
		*us++=(VPUTF16)*cs++;
		l++;
	}
	return l;
}
	
CandidateList::CandidateList()
{
	size=currentpage=totalpage=0;
}
	
int CandidateList::load(VPUTF16* str, int len)
{
	size=len;
	currentpage=totalpage=0;
	if (size) totalpage=(size-1)/vimclCandidatePerPage+1;
	for (int i=0; i<len; i++) list[i]=str[i];
	return len;
}
	
int CandidateList::updatecontent(VPUTF16* str)
{
	int from=currentpage*vimclCandidatePerPage, to=from+vimclCandidatePerPage;
	if (to > size) to = size;
		
	int l=0;
	char buf[256];
	int j=1;
	for (int i=from; i<to; i++, j++)
	{
		sprintf (buf, "%d.", j);
		str += appendutf16(str, buf);
		l+=strlen(buf);
		*str++ = list[i];
		l++;
		str += appendutf16(str, " ");
		l++;
	}
		
	sprintf (buf, " (%d/%d)", currentpage+1, totalpage);
	str+=appendutf16(str,buf);
	l+=strlen(buf);
	return l;
}
	
int CandidateList::pageup()
{
	if (!currentpage)
	{
		if (totalpage)  currentpage=totalpage-1;
	}
	else currentpage--;
	return currentpage;
}
	
int CandidateList::pagedown()
{
	currentpage++;
	if (currentpage >= totalpage) currentpage=0;
	return currentpage;
}
	
VPUTF16 CandidateList::select(int candidate)
{
	int i=candidate+currentpage*vimclCandidatePerPage;
	if (i >= size) return 0;
	return list[i];
}



