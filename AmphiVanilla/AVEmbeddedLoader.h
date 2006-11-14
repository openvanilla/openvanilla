#ifndef AVEmbeddedLoader_h
#define AVEmbeddedLoader_h

#include <vector>
#include "AVConfig.h"
#include "AVDictionary.h"
#include "AVDisplayServer.h"
#include "AVLoaderUtility.h"
#include "AVService.h"

class AVEmbeddedLoader : public OVBase
{
public:
	static AVEmbeddedLoader* getInstance()
	{
		if(embeddedLoaderInstance == NULL)
			embeddedLoaderInstance = new AVEmbeddedLoader();
		return embeddedLoaderInstance;
	}

	AVService *srv();
	AVConfig *cfg();
	AVDictionary *dict();
	AVDictionary *dict(const char *);
	std::vector<OVModule*> &modlist();

protected:
	AVEmbeddedLoader();
	~AVEmbeddedLoader();

private:
	AVService *m_srv;
	AVConfig *m_cfg;
	AVDictionary *m_dict;
	std::vector<OVModule*> m_modlist;

	static AVEmbeddedLoader* embeddedLoaderInstance;
};

#endif // AVEmbeddedLoader_h
