//#define OV_DEBUG
#include "AVEmbeddedLoader.h"
#include <algorithm>
#include <functional>
using namespace std;

AVEmbeddedLoader* AVEmbeddedLoader::embeddedLoaderInstance = NULL;

struct sort_im : public binary_function< OVModule*, OVModule*, bool >
{
	sort_im(AVDictionary* dict) : m_dict(dict) {}
	int operator ()(OVModule *a, OVModule *b)
	{	int pa = 0, pb = 0;
		m_dict->setDict(a->identifier());
		pa = m_dict->getInteger("priority");
		if(!strcmp(a->moduleType(),"OVInputMethod")) pa+=2000;   //offset OVIM modules before OF and KP  modules 
		if(!strcmp(a->moduleType(),"OVKeyPreprocessor")) pa+=1000; //offset OVKP modules before OF modules  
		if(!strcmp(a->identifier(),"OVOFFullWidthCharacter")) pa+=100; //offset FullWidthOF to be the first OF.
		m_dict->setDict(b->identifier());
		pb = m_dict->getInteger("priority");
		if(!strcmp(b->moduleType(),"OVInputMethod")) pb+=2000;  //offset OVIM modules before OFKP modules
		if(!strcmp(b->moduleType(),"OVKeyPreprocessor")) pb+=1000;   //offset OVKP modules before OF modules 
		if(!strcmp(b->identifier(),"OVOFFullWidthCharacter")) pb+=100; //offset FullWidthOF to be the first OF.
		return (pa > pb);
	}
	AVDictionary* m_dict;
};


AVEmbeddedLoader::AVEmbeddedLoader()
{
	m_cfg = new AVConfig();
	

	m_dict = AVDictionary::getDict(m_cfg->getUserDir());
	m_dict->setAutoFlush(true);
	
	m_dict->setGlobalConfig("Directories");
	m_cfg->setBaseDir(m_dict->getStringWithDefault("BaseDir",m_cfg->getBaseDir() ));  //Set basedir from config.xml with default from AVConfig.
	m_cfg->setModuleDir(m_dict->getStringWithDefault("ModuleDir",m_cfg->getModuleDir()));
	m_cfg->setUserDir(m_dict->getStringWithDefault("UserDir",m_cfg->getUserDir()));

	murmur("Basedir:%s\n",m_cfg->getBaseDir());
	murmur("AVEmbeddedLoader::AVEmbeddedLoader()\n");	
	murmur("Module dir:%s\n",m_cfg->getModuleDir());	

	m_srv = new AVService(m_cfg->getUserDir());
	murmur("EM:Current user locale:%s\n", m_srv->locale() );

	AVLoadEverything(m_cfg->getModuleDir(), m_srv, m_modlist);
	sort(m_modlist.begin(), m_modlist.end(), sort_im(m_dict));
}

AVEmbeddedLoader::~AVEmbeddedLoader()
{
	delete m_srv;
	delete m_cfg;
	AVDictionary::shutdown();
	AVUnloadLibrary(m_modlist);
}

void AVEmbeddedLoader::removeInstance()
{
	if(embeddedLoaderInstance)
		delete embeddedLoaderInstance;
	embeddedLoaderInstance = NULL;
}

AVService *AVEmbeddedLoader::srv() { return m_srv; }
AVConfig *AVEmbeddedLoader::cfg() { return m_cfg; }
AVDictionary *AVEmbeddedLoader::dict() { return m_dict; }
AVDictionary *AVEmbeddedLoader::dict(const char* d)
{
	m_dict->setDict(d);
	return m_dict;
}
std::vector<OVModule*> &AVEmbeddedLoader::modlist() { return m_modlist; }
