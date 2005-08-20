#ifdef _UNICODE
	#undef _UNICODE
#endif
#ifdef UNICODE
	#undef UNICODE
#endif

#include <string>
#include <sys/types.h>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "OpenVanilla.h"
#include "OVUtility.h"
#include "OVLibrary.h"
#include "OVCandidateList.h"
#include "iconv.h"

#include "DOVDictionary.h"
#include "OVDisplayServer.h"

#include <exception>

extern "C" {
#include "ltdl.h"
}

TCHAR OV_BASEDIR[MAX_PATH];
TCHAR OV_USERDIR[MAX_PATH];
TCHAR OV_MODULEDIR[MAX_PATH];
enum { bit7=0x80, bit6=0x40, bit5=0x20, bit4=0x10, bit3=8, bit2=4, bit1=2, bit0=1 };

const char* hexstr(unsigned char x) {
    static char buf[4];
    sprintf(buf, "%02x", (unsigned short)x);
    return buf;
}

const string &utf8toutf16(const char* src)
{
    char *s1=(char*)src;
    static string s;
    s="";
	int len=0;
    char a, b, c;
    while (*s1)
    {
        a=*s1++;
        if ((a & (bit7|bit6|bit5))==(bit7|bit6)) { // 0x000080-0x0007ff
            b=*s1++;
            
            s += hexstr((a & (bit4|bit3|bit2)) >> 2);
            s += hexstr(((a & (bit1|bit0)) << 6) | (b & (bit5|bit4|bit3|bit2|bit1|bit0)));
        }
        else if ((a & (bit7|bit6|bit5|bit4))==(bit7|bit6|bit5)) // 0x000800-0x00ffff
        {
		    b=*s1++;
			c=*s1++;
			
            s += hexstr (((a & (bit3|bit2|bit1|bit0)) << 4) | ((b & (bit5|bit4|bit3|bit2)) >> 2));
            s += hexstr (((b & (bit1|bit0)) << 6) | (c & (bit5|bit4|bit3|bit2|bit1|bit0)));
		}
		else 
		{
            s+=hexstr(0);
            s+=hexstr(a);
		}
    }
	return (s);
}

