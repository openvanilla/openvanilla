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
		element.set(keystroke);

		string word("apple");
		chunk.set(word);
	}

	void testGet(void)
	{
		TS_ASSERT_SAME_DATA(element.get()->c_str(), "a", 1);

		TS_ASSERT_SAME_DATA(chunk.get()->c_str(), "apple", 5);
	}

        void testAdd(void)
        {
		TS_ASSERT(!element.add(&chunk));

		TS_ASSERT(chunk.add(&element));
        }

	void testGetChild(void)	
	{
		TS_ASSERT(!element.getChild(0));

		TS_ASSERT_SAME_DATA(chunk.getChild(0)->get()->c_str(), "a", 1);
	}

        void testRemove(void)
        {
		TS_ASSERT(!element.remove(0));

                TS_ASSERT(chunk.remove(0));
        }

private:
        Chunk chunk;
        Element element;
};
