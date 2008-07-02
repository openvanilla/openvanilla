#include <string>
#ifndef WINCE
#include <sys/types.h>
#endif
#include <windows.h>
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

void AVDictionary::shutdown()
{
	if(globalDict)
		delete globalDict;
	globalDict = 0;
}

bool AVDictionary::createNewConfig(std::string file)
{
	//<comment author='b6s'>Old tinyxml usage;
	//TiXmlDocument tmp(file);
	//</comment>
	// use wide char for chinese pathname compability

	//TiXmlDocument tmp(file.c_str());
	TiXmlDocument tmp;
	tmp.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><OpenVanilla></OpenVanilla>");

	wchar_t wbuf[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, file.c_str(), (int)(file.length()+1), wbuf, 256);

	FILE* fp=_wfopen(wbuf, TEXT("w"));
	tmp.SaveFile(fp);
	fclose(fp);

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
	//<comment author='b6s'>Old tinyxml usage;
	//if(!doc.LoadFile(file)) {
	//</comment>	
	
	// Use widechar for compatibility of chinese path name
	wchar_t wbuf[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, file.c_str(), (int)(file.length()+1), wbuf, 256);
	
	FILE* fp=_wfopen(wbuf, TEXT("rb"));
//	if(!doc.LoadFile(file.c_str(), TIXML_ENCODING_UTF8)) {
	if(!doc.LoadFile(fp, TIXML_ENCODING_UTF8)) {
		createNewConfig(file);
		//doc.LoadFile();
		fp=_wfopen(wbuf, TEXT("rb"));
		doc.LoadFile(fp, TIXML_ENCODING_UTF8);
	}
	fclose(fp);
}

void AVDictionary::setDict(const char *dict)
{
	module = findChild(doc.RootElement(), "dict", dict);
	name = dict;
}

void AVDictionary::setGlobalConfig(const char *globalname)
{
	module = findChild(doc.RootElement(), "Global", globalname);
	name = globalname; 
	if(!module) {
		TiXmlElement tmp("Global");
		tmp.SetAttribute("name", name.c_str());
		module = doc.RootElement()->InsertEndChild(tmp);
		if(autoflush)
		save();

	}

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
	
	wchar_t wbuf[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, file.c_str(), (int)(file.length()+1), wbuf, 256);

	FILE* fp=_wfopen(wbuf, TEXT("w"));
	doc.SaveFile(fp);
	fclose(fp);
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

#ifdef TEST_DICT
int main()
{
	AVDictionary *d = AVDictionary::getDict(".\\", "Orz");
	d->setAutoFlush(true);
	if(d->keyExist("ccc"))
		std::cout << "Ya!" << d->getString("ccc") << std::endl;
	d->setInteger("ccc", 456);
	d->setString("bbb", "hahaha");
	if(d->keyExist("ccc"))
	{
		printf("orz/aaa: %d\n", d->getInteger("aaa"));
		printf("orz/aaa: %s\n", d->getString("ccc"));
		printf("orz/bbb: %s\n", d->getString("bbb"));
	}
	return 0;
}
#endif
