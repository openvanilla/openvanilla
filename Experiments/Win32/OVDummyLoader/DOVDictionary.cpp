#include <string>
#include "DOVDictionary.h"
#include "OVUtility.h"

DummyDictionary::DummyDictionary(const char *f, const char *dict)
{
	file = f;
	file += "config.xml";
	if(!doc.LoadFile(file)) {
		createNewConfig(file);
		doc.LoadFile();
	}

	module = findChild(doc.RootElement(), "dict", dict);
	name = dict;
}

bool DummyDictionary::createNewConfig(std::string file)
{
	TiXmlDocument tmp(file);
	tmp.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><OpenVanilla></OpenVanilla>");
	tmp.SaveFile();
	return true;
}

void DummyDictionary::newDict()
{
	TiXmlElement tmp("dict");
	tmp.SetAttribute("name", name.c_str());
	module = doc.RootElement()->InsertEndChild(tmp);
	doc.SaveFile();
}

TiXmlNode *DummyDictionary::findChild(TiXmlNode *parent, const char *node, const char *name)
{
	TiXmlNode *child = 0;
	while( child = parent->ToElement()->IterateChildren( child ) ) {
		if(!strcmp(child->Value(), node))
			if(!strcmp(child->ToElement()->Attribute("name"), name))
				break;
	}
	return child;
}

DummyDictionary::~DummyDictionary()
{
}

int DummyDictionary::keyExist(const char *key)
{
	if(module && findChild(module, "key", key))
		return 1;
	else
		return 0;
}

int DummyDictionary::getInteger(const char *key)
{
	return atoi(getString(key));
}

int DummyDictionary::setInteger(const char *key, int value)
{
	char tmp[1024];
	sprintf(tmp, "%d", value);
	setString(key, tmp);
	return value;
}

const char* DummyDictionary::getString(const char *key)
{
	if(!module) return "";
	TiXmlNode *child = findChild(module, "key", key);
	if(child)
		return child->ToElement()->Attribute("value");
	else
		return "";
}

const char* DummyDictionary::setString(const char *key, const char *value)
{
	if(!module)
		newDict();
	TiXmlNode *child;
	child = findChild(module, "key", key);
	if(!child) {
		TiXmlElement tmp("key");
		tmp.SetAttribute("name", key);
		child = module->ToElement()->InsertEndChild(tmp);
	}
	child->ToElement()->SetAttribute("value", value);
	doc.SaveFile();
	return value;
}

/*
int main()
{
	DummyDictionary d(".\\", "Orz");
	if(d.keyExist("ccc"))
		std::cout << "Ya!" << d.getString("ccc") << std::endl;
	d.setInteger("ccc", 456);
	d.setString("bbb", "hahaha");
	if(d.keyExist("ccc"))
	{
		printf("orz/aaa: %d\n", d.getInteger("aaa"));
		printf("orz/aaa: %s\n", d.getString("ccc"));
		printf("orz/bbb: %s\n", d.getString("bbb"));
	}
	return 0;
}
*/
