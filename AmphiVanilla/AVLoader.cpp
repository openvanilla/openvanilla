//#define OV_DEBUG
#include "AVLoader.h"
#include <exception>
#include <algorithm>
#include <functional>
#include <string>
using namespace std;

AVLoader* AVLoader::globalLoader = NULL;

AVLoader* AVLoader::open()
{
	//if(globalLoader) delete globalLoader; 
	if(globalLoader == NULL)
		globalLoader = new AVLoader();
	return globalLoader;
}


void AVLoader::close()
{
	if(globalLoader) delete globalLoader;
	globalLoader = NULL;
}

AVLoader::AVLoader() : dsvr(0) 
{
	em = AVEmbeddedLoader::getInstance();
	murmur("loading modules.");
	loadModules();
	murmur("loading pref.");
	loadPrefs();

	candi = new AVCandidate();
	buf = new AVBuffer(&ovof_vector, em->srv());
	em->srv()->setCandi(candi);
}

AVLoader::~AVLoader()
{
	/* FIXME: uncomment this will cause segfault in ime.
	for(int i = 0; i < ctx_vector.size(); i++)
		if(ctx_vector[i] != NULL)
			delete ctx_vector[i];
	*/

	ctx_vector.clear();
	ovof_vector.clear();
//	startedCtxVector.clear();
	murmur("Shutdown loader: clear containers");
	em->removeInstance();
	murmur("Shutdown loader: delete em");
	

	delete candi; 
	delete buf; 
}

const char* AVLoader::getGlobalConfigKey(const char* key)
{
	//em->dict()->setGlobalConfig("StatusPosition"); 
	return em->dict()->getString(key);
	 
	
}
void AVLoader::setGlobalConfigKey(const char* key, const char* value)
{
	//em->dict()->setGlobalConfig("StatusPosition");
	em->dict()->setString(key, value);
	
}

void AVLoader::setGlobalConfig(const char* name)
{
	em->dict()->setGlobalConfig(name);
	
}


void AVLoader::loadPrefs()
{
	em->dict()->update();

	
	murmur("em->modlist().size() :%d",em->modlist().size());
	for(size_t i = 0; i < imCount+kpCount; i++) {		
		if(em->modlist().at(i) != NULL) {
			murmur("Reload: %s", em->modlist().at(i)->identifier());
			em->dict()->setDict(em->modlist().at(i)->identifier());
			em->modlist().at(i)->update(em->dict(), em->srv());
		}
	}
}

