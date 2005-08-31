#ifdef _UNICODE
	#undef _UNICODE
#endif
#ifdef UNICODE
	#undef UNICODE
#endif
#ifdef WIN32
#define PATH_MAX MAX_PATH
#endif

#include <string>
#include <sys/types.h>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <map>
#include "OpenVanilla.h"
#include "OVUtility.h"
#include "OVLibrary.h"
#include "OVCandidateList.h"
#include "iconv.h"

#include "AVDictionary.h"
#include "AVDisplayServer.h"
#include "AVLoaderUtility.h"

#include <exception>
using namespace std;

char OV_BASEDIR[PATH_MAX];
char OV_USERDIR[PATH_MAX];
char OV_MODULEDIR[PATH_MAX];

class DummyKeyCode : public OVKeyCode  {
public:
    DummyKeyCode (int p=0)          { chr=p; shift=capslock=ctrl=alt=0; }
    virtual int code()              { return chr & 0x00FF; }
    virtual int isShift()           { return chr & 0x0100; }
    virtual int isCapslock()        { return chr & 0x0400; }
    virtual int isCtrl()            { return ctrl; }
    virtual int isAlt()             { return alt; }
    
    virtual void setCode(int x)     { chr=x; }
    virtual void setShift(int x)    { shift=x; }
    virtual void setCapslock(int x) { capslock=x; }
    virtual void setCtrl(int x)     { ctrl=x; }
    virtual void setAlt(int x)      { alt=x; }
protected:
    int chr;
    int shift, capslock, ctrl, alt;
};

MyOVDisplayServer dsvr;

class DummyBuffer : public OVBuffer {
public:
    virtual OVBuffer* clear() { 
        action += "bufclear ";
        bufstr="";
		dsvr.showBuf(false);
        return this;
    }
    virtual OVBuffer* append(const char *s) {
        bufstr+=s;
        return this;
    }
    virtual OVBuffer* send() { 
        //dumpu8string(bufstr.c_str());
	if(bufstr!="") {
		action += "bufsend "; 
		action += bufstr;
		action += " ";
		bufstr="";
		dsvr.showBuf(false);
	}
        return this;
    }
    virtual OVBuffer* update() { 
	if(bufstr!="") {
		char tmp[100];
		//dsvr.setBufString(utf8toutf16(bufstr.c_str()))->setCursorPos(cursorPos)->setMarkFrom(markFrom)->setMarkTo(markTo)->notify();
		action += "bufupdate "; 
		action += bufstr;
		action += " cursorpos ";
		sprintf(tmp, "%d", cursorPos);
		action += string(tmp);
		action += " markfrom ";
		sprintf(tmp, "%d", markFrom);
		action += string(tmp);
		action += " markto ";
		sprintf(tmp, "%d", markTo);
		action += string(tmp);
		action += " ";
		
	}
        return this;
    }
    virtual OVBuffer* update(int cursorPos, int markFrom=-1, int markTo=-1) {
	    this->cursorPos = cursorPos;
	    this->markFrom = markFrom;
	    this->markTo = markTo;
        return update();
    }
    virtual int isEmpty() {
        if (!bufstr.length()) return 1; return 0;
    }

    string action;
    string bufstr;
    int cursorPos;
    int markFrom;
    int markTo;
};

class DummyCandidate : public OVCandidate  {
public:
    DummyCandidate() : onscreen(0) {}
    virtual OVCandidate* clear() {
        candistr="";
        action += "candiclear "; return this;
		dsvr.showCandi(false);
    }
    virtual OVCandidate* append(const char *s) {
        candistr+=s;
        return this;
    }
    virtual OVCandidate* hide() {
        if (onscreen) { onscreen=0; action += "candihide "; dsvr.showCandi(false);}
        return this;
    }
    virtual OVCandidate* show() {
        if (!onscreen) { onscreen=1; action += "candishow "; dsvr.showCandi(true)->notify();}
        return this;
    }
    virtual OVCandidate* update() {
        action += "candiupdate "; 
        action += candistr;
        action += " candiend ";
		//dsvr.setCandiString(utf8toutf16(candistr.c_str()));
        return this; 
    }
    virtual int onScreen() {
        return onscreen;
    }

    string action;
    string candistr;
protected:
    int onscreen;
};

