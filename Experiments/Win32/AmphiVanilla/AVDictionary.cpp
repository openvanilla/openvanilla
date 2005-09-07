#include <string>
#include <sys/types.h>
#include "AVDictionary.h"
#include "OVUtility.h"

AVDictionary *AVDictionary::globalDict = 0;

AVDictionary::AVDictionary() : file(""), name(""), autoflush(false)
{
}

AVDictionary::AVDictionary(const char *f) : autoflush(false)
{
	setPath(f);
}

AVDictionary::AVDictionary(const char *f, const char *dict) : autoflush(false)
{
	setPath(f);
	setDict(dict);
}

AVDictionary::~AVDictionary()
{
	if(!autoflush)
		save();
}

AVDictionary* AVDictionary::getDict()
{
	if(!globalDict)
		globalDict = new AVDictionary();
	return globalDict;
}

AVDictionary* AVDictionary::getDict(const char *f)
{
	if(!globalDict)
		globalDict = new AVDictionary(f);
	return globalDict;
}

AVDictionary* AVDictionary::getDict(const char *f, const char *dict)
{
	if(!globalDict)
		globalDict = new AVDictionary(f, dict);
	return globalDict;
}

bool AVDictionary::createNewConfig(std::string file)
{
	TiXmlDocument tmp(file);
	tmp.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><OpenVanilla></OpenVanilla>");
	tmp.SaveFile();
	return true;
}

void AVDictionary::setAutoFlush(bool b)
{
	autoflush = b;
}

void AVDictionary::setPath(const char *path)
{
	file = path;
	file += "config.xml";
	if(!doc.LoadFile(file)) {
		createNewConfig(file);
		doc.LoadFile();
	}
}

void AVDictionary::setDict(const char *dict)
{
	module = findChild(doc.RootElement(), "dict", dict);
	name = dict;
}

void AVDictionary::newDict()
{
	TiXmlElement tmp("dict");
	tmp.SetAttribute("name", name.c_str());
	module = doc.RootElement()->InsertEndChild(tmp);
	if(autoflush)
		save();
}

TiXmlNode *AVDictionary::findChild(TiXmlNode *parent, const char *node, const char *key)
{
	TiXmlNode *child = 0;
	while( child = parent->ToElement()->IterateChildren( child ) ) {
		if(!strcmp(child->Value(), node))
			if(!strcmp(child->ToElement()->Attribute("name"), key))
				break;
	}
	return child;
}

void AVDictionary::update()
{
	doc.LoadFile();
}

void AVDictionary::save()
{
	doc.SaveFile();
}

int AVDictionary::keyExist(const char *key)
{
	if(module && findChild(module, "key", key))
		return 1;
	else
		return 0;
}

int AVDictionary::getInteger(const char *key)
{
	return atoi(getString(key));
}

int AVDictionary::setInteger(const char *key, int value)
{
	char tmp[1024];
	sprintf(tmp, "%d", value);
	setString(key, tmp);
	return value;
}

const char* AVDictionary::getString(const char *key)
{
	if(!module) return "";
	TiXmlNode *child = findChild(module, "key", key);
	if(child)
		return child->ToElement()->Attribute("value");
	else
		return "";
}

const char* AVDictionary::setString(const char *key, const char *value)
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
	if(autoflush)
		save();
	return value;
}

/*
int main()
{
	AVDictionary d(".\\", "Orz");
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
