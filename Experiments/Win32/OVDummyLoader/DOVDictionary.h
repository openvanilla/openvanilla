#include <string>
#include "OpenVanilla.h"
extern "C" {
#include "scew/scew.h"
}

class DummyDictionary : public OVDictionary {
	public:
		DummyDictionary(const char *f, const char *path);
		virtual ~DummyDictionary();
		virtual int keyExist(const char *key);
		virtual int getInteger(const char *key);
		virtual int setInteger(const char *key, int value);
		virtual const char* getString(const char *key);
		virtual const char* setString(const char *key, const char *value);
		bool createNewConfig(const char *path);
	private:
		std::string name;
		std::string file;
		void newDict();
		scew_parser* parser;
		scew_tree* tree;
		scew_element* root;
		scew_element* module;
};
