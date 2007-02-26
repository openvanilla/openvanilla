#include <iostream>
#include <UnitTest++.h>
#include "Dictionary.h"

using namespace std;
using namespace UnitTest;

class TestDictionary 
{
public:
	TestDictionary()
	{
		string dicFilePath("./tsi.src");
		dic_ = new Dictionary(dicFilePath);
	}

	~TestDictionary()
	{
		delete dic_;
	}
	
	Dictionary* dic_;
};

TEST_FIXTURE(TestDictionary, DictionaryTest)
{
	Dictionary* dic = dic_;
	vector<string> values;
	string key("ㄧ");
	size_t count = dic->getValueVector(key, values);
	CHECK(count > 0);
	cerr << key << "(" << count << "):";
	for(size_t i = 0; i < count; i++)
		cerr << values[i] << " ";
}

int main()
{
	return RunAllTests();
}
