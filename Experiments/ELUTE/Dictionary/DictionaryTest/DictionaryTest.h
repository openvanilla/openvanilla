#include <cxxtest/TestSuite.h>
#include "Dictionary.h"

using namespace std;

class DictionaryTest : public CxxTest::TestSuite 
{
public:
	void setUp(void)
	{
		string dicFilePath("../../tsi.src");
		dic_ = new Dictionary(dicFilePath);
	}

	void testGetValueVector(void)
	{
		vector<string> values;
		string key("\xE3\x84\xA7");
		size_t count = dic_->getValueVector(key, values);
		TS_ASSERT_LESS_THAN(0, count);
		cout << "dic[" << key << "](" << count << ")={";
		for(size_t i = 0; i < count; i++)
			cout << values[i] << ",";
		cout << "}" << endl;
	}
	
	void tearDown (void)
	{
		delete dic_;
	}
	
private:
	Dictionary* dic_;
};