void AVLoader::loadModules()
{
	hasBoPoMoFo = false;
	hasPhoneticHsu = false;

	kpCount = 0;
	imCount = 0;
	ofCount = 0;

	vector<OVModule*> tmpmod_vector;
	vector<OVModule*>::iterator m;
	for(m = em->modlist().begin(); m != em->modlist().end(); m++) {		
		em->dict()->setDict((*m)->identifier());
		murmur("\tloading %s...", (*m)->identifier() );
		if(!strcmp((*m)->moduleType(), "OVInputMethod")) {
			murmur("loading IM module...");
			if(!em->dict()->keyExist("enable")) {
				em->dict()->setInteger("enable", 1);   //loaded for first time
				em->dict()->setString("LocalizedName-zh_TW", (*m)->localizedName("zh_TW"));
				em->dict()->setString("LocalizedName-zh_CN", (*m)->localizedName("zh_CN"));
				em->dict()->setString("LocalizedName-en", (*m)->localizedName("en_US"));
				em->dict()->setString("moduleType", (*m)->moduleType());
				//(*m)->initialize(em->dict(), em->srv(), em->cfg()->getModuleDir()); // loading the module for setting default config
				tmpmod_vector.push_back(*m);				
				imCount++;
			} else {
				if(em->dict()->getInteger("enable")) {
					tmpmod_vector.push_back(*m);
					imCount++;
					murmur("\tenabled em_dict_name=%s",(*m)->identifier());

					if(!strcmp((*m)->localizedName("en"), "BoPoMoFo"))
						hasBoPoMoFo = true;
					if(!strcmp((*m)->localizedName("en"), "PhoneticHsu"))
						hasPhoneticHsu = true;
				}
				else
					murmur("\tdisabled em_dict_name=%s",(*m)->identifier());
			}
		} 
		else if(!strcmp((*m)->moduleType(), "OVKeyPreprocessor")){
			murmur("loading keyPreprocesser...");
			if(!em->dict()->keyExist("enable")) {
				em->dict()->setInteger("enable", 1);
				em->dict()->setString("LocalizedName-zh_TW", (*m)->localizedName("zh_TW"));
				em->dict()->setString("LocalizedName-zh_CN", (*m)->localizedName("zh_CN"));
				em->dict()->setString("LocalizedName-en", (*m)->localizedName("en_US"));
				em->dict()->setString("moduleType", (*m)->moduleType());
				tmpmod_vector.push_back(*m);				
				kpCount++;
			} else {
				if(em->dict()->getInteger("enable")) {
					tmpmod_vector.push_back(*m);
					kpCount++;
					murmur("\tenabled em_kp_name=%s",(*m)->identifier());
				}
				else
					murmur("\tdisabled em_kp_name=%s",(*m)->identifier());
			}


		}
		else if(!strcmp((*m)->moduleType(), "OVOutputFilter")) {
			murmur("loading output filter...");
			if(!em->dict()->keyExist("enable")) {
				em->dict()->setInteger("enable", 0);
				em->dict()->setString("LocalizedName-zh_TW", (*m)->localizedName("zh_TW"));
				em->dict()->setString("LocalizedName-zh_CN", (*m)->localizedName("zh_CN"));
				em->dict()->setString("LocalizedName-en", (*m)->localizedName("en_US"));
				em->dict()->setString("moduleType", (*m)->moduleType());
				OVOutputFilter *of = dynamic_cast<OVOutputFilter*>(*m);
				murmur("\tload output filter for the first time. %s", of->identifier());
				//ovof_vector.push_back(of);   //not loaded of by default.
			
			} else 	if(em->dict()->getInteger("enable")  || !strcmp((*m)->identifier(), "OVOFFullWidthCharacter")) {
				OVOutputFilter *of = dynamic_cast<OVOutputFilter*>(*m);
				murmur("\tload enabled output filter %s", of->identifier());
				//of->initialize(em->dict(), em->srv(), em->cfg()->getModuleDir());
				//ovof_vector.push_back(of);
				tmpmod_vector.push_back(*m);
				ofCount++;
				} else {
				murmur("\tdisabled of_name=%s",(*m)->identifier());
				}
			
		}
	}
	em->modlist() = tmpmod_vector; // Orz
	//int size = em->modlist().size();
	//ctx_vector.assign(size, static_cast<OVInputMethodContext*>(NULL));
	//ctx_vector.assign(kpCount +1, static_cast<OVInputMethodContext*>(NULL));
//	startedCtxVector.assign(kpCount+imCount, false);
	activatedIm = -1;
	//ovof_vector.assign(ofCount, static_cast<OVOutputFilter*>(NULL));
	
	
	//if(imCount > 0) initContext(0); //load first IM. 
}

