/*
 * Implementation of AVDictionary
 */
#include "AVDictionary.h"
#include <iconv.h>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

/* used */
AVDictionary::AVDictionary()
{
	_readonly = true;
	//_dict = new std::map<std::string, std::string>;
}

/* used */
AVDictionary::AVDictionary(const AVDictionary &dict)
{
	_readonly = dict._readonly;
	_dict = _readonly ? _dict : dict._dict;
}

AVDictionary::~AVDictionary()
{
	//if (_readonly) delete _dict;
}

/* used */
const AVDictionary& AVDictionary::operator=(const AVDictionary &dict)
{
	_readonly = dict._readonly;
	_dict = _readonly ? _dict : dict._dict;
	return *this;
}

/* used */
bool AVDictionary::dictionaryExists(const char *key)
{
	/*
	PlistNode *n = _datanode->findChildByKey(key);
	if (!n) return false;

	if (n->name()=="dict") return true;
	*/
	return true;
}

/* used */
AVDictionary AVDictionary::getDictionary(const char *key, bool readonly)
{
	/*
	if (!dictionaryExists(key)) return AVDictionary();

	PlistNode *n = _datanode->findChildByKey(key);
	if (_readonly) return AVDictionary(new PlistNode(*n));
	return AVDictionary(n, false);        
	*/

	return *this;
}

/* used */
void AVDictionary::createDictionaryForKey(const char *key)
{
	/*
	if (dictionaryExists(key)) return;

	PlistNode n("dict");
	_datanode->setChildByKey(key, n);
	*/
	return;
}

bool AVDictionary::keyExist(const char *key) {
	return _dict.find(key) != _dict.end();
}

int AVDictionary::getInteger(const char *key) {
	return atoi(_dict[key].c_str());
}

int AVDictionary::setInteger(const char *key, int value) {
	std::stringstream ss;
	ss << value;
	_dict.insert( std::make_pair(key, ss.str()) );
	return value;
}

const char* AVDictionary::getString(const char *key) {
	return _dict[key].c_str();
}

const char* AVDictionary::setString(const char *key, const char *value) {
	_dict.insert( std::make_pair(key, value) );
	return value;
}
