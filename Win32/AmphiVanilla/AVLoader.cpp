#include "AVLoader.h"
#include <exception>
#include <algorithm>
#include <functional>
#include <string>
using namespace std;

AVLoader* AVLoader::globalLoader = 0;

AVLoader* AVLoader::getLoader()
{
	if(!globalLoader)
		globalLoader = new AVLoader();
	return globalLoader;
}

void AVLoader::shutdown()
{
	if(globalLoader)
		delete globalLoader;
	globalLoader = 0;
}

AVLoader::AVLoader() : dsvr(0)
{
	em = new AVEmbeddedLoader();

	vector<OVModule*> tmpmod_vector;
	vector<OVModule*>::iterator m;
	for(m = em->modlist().begin(); m != em->modlist().end(); m++) {
		em->dict()->setDict((*m)->identifier());
		if(!strcmp((*m)->moduleType(), "OVInputMethod")) {
			if(!em->dict()->keyExist("enable")) {
				em->dict()->setInteger("enable", 1);
				tmpmod_vector.push_back(*m);
			} else {
				if(em->dict()->getInteger("enable")) {
					tmpmod_vector.push_back(*m);
				}
			}
		} else if(!strcmp((*m)->moduleType(), "OVOutputFilter")) {
			if(em->dict()->getInteger("enable")) {
				OVOutputFilter *of = dynamic_cast<OVOutputFilter*>(*m);
				murmur("load output filter %s", of->identifier());
				of->initialize(em->dict(), em->srv(), em->cfg()->getModuleDir());
				ovof_vector.push_back(of);
			} else {
				em->dict()->setInteger("enable", 0);
			}
		}
	}
	em->modlist() = tmpmod_vector; // Orz
	int size = em->modlist().size();
	ctx_vector.assign(size, static_cast<OVInputMethodContext*>(NULL));
	startedCtxVector.assign(size, false);

	candi = new AVCandidate();
	buf = new AVBuffer(&ovof_vector, em->srv());
}

AVLoader::~AVLoader()
{
	/* FIXME: uncomment this will cause segfault in ime.
	for(int i = 0; i < ctx_vector.size(); i++)
		if(ctx_vector[i] != NULL)
			delete ctx_vector[i];
	*/
	delete em;
	delete candi;
	delete buf;
}

void AVLoader::initContext(int i)
{
	if(!strcmp(em->modlist()[i]->moduleType(), "OVInputMethod"))
	{
		OVInputMethod *im = reinterpret_cast<OVInputMethod*>(em->modlist()[i]);
		em->dict()->setDict(im->identifier());
		im->initialize(em->dict(), em->srv(), em->cfg()->getModuleDir());
		murmur("InitContext %s", im->localizedName("zh_TW"));
		ctx_vector.at(i) = im->newContext();
	}
}

void AVLoader::reloadConfig()
{
	em->dict()->update();
	for(int i = 0; i < em->modlist().size(); i++) {
		if(ctx_vector.at(i) != NULL) {
			murmur("Reload: %s", em->modlist().at(i)->identifier());
			em->dict()->setDict(em->modlist().at(i)->identifier());
			em->modlist().at(i)->update(em->dict(), em->srv());
		}
	}
}

void AVLoader::connectDisplayServer(AVDisplayServer *svr)
{
	dsvr = svr;
	candi->setDisplayServer(dsvr);
	buf->setDisplayServer(dsvr);
	em->srv()->setDisplayServer(dsvr);
}

bool AVLoader::keyEvent(int n, AVKeyCode c)
{
	//if( n > ctx_vector.size() - 1) n = ctx_vector.size() - 1;
	int ctxVectorNum = static_cast<int>(ctx_vector.size()) - 1;
	if(n > ctxVectorNum) return 0;
	if(ctx_vector.at(n) == NULL)
		initContext(n);
	bool st = true;
	if(ctx_vector[n] && !startedCtxVector[n]) {
		ctx_vector[n]->start(buf, candi, em->srv());
		startedCtxVector[n] = true;
	}
	try {
		dsvr->hideNotify();
		if(!ctx_vector[n]->keyEvent(&c, buf, candi, em->srv()))
			st = false;
	}
	catch (...) {}
	return st;
}

bool AVLoader::moduleName(int i, char *str)
{
	string s;
	int modVectorNum = static_cast<int>(em->modlist().size()) - 1;
	if(i > modVectorNum) {
		strcpy(str, "");
		return false;
	}
	s = em->modlist().at(i)->localizedName("zh_TW");
	strcpy(str, s.c_str());
	return true;
}
