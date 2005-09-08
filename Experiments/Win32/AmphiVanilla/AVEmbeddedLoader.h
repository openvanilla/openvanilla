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
	AVEmbeddedLoader();
	~AVEmbeddedLoader();
	AVService *srv();
	AVConfig *cfg();
	AVDictionary *dict();
	AVDictionary *dict(const char *);
	std::vector<OVModule*> &modlist();
private:
	AVService *m_srv;
	AVConfig *m_cfg;
	AVDictionary *m_dict;
	std::vector<OVModule*> m_modlist;
};

#endif // AVEmbeddedLoader_h