void AVLoader::initContext(int n)
{
	if(!imCount) return;
	ctx_vector.clear();
	ovof_vector.clear();
	murmur("\tInitContext for module:%d, kpCount:%d, imCount:%d, ofCount:%d", n, kpCount, imCount, ofCount);
	if(!strcmp(em->modlist()[n]->moduleType(), "OVInputMethod"))
	{		
		for(size_t i=0; i < kpCount; i++ ){
			
			OVInputMethod *im = reinterpret_cast<OVInputMethod*>(em->modlist()[i+imCount]);
			if(dsvr->getStatusModuleChecked(i)){
				em->dict()->setDict(im->identifier());
				im->initialize(em->dict(), em->srv(), em->cfg()->getModuleDir());
				murmur("\tInitContext %s", im->localizedName(em->srv()->locale()));
				ctx_vector.push_back(im->newContext());  // Load the kp if it's checked in the menu
				if(ctx_vector.size()>0&&ctx_vector[ctx_vector.size()-1])//&&!startedCtxVector[i+imCount]) 
				{
					ctx_vector[ctx_vector.size()-1]->start(buf, candi, em->srv());	
					//startedCtxVector[i+imCount] = true;
					murmur("\tStarting %s",  im->localizedName(em->srv()->locale()));
				}
			}
		}
		  // IM at the last item of ctx_vector
		OVInputMethod *im = reinterpret_cast<OVInputMethod*>(em->modlist()[n]);
		em->dict()->setDict(im->identifier());
		im->initialize(em->dict(), em->srv(), em->cfg()->getModuleDir());
		murmur("\tInitContext %s", im->localizedName(em->srv()->locale()));
		ctx_vector.push_back(im->newContext());
		if(ctx_vector.size()>0&&ctx_vector[ctx_vector.size()-1])//&&!startedCtxVector[i+imCount]) 
		{
			ctx_vector[ctx_vector.size()-1]->start(buf, candi, em->srv());	
			//startedCtxVector[i+imCount] = true;
			murmur("\tStarting %s",  im->localizedName(em->srv()->locale()));
		}



			
		for(size_t i=kpCount; i<kpCount+ofCount; i++){
			OVOutputFilter *of = dynamic_cast<OVOutputFilter*>(em->modlist()[i+imCount]);
			
			if(i==kpCount) { // The first OF, OVFullwidthcharacter, is always loaded
				em->dict()->setDict(of->identifier());
				of->initialize(em->dict(), em->srv(), em->cfg()->getModuleDir());
				ovof_vector.push_back(of);  
				murmur("\tInit OF %s", of->localizedName(em->srv()->locale()));
			}
			else if(dsvr->getStatusModuleChecked(i)){	// ,else loaded the of if it's checked in the menu 
				em->dict()->setDict(of->identifier());
					of->initialize(em->dict(), em->srv(), em->cfg()->getModuleDir());
				ovof_vector.push_back(of);
				murmur("\tInit OF %s", of->localizedName(em->srv()->locale()));
			}
		}
		
	}
}

void AVLoader::connectDisplayServer(AVDisplayServer *svr)
{
	murmur("AVLoader::connect dsvr\n"); 
	dsvr = svr;
	LPPOINT lp_pt;
	lp_pt = new POINT;
 
	candi->setDisplayServer(dsvr);
	buf->setDisplayServer(dsvr);
	em->srv()->setDisplayServer(dsvr);



}

bool AVLoader::keyEvent(int n, AVKeyCode c) 
{
	
	if(n > (int) imCount) return 0;
	
	if(n != activatedIm)  
	{
		initContext(n);  // Rebuild ctx_vector if acivatedIM changed.
		activatedIm = n;
		murmur("Staring IM module#%d", n);						
		
	}
	else if( candi->onScreen() ){   // override up/down/enter key when candi on screen for key selection navigation.
		murmur("candi->onScreen; Loader->Keyev:%x\n",c.code());
		switch (c.code()) {
		case ovkDown:
			dsvr->selectNextCandItem(1);
			return 1;
		case ovkUp: 
			dsvr->selectNextCandItem(-1); 
			return 1;
#ifdef WINCE
		case ovkPageDown:
			dsvr->selectNextCandItem(2);
			return 1;
		case ovkPageUp: 
			dsvr->selectNextCandItem(-2); 
			return 1;
#endif
		case ovkReturn:
			{
#ifdef WINCE
			return dsvr->getCandSelectedItemKey();
#else
			int k = dsvr->getCandSelectedItemKey();
			if(k) c.setCode(k);
#endif
			break;
			}
		default:
			break;
		}

		
	}
	
	// Pass kp first, and the active IM at ctx_vector[kpcount]
	murmur("passing kp + active IM, %d modules, IM moduleId:%d, activatedIM:%d, key:%x",  ctx_vector.size(),n, activatedIm, c.code());
	bool st;
	for(size_t i=0; i < ctx_vector.size(); i++ ){
		//murmur("passsing module#%d", i);
		st = true;
		try {
			if(!ctx_vector[i]->keyEvent(&c, buf, candi, em->srv()))
				st = false;
		}
		catch (...) 
		{	
			murmur("module #%d keyevent exception",i);
		}
			if(st) return st;
	}


	
	dsvr->hideNotify();
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
	s = em->modlist()[i]->localizedName(em->srv()->locale());
	strcpy(str, s.c_str());
	return true;
}

