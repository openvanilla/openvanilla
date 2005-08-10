#include <string>
#include "DOVDictionary.h"
#include "OVUtility.h"

DummyDictionary::DummyDictionary(const char *f, const char *dict)
{
	file = f;
	file += "config.xml";
	parser = scew_parser_create();
	scew_parser_ignore_whitespaces(parser, 1);
	if(!scew_parser_load_file(parser, file.c_str())) {
		createNewConfig(file.c_str());
		scew_parser_load_file(parser, file.c_str());
	}
	tree = scew_parser_tree(parser);
	root = scew_tree_root(tree);
	scew_element* i = scew_element_next(root, NULL);
	while(i) {
		if(!strcmp(scew_element_name(i), "dict")) {
			scew_attribute* at = scew_attribute_by_name(i, "name");
			if(at && !strcmp(scew_attribute_value(at), dict))
				break;
		}
		i = scew_element_next(root, i);
	}
	module = i;
	name = dict;
}

bool DummyDictionary::createNewConfig(const char *f)
{
	scew_tree *t = NULL;
	scew_element *r = NULL;
	t = scew_tree_create();
	r = scew_tree_add_root(t, "OpenVanilla");
	scew_writer_tree_file(t, f);
	scew_tree_free(t);
	return true;
}

void DummyDictionary::newDict()
{
	module = scew_element_add(root, "dict");
	scew_element_add_attr_pair(module, "name", name.c_str());
	scew_writer_tree_file(tree, file.c_str());
}

DummyDictionary::~DummyDictionary()
{
	scew_parser_free(parser);
}

int DummyDictionary::keyExist(const char *key)
{
	unsigned int count;
	int retval = 0;
	if(!module) return retval;
	scew_element** list = scew_element_list(module, "key", &count);
	for(int i = 0; i < count; i++) {
		scew_attribute* at = scew_attribute_by_name(list[i], "name");
		if(at && !strcmp(scew_attribute_value(at), key)) {
			retval = 1;
			break;
		}
	}
	scew_element_list_free(list);
	return retval;
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
	int i;
	unsigned int count;
	if(!module) return "";
	scew_element** list = scew_element_list(module, "key", &count);
	for(i = 0; i < count; i++) {
		scew_attribute* at = scew_attribute_by_name(list[i], "name");
		if(at && !strcmp(scew_attribute_value(at), key))
			break;
	}
	if(i == count) {
		scew_element_list_free(list);
		return "";
	}
	scew_attribute* at = scew_attribute_by_name(list[i], "value");
	const XML_Char *value = at ? scew_attribute_value(at) : NULL;
	scew_element_list_free(list);
	if(value)
		return value;
	else
		return "";
}

const char* DummyDictionary::setString(const char *key, const char *value)
{
	int i;
	unsigned int count;
	scew_element *cur;
	if(!module)
		newDict();
	scew_element** list = scew_element_list(module, "key", &count);
	for(i = 0; i < count; i++) {
		scew_attribute* at = scew_attribute_by_name(list[i], "name");
		if(at && !strcmp(scew_attribute_value(at), key))
			break;
	}
	if(i == count)
		cur = scew_element_add(module, "key");
	else
		cur = list[i];
	scew_element_add_attr_pair(cur, "name", key);
	scew_element_add_attr_pair(cur, "value", value);
	scew_writer_tree_file(tree, file.c_str());
	scew_element_list_free(list);
	return value;
}

/*
int main()
{
	DummyDictionary d("F:\\windows\\openvanilla\\", "Orz");
	d.setInteger("ccc", 456);
	if(d.keyExist("ccc"))
	{
		printf("orz/aaa: %d\n", d.getInteger("aaa"));
		printf("orz/aaa: %s\n", d.getString("ccc"));
	}
	return 0;
}
*/
