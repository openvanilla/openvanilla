#include "AVEmbeddedLoader.h"

AVEmbeddedLoader::AVEmbeddedLoader()
{
	m_srv = new AVService();
	m_cfg = new AVConfig();
	m_dict = new AVDictionary(m_cfg->getBaseDir());
	m_modlist = AVLoadEverything(m_cfg->getModuleDir(), m_srv);
}

AVEmbeddedLoader::~AVEmbeddedLoader()
{
	delete m_srv;
	delete m_cfg;
	delete m_dict;
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