void AVLoader::unloadCurrentModule()
{
	if(activatedIm > -1)
	{
		if(buf && !buf->isEmpty() &&activatedIm>-1)
		{
			AVKeyCode k;
			k.setCode(ovkReturn);  // force commit before leaving 
			keyEvent(activatedIm,k);
			//buf->send()->clear(); 
		}
		candi->hide();
		ctx_vector[ctx_vector.size()-1]->end();
		//startedCtxVector[activatedIm] = false;
		activatedIm = -1;
	}
}
/*
int AVLoader::getInputMethodCount()
{
	return (int) em->modlist().size();
}

int AVLoader::getOutputFilterCount()
{
	return (int) (ovof_vector.size()-1); // exclude Fullcharaterwidth OF module.
}
*/

int AVLoader::getSwitchedBoPoMoFoLayoutModIndex(int currentId)
{
	/*
	if(activatedIm == -1) {
		if(hasBoPoMoFo && hasPhoneticHsu) {
			for(size_t i = 0; i < em->modlist().size(); i++)
				if(!strcmp(em->modlist()[i]->localizedName("en"),"PhoneticHsu") ||
					!strcmp(em->modlist()[i]->localizedName("en"), "BoPoMoFo") ){
					//activatedIm = i;
					return i;
				}
		}
		else return -1;
	}
	*/
	 

	if(strcmp(em->modlist()[currentId]->localizedName("en"), "PhoneticHsu") &&
		strcmp(em->modlist()[currentId]->localizedName("en"), "BoPoMoFo")  ) {
		if(hasBoPoMoFo && hasPhoneticHsu) {
			for(size_t i = 0; i < em->modlist().size(); i++)
				if(!strcmp(em->modlist()[i]->localizedName("en"),"PhoneticHsu") ||
					!strcmp(em->modlist()[i]->localizedName("en"), "BoPoMoFo") ){
					//activatedIm = i;
					return i;
				}
		}
		else return -1;
	}


	
	if(hasBoPoMoFo &&
		!strcmp(em->modlist()[currentId]->localizedName("en"), "PhoneticHsu")) {
		for(size_t i = 0; i < em->modlist().size(); i++) {
			if(!strcmp(em->modlist()[i]->localizedName("en"), "BoPoMoFo")) {
				//unloadCurrentModule();
				//activatedIm = i;
				return i;
			}
		}
	}
	
	if(hasPhoneticHsu &&
		!strcmp(em->modlist()[currentId]->localizedName("en"), "BoPoMoFo")) {
		for(size_t i = 0; i < em->modlist().size(); i++) {
			if(!strcmp(em->modlist()[i]->localizedName("en"), "PhoneticHsu")) {
				//unloadCurrentModule();
				//activatedIm = i;
				return i;
			}
		}
	}

	return currentId;
}

