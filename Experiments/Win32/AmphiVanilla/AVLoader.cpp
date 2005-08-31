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

#include "AVConfig.h"
#include "AVDictionary.h"
#include "AVDisplayServer.h"
#include "AVLoaderUtility.h"
#include "AVService.h"

#include <exception>
using namespace std;

class AVKeyCode : public OVKeyCode  {
public:
    AVKeyCode (int p=0)          { chr=p; shift=capslock=ctrl=alt=0; }
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

class AVBuffer : public OVBuffer {
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

class AVCandidate : public OVCandidate  {
public:
    AVCandidate() : onscreen(0) {}
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

AVService srv;
AVCandidate candi;
AVBuffer buf;
AVDictionary dict;
AVConfig cfg;
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
    dict.setPath(cfg.getBaseDir());
    mod_vector = AVLoadEverything(cfg.getModuleDir(), &srv);
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
		im->initialize(&dict, &srv, cfg.getModuleDir());
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
		for_each(ctx_vector.begin(), ctx_vector.end(), DeleteObject);
		ctx_vector.clear();
		for_each(mod_vector.begin(), mod_vector.end(), DeleteObject);
		mod_vector.clear();
		lt_dlexit();
	}
	int KeyEvent(int n, int c, wchar_t *s) {
		if (!inited) init();

		AVKeyCode kc(c);		
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
		MultiByteToWideChar(CP_UTF8, 0, ac.c_str(), ac.size()+1, s, 1024);
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
