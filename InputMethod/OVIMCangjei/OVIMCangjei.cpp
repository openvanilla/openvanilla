// OVIMCangjei.cpp

#define OVDEBUG

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"
#include "OpenVanilla/OVUtility.h"
#include "vanillaphone.h"
#include "vanillacj.h"

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

class OVCangjeiIM;

class OVCangjeiContext : public OVIMContext
{
public:
	OVCangjeiContext(int simplex)
	{
		vimPunctuationWindow=vimCandidateWindow=0;
		vimInputMethodSimplex=simplex;
	}

	virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
		OVService *srv);

protected:
	int HandlePunctuationWindow(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar);
	int HandleCandidateWindow(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar);
	
	int vimPunctuationWindow;
	int vimCandidateWindow;
	int vimInputMethodSimplex;
	VCCJString vimCJString;
	CandidateList vimCandidatelist;
};

class OVCangjeiIM : public OVInputMethod
{
public:
    OVCangjeiIM(int s=0)
    {
    	simplex=s;
    }
    
    virtual int identifier(char* s)
    {
    	if (!simplex) return strlen(strcpy(s, "OVCangjeiIM"));
        return strlen(strcpy(s, "OVSimplexIM"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        
        if (!simplex)
        {
	        if (!strcasecmp(locale, "zh_TW"))
    	        return strlen(strcpy((char*)s, "OpenVanilla 倉頡輸入法"));
        	else
            	return strlen(strcpy((char*)s, "OpenVanilla Cangjei IM"));
        }
        else
        {
	        if (!strcasecmp(locale, "zh_TW"))
    	        return strlen(strcpy((char*)s, "OpenVanilla 簡易輸入法"));
        	else
            	return strlen(strcpy((char*)s, "OpenVanilla Simplex IM"));
        }        
    }

    virtual int initialize(OVDictionary* g, OVDictionary* l, OVService*, char*)
    {
    	return 1;
    }
    
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
    {
        return 1;
    }

    virtual int update(OVDictionary* g, OVDictionary* l)
    {
    }

    virtual OVIMContext* newContext() 
    {
    	return new OVCangjeiContext(simplex);
    }
    
    virtual void deleteContext(OVIMContext* s) 
    {
        delete s; 
    }
    
protected:
	int simplex;
};


OVLOADABLECANUNLOAD;

// blow are from wrapper code, but we'll do it ourselves, because
// we have TWO IM's here
    extern "C" int OVLoadableAvailableIMCount(char*)
    {
        return 2;
    }
    extern "C" unsigned int OVLoadableVersion()
    {
        return ovVersion;
    }
    extern "C" OVInputMethod* OVLoadableNewIM(int id)
    {
    	if (!id) return new OVCangjeiIM(0);
    	return new OVCangjeiIM(1);
    }
    extern "C" void OVLoadableDeleteIM(OVInputMethod *im)
    {
        delete im;
    }



int OVCangjeiContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
	OVService *srv)
{
    int inCharCode=key->code();

	if (vimPunctuationWindow) return HandlePunctuationWindow(key, buf, textbar);	
	if (vimCandidateWindow) return HandleCandidateWindow(key, buf, textbar);
		
	if (vimCJString.IsEmpty()) 
	{
		if (key->isCtrl() && key->isOpt() && key->code()==',')
		{
			buf->append((char*)"符")->update(); 

			vimPunctuationWindow=1;
			char msgstr[]="press a key to get punctuation...";
			textbar->clear()->append(msgstr)->update()->show();
			return 1;
		}

		if (key->isCommand() || key->isCtrl() || key->isOpt()) return 0;

		// if CAPS LOCK is on
		if (key->isCapslock())
		{
			// if belongs to control keys, don't process it
			if (inCharCode < 0x20) return 0;
			
			char charbuf[2];
			charbuf[1]=0;
			charbuf[0]=inCharCode;
			if (key->isShift()) 
			     charbuf[0]=VPToLower(inCharCode);
			else
	             charbuf[0]=VPToUpper(inCharCode);
            buf->append(charbuf)->send();				
			return 1;
		}
		
		if (inCharCode==0x3 || inCharCode==0xd || inCharCode==0x20 || 
			inCharCode==0x8 || inCharCode==0x7f|| inCharCode < 0x20)
		{
			return 0;		// so it means an EMPTY ENTER/SPACE/DELETE...
		}

		// punctuation marks get processed...
		VPUTF16 pmstr[256];
		int pml=VPFindPunctuationCandidate(inCharCode, pmstr);

		if (pml==1)
		{
            buf->append(&pmstr[0], ovEncodingUTF16BE, 1)->send();
			return 1;
		}
		
		if (pml)		// POP UP CANDIDATE WINDOW NOW!
		{
			vimCandidatelist.load(pmstr, pml);
			VPUTF16 utfstr[256];
			int usl=vimCandidatelist.updatecontent(utfstr);

            textbar->clear()->append(utfstr, ovEncodingUTF16BE, usl)->show();            
			buf->append(&pmstr[0], ovEncodingUTF16BE, 1)->update();
			vimCandidateWindow=1;
			return 1;
		}
	}
	
	if (!vimCJString.IsEmpty())
	{
		if (inCharCode==27)
		{
SIMPLEXCLEAN:
			vimCJString.Clean();
			buf->clear()->update();
			return 1;
		}
		
		if (inCharCode==0x3 || inCharCode==0xd || inCharCode==0x20)
		{
SIMPLEXDIRTY:
			VPUTF16 candi[512];
			
			int cl;
			if (vimInputMethodSimplex)		// added: simplex method!
			{
				cl=vimCJString.SearchCandidateJianyi(candi);
			}
			else
			{
				cl=vimCJString.SearchCandidate(candi);
			}
			
			if (!cl)
			{
//				if (BIMGetWarningSound()) SysBeep(30);
				if (vimInputMethodSimplex) goto SIMPLEXCLEAN;
				return 1;
			}

			vimCJString.Clean();
			if (cl==1)
			{
				buf->clear()->append(&candi[0], ovEncodingUTF16BE, 1)->send();
			}
			else
			{
				buf->clear()->append(&candi[0], ovEncodingUTF16BE, 1)->update();
				
				vimCandidateWindow=1;
				vimCandidatelist.load(candi, cl);
				
				VPUTF16 utfstr[256];
				int usl=vimCandidatelist.updatecontent(utfstr);

                textbar->clear()->append(utfstr, ovEncodingUTF16BE, usl)->
	                update()->show();
			}
			return 1;
		}
	}

	VCCJCode currentchar(inCharCode);
	int valid=currentchar.IsValidCode();

	if (inCharCode==0x8)
	{
		vimCJString.ChopCode();
		valid=1;
			
		if (vimCJString.IsEmpty())
		{
			buf->clear()->update();
			return 1;
		}
	}
	else
	{
		if (key->isShift())
		{
			vimCJString.Clean();
			char cbuf[2];
			cbuf[0]=VPToLower(inCharCode);
			cbuf[1]=0;
			buf->clear()->append(cbuf)->send();
			return 1;
		}
		else
		{
			if (currentchar.IsValidCode()) 
			{
				if (vimInputMethodSimplex && vimCJString.Length()==2)
				{
//					if (BIMGetWarningSound()) SysBeep(30);
					buf->update();
					return 1;
				}
				vimCJString.AppendCode(currentchar);
			}
		}
	}
		
	VPUTF16 symbolstr[16];
	int ssl=vimCJString.ToStringUTF16(symbolstr);

	if (!vimCJString.IsEmpty() && valid)
	{
		buf->clear()->append(symbolstr, ovEncodingUTF16BE, ssl)->update();
		if (vimInputMethodSimplex && vimCJString.Length()==2) goto SIMPLEXDIRTY;
	}
	else
	{
		vimCJString.Clean();
		char cbuf[2];
		cbuf[0]=VPToLower(inCharCode);
		cbuf[1]=0;
		buf->clear()->append(cbuf)->send();
	}
	
	return 1;
}

int OVCangjeiContext::HandlePunctuationWindow(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar)
{
    int inCharCode=key->code();

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
		buf->clear()->append(&uc, ovEncodingUTF16BE, 1)->send();
	}
		
