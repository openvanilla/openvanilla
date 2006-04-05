#include ".\OVIME2Instance.h"
#include "OVIME2Server.h"

#include <exception>
#include <algorithm>
#include <functional>
#include <string>

using namespace std;

OVIME2Instance::OVIME2Instance(OVIME2Server* server)
	: svr(server), cursorPos(0), markFrom(0), markTo(0)
{
	int n_context = server->modlist().size();
	ctx_vector.assign(n_context, static_cast<OVInputMethodContext*>(NULL));
	startedCtxVector.assign(n_context, false);
	activatedIm = 0;

	candi = new AVCandidate();
	buf = new AVBuffer(&server->filters(), server->srv());

	candi->setDisplayServer(this);
	buf->setDisplayServer(this);
	svr->srv()->setDisplayServer(this);
}

OVIME2Instance::~OVIME2Instance(void)
{
	delete candi;
	delete buf;
}

void OVIME2Instance::initContext(int i)
{
	if(!strcmp(svr->modlist()[i]->moduleType(), "OVInputMethod"))
	{
		OVInputMethod *im = reinterpret_cast<OVInputMethod*>(svr->modlist()[i]);
		svr->dict()->setDict(im->identifier());
		im->initialize(svr->dict(), svr->srv(), svr->cfg()->getModuleDir());
		murmur("InitContext %s", im->localizedName("zh_TW"));
		ctx_vector.at(i) = im->newContext();
	}
}

void OVIME2Instance::reloadConfig()
{
	svr->dict()->update();
	for(size_t i = 0; i < svr->modlist().size(); i++) {
		if(ctx_vector.at(i) != NULL) {
			murmur("Reload: %s", svr->modlist().at(i)->identifier());
			svr->dict()->setDict(svr->modlist().at(i)->identifier());
			svr->modlist().at(i)->update(svr->dict(), svr->srv());
		}
	}
}


bool OVIME2Instance::keyEvent( AVKeyCode c)
{
	int n = activatedIm;
	//if( n > ctx_vector.size() - 1) n = ctx_vector.size() - 1;
	int ctxVectorNum = static_cast<int>(ctx_vector.size()) - 1;
	if( n > ctxVectorNum ) return 0;
	if( ctx_vector.at(n) == NULL )
		initContext(n);
	bool st = true;
	if( ctx_vector[n] && !startedCtxVector[n] ) {
		ctx_vector[n]->start(buf, candi, svr->srv());
		startedCtxVector[n] = true;
	}
	try {
//		These lines are used to debug
//		char name[100];
//		svr->moduleName(0, name);
//		const char* name2 = svr->srv()->fromUTF8( "big5", name );
//		cout << name2;

		this->hideNotify();
		if(!ctx_vector[n]->keyEvent(&c, buf, candi, svr->srv()))
			st = false;
	}
	catch (...) {}
	return st;
}


void OVIME2Instance::closeModule()
{
	if(activatedIm > -1)
	{
		ctx_vector[activatedIm]->end();
		startedCtxVector[activatedIm] = false;
		activatedIm = -1;
	}
}

AVDisplayServer *OVIME2Instance::setBufString(const char *str)
{
	wchar_t* wstr = NULL;
	int len;
	if( str && (len = svr->srv()->UTF8ToUTF16( str, &wstr )) > 0 ) {
		compStr.assign( wstr, len );
	}
	else {
		compStr.clear();
	}
	return this;
}

AVDisplayServer *OVIME2Instance::sendBuf(const char *str)
{
	wchar_t* wstr = NULL;
	int len;
	if( str && (len = svr->srv()->UTF8ToUTF16( str, &wstr )) > 0 ) {
		resultStr.assign( wstr, len );
	}
	else {
		resultStr.clear();
	}
	compStr.clear();
	return this;
}

AVDisplayServer *OVIME2Instance::setCandiString(const char *str)
{
	cout << "setCandiString " << str << endl;
	return this;
}

AVDisplayServer *OVIME2Instance::showBuf(bool t)
{
	showCompWnd = t;
	return this;
}

AVDisplayServer *OVIME2Instance::showCandi(bool t)
{
	showCandWnd = t;
	return this;
}

AVDisplayServer *OVIME2Instance::showNotify(const char *str)
{
	wchar_t* wstr = NULL;
	int len;
	if( str && (len = svr->srv()->UTF8ToUTF16( str, &wstr )) > 0 ) {
		notifyStr.assign( wstr, len );
	}
	else {
		notifyStr.clear();
	}
	return this;
}

AVDisplayServer *OVIME2Instance::hideNotify()
{
	notifyStr.clear();
	return this;
}

AVDisplayServer *OVIME2Instance::setCursorPos(int i)
{
	cursorPos = i;
	return this;
}

AVDisplayServer *OVIME2Instance::setMarkFrom(int i)
{
	markFrom = i;
	return this;
}

AVDisplayServer *OVIME2Instance::setMarkTo(int i)
{
	markTo = i;
	return this;
}

