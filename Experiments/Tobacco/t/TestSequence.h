#include <cxxtest/TestSuite.h>
#include "Sequence.h"

#include <string>

using namespace std;
          
class TestSequence : public CxxTest::TestSuite 
{
public:
	void setUp(void)
	{
		string keystroke("a");
		TS_ASSERT(element.set(keystroke));
	}

	void tearDown(void)
	{
	}

        void testAdd(void)
        {
		TS_ASSERT(clause.add(element));
        }

	void testGetChild(void)	
	{
		Element tempElement;
		TS_ASSERT(clause.getChild(0, tempElement));
	}

        void testRemove(void)
        {
                TS_ASSERT(clause.remove(0));
        }

private:
        Clause clause;
        Element element;
};