class DummyService : public OVService {
public:
    virtual void beep() {}
    virtual void notify(const char *msg) { fprintf(stderr, "%s\n", msg); }
    virtual const char *locale() { return "zh_TW"; }
    virtual const char *userSpacePath(const char *modid) { return OV_USERDIR; }
    virtual const char *pathSeparator() { return "\\"; }
    virtual const char *toUTF8(const char *encoding, const char *src) 
    { 
	    char *out = NULL;
	    size_t inbytesleft = strlen(src) + 1;
	    size_t outbytesleft = 1024;
	    iconv_t cd;
	    memset(internal, 0, 1024);
	    out = internal;
	    cd = iconv_open("UTF-8", encoding);
	    iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	    iconv_close(cd);
	    return internal;
    }
    virtual const char *fromUTF8(const char *encoding, const char *src)
    {
	    char *out = NULL;
	    size_t inbytesleft = strlen(src) + 1;
	    size_t outbytesleft = 1024;
	    iconv_t cd;
	    memset(internal, 0, 1024);
	    out = internal;
	    cd = iconv_open(encoding, "UTF-8");
	    iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	    iconv_close(cd);
	    return internal;
    }
    /* copy from OVPhoneticLib.cpp: VPUTF16ToUTF8 */
    virtual const char *UTF16ToUTF8(unsigned short *s, int l)
    {
	    char *b = internal;
	    for (int i=0; i<l; i++)
	    {
		    if (s[i] < 0x80)
		    {
			    *b++=s[i];
		    }
		    else if (s[i] < 0x800)
		    {
			    *b++=(0xc0 | s[i]>>6);
			    *b++=(0x80 | s[i] & 0x3f);
		    }
		    else if (s[i] < 0xd800 || s[i] > 0xdbff)
		    {
			    *b++ = (0xe0 | s[i]>>12);
			    *b++ = (0x80 | s[i]>>6 & 0x3f);
			    *b++ = (0x80 | s[i] & 0x3f);

		    }
		    else
		    {
			    unsigned int offset= 0x10000 - (0xd800 << 10) - 0xdc00;
			    unsigned int codepoint=(s[i] << 10) + s[i+1]+offset;
			    i++;
			    *b++=(0xf0 | codepoint>>18);
			    *b++=(0x80 | codepoint>>12 & 0x3f);
			    *b++=(0x80 | codepoint>>6 & 0x3f);
			    *b++=(0x80 | codepoint & 0x3F);
		    }
	    }

	    *b=0;
	    return internal;
    }
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr) {
        return 0;
    }
private:
    char internal[1024];
};

DummyService srv;
DummyCandidate candi;
DummyBuffer buf;
AVDictionary dict;
std::vector<OVInputMethodContext*> ctx_vector;
int inited=0;
std::vector<OVModule*> mod_vector;
std::vector<bool> startedCtxVector;	// 這是很浪費的作法 orz

bool sort_im(OVModule *a, OVModule *b)
{
	int pa = 0, pb = 0;
	dict.setDict(a->identifier());
	pa = dict.getInteger("priority");
	dict.setDict(b->identifier());
	pb = dict.getInteger("priority");
	return (pa >= pb);
}

void init() {    
    if (inited) return;
    GetWindowsDirectory(OV_BASEDIR, MAX_PATH - 14);
    sprintf(OV_BASEDIR, "%s\\%s", OV_BASEDIR, "\\OpenVanilla\\");
    sprintf(OV_USERDIR, "%s\\%s", OV_BASEDIR, "\\User\\");
    sprintf(OV_MODULEDIR, "%s\\%s", OV_BASEDIR, "\\Modules\\");
    dict.setPath(OV_BASEDIR);
    mod_vector = AVLoadEverything(OV_MODULEDIR, &srv);
    // delete unused im
    vector<OVModule*>::iterator m;
    for(m = mod_vector.begin(); m != mod_vector.end(); m++) {
	    dict.setDict((*m)->identifier());
	    if(!dict.keyExist("enable")) {
		    dict.setInteger("enable", 1);
	    } else {
		    if(!dict.getInteger("enable"))
			    mod_vector.erase(m);
	    }
    }
    sort(mod_vector.begin(), mod_vector.end(), sort_im);
    int size = mod_vector.size();
    ctx_vector.assign(size, static_cast<OVInputMethodContext*>(NULL));
    startedCtxVector.assign(size, false);
    fprintf(stderr, "INIT\n");
    /*
	vector<OVModule*>::iterator iter;
	for(iter = mod_vector.begin(); iter != mod_vector.end(); ++iter) {
		if(!strcmp((*iter)->moduleType(), "OVInputMethod"))
		{
			OVInputMethod *im = reinterpret_cast<OVInputMethod*>((*iter));
			im->initialize(new DummyDictionary(OV_MODULEDIR, im->identifier()), &srv, OV_MODULEDIR);
			OVInputMethodContext* ctx;
			ctx = im->newContext();
			ctx_vector.push_back(ctx);
		}
		else
		{
			// 以後可以不用浪費空間擺不是 OVInputMethod 的東西，
			// 取而代之的是加入 OVOutputFilter 的處理。
			ctx_vector.push_back(static_cast<OVInputMethodContext*>(NULL));
		}

		startedCtxVector.push_back(false);
	}
    */
	
    inited=1;
}