	return 1;
}

int OVCangjeiContext::HandleCandidateWindow(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar)
{
    int inCharCode=key->code();

	int refresh=0;
		
	if (inCharCode==27 || inCharCode==0x8)		// backspace/esc cancles the window
	{
		vimCandidateWindow=0;
		textbar->hide();
		buf->clear()->update();
		return 1;
	}
		
	if (inCharCode==32 || inCharCode==0x1c || inCharCode==0x1f || inCharCode=='>')  	{
		vimCandidatelist.pagedown();
		refresh=1;
	}
		
	if (inCharCode==0x1d || inCharCode==0x1e || inCharCode=='<')   // right/up
	{
		vimCandidatelist.pageup();
		refresh=1;
	}
		
	int refreshacckey=0;
	VCCJCode code(inCharCode);
	
	if ((inCharCode < '1' || inCharCode > '9') && code.IsValidCode())
	{
		vimCJString.AppendCode(code);
		inCharCode='1';
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
			buf->clear()->append(&uc, ovEncodingUTF16BE, 1)->send();
				
			if (refreshacckey)
			{
				VPUTF16 symbolstr[16];
				int ssl=vimCJString.ToStringUTF16(symbolstr);
				buf->clear()->append(&symbolstr, ovEncodingUTF16BE, ssl)->update();
			}

			return 1;
		}
	}
		
	if (refresh)
	{
		VPUTF16 string[256];
		int len=vimCandidatelist.updatecontent(string);
		textbar->clear()->append(string, ovEncodingUTF16BE, len)->update();
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


