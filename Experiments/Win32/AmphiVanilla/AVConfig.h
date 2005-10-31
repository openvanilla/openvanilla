#ifndef AVConfig_h
#define AVConfig_h
#include <string>

class AVConfig
{
public:
	AVConfig();
	const char *getBaseDir();
	const char *getUserDir();
	const char *getModuleDir();
	void setBaseDir(const char *);
	void setUserDir(const char *);
	void setModuleDir(const char *);
private:
	std::string basedir;
	std::string userdir;
	std::string moduledir;
};

#endif // AVConfig_h