void initContext(int n) {
	if(!strcmp(mod_vector[n]->moduleType(), "OVInputMethod"))
	{
		OVInputMethod *im = reinterpret_cast<OVInputMethod*>(mod_vector[n]);
		dict.setDict(im->identifier());
		im->initialize(&dict, &srv, OV_MODULEDIR);
		murmur("InitContext %s", im->localizedName("zh_TW"));
		ctx_vector.at(n) = im->newContext();
	}
}

struct DeleteObject {
	template<typename T>
		void operator()(const T* ptr) const
		{
			if(ptr != static_cast<T*>(NULL))
				delete ptr;
		}
};

extern "C" {
	void InitLoader() {
		if (!inited) init();
	}
	void ShutdownLoader() {
		//for_each(ctx_vector.begin(), ctx_vector.end(), DeleteObject);
		//ctx_vector.clear();
		//for_each(mod_vector.begin(), mod_vector.end(), DeleteObject);
		//mod_vector.clear();
	}
	int KeyEvent(int n, int c, wchar_t *s) {
		if (!inited) init();

		DummyKeyCode kc(c);		
		//if( n > ctx_vector.size() - 1) n = ctx_vector.size() - 1;
		int ctxVectorNum = static_cast<int>(ctx_vector.size()) - 1;
		if(n > ctxVectorNum) return 0;
		if(ctx_vector.at(n) == NULL)
			initContext(n);
		int st = 1;
		if(ctx_vector[n] && !startedCtxVector[n]) {
			ctx_vector[n]->start(&buf, &candi, &srv);
			startedCtxVector[n] = true;
		}
		try {
			st =ctx_vector[n]->keyEvent(&kc, &buf, &candi, &srv);
		}
		catch (...) {}
		string ac=candi.action+buf.action;
		if (st) ac += "processed"; else ac+="unprocessed";
#ifdef WIN32
		MultiByteToWideChar(CP_UTF8, 0, ac.c_str(), ac.size(), s, 1024);
#else	    
		iconv_t cd;
	    char *out = (char*)s;
	    cd = iconv_open("WCHAR_T", "UTF-8");
		const char *src = ac.c_str();
		size_t inbytesleft = ac.size();
		size_t outbytesleft = 1024;
	    iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	    iconv_close(cd);
#endif
		candi.action="";
		buf.action="";
		return static_cast<int>(ac.length());
	}

	int ModuleName(int i, wchar_t *str)
	{
		string s;
		int modVectorNum = static_cast<int>(mod_vector.size()) - 1;
		if(i > modVectorNum) {
			wcscpy(str, L"");
			return 0;
		}
		s = mod_vector.at(i)->localizedName("zh_TW");
#ifdef WIN32
		// s.size()+1 is used to include the NULL character at the 
		// end of the string.
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), s.size()+1, str, 1024);
#else
		iconv_t cd;
	    char *out = (char*)str;
	    cd = iconv_open("WCHAR_T", "UTF-8");
		const char *src = s.c_str();
		size_t inbytesleft = s.size()+1;
		size_t outbytesleft = 1024;
	    iconv (cd, &src, &inbytesleft, &out, &outbytesleft);
	    iconv_close(cd);
#endif
		return 1;
	}
}

#ifndef _UNICODE
	#define _UNICODE
#endif
#ifndef UNICODE
	#define UNICODE
#endif
