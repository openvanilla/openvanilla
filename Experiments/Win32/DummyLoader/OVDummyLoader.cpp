#include <string>
#include <sys/types.h>
#include <windows.h>
#include <vector>
#include "OpenVanilla.h"
#include "OVUtility.h"
#include "OVLibrary.h"
#include "OVCandidateList.h"
extern "C" {
#include "ltdl.h"
}

#define OV_MODULEDIR "C:\\OpenVanilla\\"
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
    virtual int code()              { return chr; }
    virtual int isShift()           { return shift; }
    virtual int isCapslock()        { return capslock; }
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

class DummyBuffer : public OVBuffer {
public:
    virtual OVBuffer* clear() { 
        action += "bufclear ";
        bufstr="";
        return this;
    }
    virtual OVBuffer* append(const char *s) {
        bufstr+=s;
        return this;
    }
    virtual OVBuffer* send() { 
//        dumpu8string(bufstr.c_str());
        action += "bufsend "; 
        action += utf8toutf16(bufstr.c_str());
        action += " ";
        bufstr="";
        return this;
    }
    virtual OVBuffer* update() { 
        action += "bufupdate "; 
        action += utf8toutf16(bufstr.c_str());
        action += " ";
        return this;
    }
    virtual OVBuffer* update(int cursorPos, int markFrom=-1, int markTo=-1) {
        return update();
    }
    virtual int isEmpty() {
        if (!bufstr.length()) return 1; return 0;
    }

    string action;
    string bufstr;
};

class DummyCandidate : public OVCandidate  {
public:
    DummyCandidate() : onscreen(0) {}
    virtual OVCandidate* clear() {
        candistr="";
        action += "candiclear "; return this;
    }
    virtual OVCandidate* append(const char *s) {
        candistr+=s;
        return this;
    }
    virtual OVCandidate* hide() {
        if (onscreen) { onscreen=0; action += "candihide "; }
        return this;
    }
    virtual OVCandidate* show() {
        if (!onscreen) { onscreen=1; action += "candishow "; }
        return this;
    }
    virtual OVCandidate* update() {
        action += "candiupdate "; 
        action += utf8toutf16(candistr.c_str());
        action += " ";
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

class DummyDictionary : public OVDictionary {
public:
    virtual int keyExist(const char *key) { return 0; }
    virtual int getInteger(const char *key) { return 0; }
    virtual int setInteger(const char *key, int value) { return value; }
    virtual const char* getString(const char *key) { return "123456789"; }
    virtual const char* setString(const char *key, const char *value) {
        return value;
    }
};

class DummyService : public OVService {
public:
    virtual void beep() {}
    virtual void notify(const char *msg) { fprintf(stderr, "%s\n", msg); }
    virtual const char *locale() { return "zh_TW"; }
    virtual const char *userSpacePath(const char *modid) { return "/tmp"; }
    virtual const char *pathSeparator() { return "/"; }
    virtual const char *toUTF8(const char *encoding, const char *src) 
        { return src; }
    virtual const char *fromUTF8(const char *encoding, const char *src)
        { return src; }
    virtual const char *UTF16ToUTF8(unsigned short *src, int len) {
        return "blahblah";
    }
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr) {
        return 0;
    }
};

DummyService srv;
DummyCandidate candi;
DummyBuffer buf;
DummyDictionary dict;
OVInputMethodContext *ctx;
int inited=0;
std::vector<OVModule*> mod_vector;

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
			if(strstr(FileData.cFileName, ".dll") ||
				       	strstr(FileData.cFileName, ".DLL"))
			{
				fprintf(stderr,  "Load OV module: %s\n", FileData.cFileName);
				OVLibrary* mod = open_module((path + FileData.cFileName).c_str());
				if(mod){
					OVModule* m;
					mod->initLibrary(&srv, OV_MODULEDIR);
					for(int i=0; m = mod->getModule(i); i++)
						mod_vector.push_back(m);
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
		
   return mod_vector.size();
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

    lt_dlinit();
    lt_dlsetsearchpath(OV_MODULEDIR);

    scan_ov_modules();
    OVInputMethod *im = dynamic_cast<OVInputMethod*>(mod_vector[0]);
    im->initialize(&dict, &srv, OV_MODULEDIR);
    fprintf(stderr, "INIT\n");
    ctx = im->newContext();
    inited=1;
}

void exit() {
	lt_dlexit();
}

extern "C" {
	int  keyevent(int c, char *s) {
		if (!inited) init();

		DummyKeyCode kc(c);
		int st=ctx->keyEvent(&kc, &buf, &candi, &srv);

		string ac=candi.action+buf.action;
		if (st) ac += "processed"; else ac+="unprocessed";
		strcpy(s, ac.c_str());
		candi.action="";
		buf.action="";
		return ac.length();
	}
}
