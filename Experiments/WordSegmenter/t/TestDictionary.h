#include <cxxtest/TestSuite.h>
#include "Dictionary.h"

using namespace std;

class TestDictionary : public CxxTest::TestSuite 
{
public:
	void setUp(void)
	{
		string dicFilePath("./tsi.src");
		dic_ = new Dictionary(dicFilePath);
	}
	
	void tearDown (void)
	{
		delete dic_;
	}
	
private:
	Dictionary* dic_;
};
