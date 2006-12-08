#define OV_DEBUG
#include "AVEmbeddedLoader.h"
#include <algorithm>
#include <functional>
using namespace std;

AVEmbeddedLoader* AVEmbeddedLoader::embeddedLoaderInstance = NULL;

struct sort_im : public binary_function< OVModule*, OVModule*, bool >
{
	sort_im(AVDictionary* dict) : m_dict(dict) {}
	int operator ()(OVModule *a, OVModule *b)
	{
		int pa = 0, pb = 0;
		m_dict->setDict(a->identifier());
		pa = m_dict->getInteger("priority");
		m_dict->setDict(b->identifier());
		pb = m_dict->getInteger("priority");
		return (pa > pb);
	}
	AVDictionary* m_dict;
};


AVEmbeddedLoader::AVEmbeddedLoader()
{
	m_srv = new AVService();
	m_cfg = new AVConfig();
	m_dict = AVDictionary::getDict(m_cfg->getUserDir());
	m_dict->setAutoFlush(true);
	//murmur("AVEmbeddedLoader::AVEmbeddedLoader()->module dir= %s",m_cfg->moduledir());
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

AVService *AVEmbeddedLoader::srv() { return m_srv; }
AVConfig *AVEmbeddedLoader::cfg() { return m_cfg; }
AVDictionary *AVEmbeddedLoader::dict() { return m_dict; }
AVDictionary *AVEmbeddedLoader::dict(const char* d)
{
	m_dict->setDict(d);
	return m_dict;
}
std::vector<OVModule*> &AVEmbeddedLoader::modlist() { return m_modlist; }
