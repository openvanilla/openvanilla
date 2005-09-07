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
#include "AVKeyCode.h"
#include "AVBuffer.h"
#include "AVCandidate.h"

#include <exception>
using namespace std;

//MyOVDisplayServer dsvr;

AVService srv;
AVConfig cfg;

AVDisplayServer *dsvr;
AVCandidate *candi;
AVBuffer *buf;
AVDictionary *dict;

std::vector<OVInputMethodContext*> ctx_vector;
int inited=0;
std::vector<OVModule*> mod_vector;
std::vector<bool> startedCtxVector;	// 這是很浪費的作法 orz

bool sort_im(OVModule *a, OVModule *b)
{
	int pa = 0, pb = 0;
	dict->setDict(a->identifier());
	pa = dict->getInteger("priority");
	dict->setDict(b->identifier());
	pb = dict->getInteger("priority");
	return (pa >= pb);
}

void init() {    
    if (inited) return;
    candi = new AVCandidate(dsvr);
    buf = new AVBuffer(dsvr);
    dict = AVDictionary::getDict(cfg.getBaseDir());
    // dict->setPath(cfg.getBaseDir());
    // dict.setAutoFlush(true);
    vector<OVModule*> tmpmod_vector = AVLoadEverything(cfg.getModuleDir(), &srv);
    // delete unused im
    vector<OVModule*>::iterator m;
    for(m = tmpmod_vector.begin(); m != tmpmod_vector.end(); m++) {
	    dict->setDict((*m)->identifier());
	    if(!dict->keyExist("enable")) {
		    dict->setInteger("enable", 1);
		    mod_vector.push_back(*m);
	    } else {
		    if(dict->getInteger("enable")) {
			    mod_vector.push_back(*m);
		    }
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
		dict->setDict(im->identifier());
		im->initialize(dict, &srv, cfg.getModuleDir());
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
		delete dict;
		delete dsvr;
		lt_dlexit();
	}
	void ReloadConfig() {
		dict->update();
		for(int i = 0; i < mod_vector.size(); i++) {
			if(ctx_vector.at(i) != NULL) {
				murmur("Reload: %s", mod_vector.at(i)->identifier());
				dict->setDict(mod_vector.at(i)->identifier());
				mod_vector.at(i)->update(dict, &srv);
			}
		}
	}
	void SetDisplayServer(AVDisplayServer *svr) {
		dsvr = svr;
	}
	int KeyEvent(int n, int c) {
		if (!inited) init();

		AVKeyCode kc(c);		
		//if( n > ctx_vector.size() - 1) n = ctx_vector.size() - 1;
		int ctxVectorNum = static_cast<int>(ctx_vector.size()) - 1;
		if(n > ctxVectorNum) return 0;
		if(ctx_vector.at(n) == NULL)
			initContext(n);
		int st = 1;
		if(ctx_vector[n] && !startedCtxVector[n]) {
			ctx_vector[n]->start(buf, candi, &srv);
			startedCtxVector[n] = true;
		}
		try {
			st =ctx_vector[n]->keyEvent(&kc, buf, candi, &srv);
		}
		catch (...) {}
		return st;
	}

	int ModuleName(int i, char *str)
	{
		string s;
		int modVectorNum = static_cast<int>(mod_vector.size()) - 1;
		if(i > modVectorNum) {
			strcpy(str, "");
			return 0;
		}
		s = mod_vector.at(i)->localizedName("zh_TW");
		strcpy(str, s.c_str());
		return 1;
	}
}