void AVLoader::getAllModuleNames(const char* nameList[])
{		
	if(em && (em->modlist().size() ) > 0){  
		for(size_t i = 0; i < imCount+kpCount+ofCount; i++){
			nameList[i] = em->modlist()[i]->localizedName(em->srv()->locale());
			if(!strcmp(nameList[i],""))	nameList[i] =em->modlist()[i]->identifier();
			murmur("\t get module name %i:%s", i, nameList[i] );
		
			}
		
	}
	
	else
		nameList = NULL;
	
}

const char* AVLoader::getBaseDirectory() 
{
	
	return em->cfg()->getBaseDir();
	
	
}

void AVLoader::syncMenuConfig(int moduleid)
{
	em->dict()->setDict(em->modlist().at(moduleid)->identifier() );

	for(size_t i=0;i<kpCount; i++)
		em->dict()->setInteger(
			em->modlist().at(i+imCount)->identifier(),dsvr->getStatusModuleChecked(i));

	for(size_t i=kpCount+1;i<kpCount+ofCount; i++)
		em->dict()->setInteger(
			em->modlist().at(i+imCount)->identifier(),dsvr->getStatusModuleChecked(i));	

}

void AVLoader::syncConfigMenu(int moduleid)
{

#ifndef WINCE // load UI Sharesettings  
	em->dict()->setGlobalConfig("SharedSettings");  
	
	wchar_t wbuf[64];
	mbstowcs(wbuf, em->dict()->getStringWithDefault("NotifyFont", "Microsoft JhengHei"), 64);

	dsvr->setNotifyFont(wbuf,
		em->dict()->getIntegerWithDefault("NotifyFontSize",14));
	
	mbstowcs(wbuf, em->dict()->getStringWithDefault("CandiFont", "Microsoft JhengHei"), 65);

	dsvr->setCandiFont(wbuf,
		em->dict()->getIntegerWithDefault("CandiFontSize",12)); 

	dsvr->setCandiColor(	
		em->dict()->getIntegerWithDefault("CandiFontColor",0xff000000), //black
		em->dict()->getIntegerWithDefault("CandiBackColor",0xfff8f8ff ), // info
		em->dict()->getIntegerWithDefault("NotifyFontColor",0xffffffff), // ghostwhite
		em->dict()->getIntegerWithDefault("NotifyBackColor",0xff646464)); // gray

	dsvr->setNotifyColor(
		em->dict()->getIntegerWithDefault("NotifyFontColor",0xffffffff),
		em->dict()->getIntegerWithDefault("NotifyBackColor",0xff646464));
	dsvr->setNotifyGlobalEnabled(
		(em->dict()->getIntegerWithDefault("NotifyWindowEnabled", 1) >0) ?true :false );
	int o =em->dict()->getIntegerWithDefault("Opacity", 95);
	dsvr->setNotifyOpacity(o);
	dsvr->setCandiOpacity(o);
	
	
	
#endif

	
	//Loading per module KP/OF checked list from config.
	em->dict()->setDict(em->modlist().at(moduleid)->identifier() );

	for(size_t i=0;i<kpCount; i++) 	{
		murmur("restore check flag for KP module #%d %s",i,em->modlist().at(i+imCount)->identifier());
		if(em->dict()->getIntegerWithDefault(
				em->modlist().at(i+imCount)->identifier(),1)) // KP is activated by default
			dsvr->setStatusModuleChecked(i,true);
		else
			dsvr->setStatusModuleChecked(i,false);
	}

	for(size_t i=kpCount+1;i<kpCount+ofCount; i++) 	{ //skip OVOFFullwidth
		murmur("restore check flag for OF module #%d %s",i,em->modlist().at(i+imCount)->identifier());
		if(em->dict()->getIntegerWithDefault(
				em->modlist().at(i+imCount)->identifier(),0)) // OF is not activated by default
			dsvr->setStatusModuleChecked(i, true);
		else
			dsvr->setStatusModuleChecked(i, false);



	}
		

}