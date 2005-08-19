#include <string>
#include "OpenVanilla.h"
#include "tinyxml.h"

class DummyDictionary : public OVDictionary {
	public:
		DummyDictionary(const char *f, const char *path);
		virtual ~DummyDictionary();
		virtual int keyExist(const char *key);
		virtual int getInteger(const char *key);
		virtual int setInteger(const char *key, int value);
		virtual const char* getString(const char *key);
		virtual const char* setString(const char *key, const char *value);
		bool createNewConfig(std::string path);
	private:
		std::string name;
		std::string file;
		void newDict();
		TiXmlNode *findChild(TiXmlNode *parent, const char *node, const char *name);
		TiXmlDocument doc;
		TiXmlNode *module;
};