void dumpu8string(const char *s) {
    string us=utf8toutf16((char*)s);
    printf("u8str=%s, dump=%s\n", s, us.c_str());
}


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
		action += utf8toutf16(bufstr.c_str());
		action += " ";
		bufstr="";
		dsvr.showBuf(false);
	}
        return this;
    }
    virtual OVBuffer* update() { 
	if(bufstr!="") {
		char tmp[100];
		wchar_t *decoded;
		action += "bufupdate "; 
		action += utf8toutf16(bufstr.c_str());
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
		dsvr.setBufString(utf8toutf16(bufstr.c_str()))->setCursorPos(cursorPos)->setMarkFrom(markFrom)->setMarkTo(markTo)->notify();
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
        action += utf8toutf16(candistr.c_str());
        action += " ";
		dsvr.setCandiString(utf8toutf16(candistr.c_str()));
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
std::vector<OVInputMethodContext*> ctx_vector;
int inited=0;
std::vector<OVModule*> mod_vector;
std::vector<bool> startedCtxVector;	// 這是很浪費的作法 orz

typedef OVModule* (*TypeGetModule)(int);
typedef int (*TypeInitLibrary)(OVService*, const char*);
typedef unsigned int (*TypeGetLibVersion)();
struct OVLibrary {
   lt_dlhandle handle;
   TypeGetModule getModule;
   TypeInitLibrary initLibrary;
   TypeGetLibVersion getLibVersion;
};

static OVLibrary* open_module(const char* modname){
   OVLibrary* mod = new OVLibrary();
   mod->handle = lt_dlopen(modname);
   if(mod->handle == NULL){
      fprintf(stderr, "dlopen %s failed\n", modname);
      goto OPEN_FAILED;
   }
   mod->getModule = (TypeGetModule)lt_dlsym( mod->handle, 
                                             "OVGetModuleFromLibrary" );
   mod->getLibVersion = (TypeGetLibVersion)lt_dlsym( mod->handle, 
                                             "OVGetLibraryVersion" );
   mod->initLibrary = (TypeInitLibrary)lt_dlsym( mod->handle,
                                             "OVInitializeLibrary" );
   if( !mod->getModule || !mod->getLibVersion || !mod->initLibrary ){
      fprintf(stderr, "dlsym %s failed\n", modname);
      goto OPEN_FAILED;
   }
   if( mod->getLibVersion() < OV_VERSION ){
      fprintf(stderr, "%s %d is too old\n", modname, mod->getLibVersion());
      goto OPEN_FAILED;
   }
   return mod;

OPEN_FAILED:
   delete mod;
   return NULL;
}

static int scan_ov_modules(){
	BOOL fFinished;
	HANDLE hList;
	WIN32_FIND_DATA FileData;
	DummyService srv;

	string path = OV_MODULEDIR;
	hList = FindFirstFile((path + "*").c_str(), &FileData);
	if(hList == INVALID_HANDLE_VALUE)
	{
		printf("No files found\n");
	}
	else
	{
		fFinished = FALSE;
		while (!fFinished)
		{
			if(strstr(FileData.cFileName, ".dll") || strstr(FileData.cFileName, ".DLL"))
			{
				fprintf(stderr,  "Load OV module: %s\n", FileData.cFileName);
				OVLibrary* mod = open_module((path + FileData.cFileName).c_str());
				if(mod){
					OVModule* m;
					mod->initLibrary(&srv, OV_MODULEDIR);
					for(int i=0; m = mod->getModule(i); i++)
					{	
						if(!strcmp(m->moduleType(), "OVDisplayComponent"))
						{
							OVDisplayComponent *dc = reinterpret_cast<OVDisplayComponent*>(m);
							dc->regDisplayServer(&dsvr);
							murmur("InitDisplayComponent: %s", dc->localizedName("zh_TW"));
							continue;
						}
						mod_vector.push_back(m);
						fprintf(stderr, "Load OVModule: %s\n", m->localizedName("zh_TW"));

					}
					delete mod;
				}
			}
			if (!FindNextFile(hList, &FileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					fFinished = TRUE;
				}
			}
		}
	}
	FindClose(hList);
		
   return static_cast<int>(mod_vector.size());
}
/*
int main() {
    DummyService srv;
    DummyCandidate candi;
    DummyBuffer buf;
    DummyDictionary dict;

    OVIMPOJHolo pojholo;
    pojholo.initialize(&dict, &srv, "c:\\OVVBPOJ\\");
    OVInputMethodContext *im=pojholo.newContext();
    
    while (!feof(stdin)) {
        char c=getchar();
        if (c==10) c=13;
        DummyKeyCode kc(c);

        int st=im->keyEvent(&kc, &buf, &candi, &srv);
        
        printf ("key=%d status=%d action='%s'\n",
            c, st, (candi.action + buf.action).c_str());
        buf.action="";
        candi.action="";
    }
}
*/

void init() {    
    if (inited) return;
    GetWindowsDirectory(OV_BASEDIR, MAX_PATH - 14);
    sprintf(OV_BASEDIR, "%s\\%s", OV_BASEDIR, "\\OpenVanilla\\");
    sprintf(OV_USERDIR, "%s\\%s", OV_BASEDIR, "\\User\\");
    sprintf(OV_MODULEDIR, "%s\\%s", OV_BASEDIR, "\\Modules\\");
    lt_dlinit();
    lt_dlsetsearchpath(OV_MODULEDIR);

    int size = scan_ov_modules();
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
/*
    OVInputMethod *im = reinterpret_cast<OVInputMethod*>(mod_vector[n]);
    im->initialize(&dict, &srv, OV_MODULEDIR);
    murmur("InitContext %s", im->localizedName("zh_TW"));
    ctx_vector.at(n) = im->newContext();
*/
	if(!strcmp(mod_vector[n]->moduleType(), "OVInputMethod"))
	{
		OVInputMethod *im = reinterpret_cast<OVInputMethod*>(mod_vector[n]);
		im->initialize(new DummyDictionary(OV_BASEDIR, im->identifier()), &srv, OV_MODULEDIR);
		murmur("InitContext %s", im->localizedName("zh_TW"));
		ctx_vector.at(n) = im->newContext();
	}
}

void exit() {
	lt_dlexit();
}

struct DeleteObject {
	template<typename T>
		void operator()(const T* ptr) const
		{
			if(ptr != static_cast<T*>(NULL))
				delete ptr;
		}
};
/*
template<typename T>
void DeleteObject(const T* ptr)
{
	if(ptr != static_cast<T*>(NULL))
		delete ptr;
}
*/

extern "C" {
	void InitLoader() {
		if (!inited) init();
	}
	void ShutdownLoader() {
//		for_each(ctx_vector.begin(), ctx_vector.end(), DeleteObject);
//		ctx_vector.clear();
//		for_each(mod_vector.begin(), mod_vector.end(), DeleteObject);
//		mod_vector.clear();
	}
	int KeyEvent(int n, int c, char *s) {
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
		strcpy(s, ac.c_str());
		candi.action="";
		buf.action="";
		return static_cast<int>(ac.length());
	}

	int ModuleName(int i, char *str)
	{
		string s;
		int modVectorNum = static_cast<int>(mod_vector.size()) - 1;
		if(i > modVectorNum) {
			strcpy(str, "");
			return 0;
		}
		s = utf8toutf16(mod_vector.at(i)->localizedName("zh_TW"));
		strcpy(str, s.c_str());
		return 1;
	}
}

#ifndef _UNICODE
	#define _UNICODE
#endif
#ifndef UNICODE
	#define UNICODE
#endif
