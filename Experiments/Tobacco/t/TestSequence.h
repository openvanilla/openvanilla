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
		element.setView(keystroke);

		chunk.add(&element, chunk.getIterator());

		string word("apple");		
		chunk.setView(word);
	}

	void testGetView(void)
	{
		TS_ASSERT_SAME_DATA(element.getView().c_str(), "a", 1);

		TS_ASSERT_SAME_DATA(chunk.getView().c_str(), "apple", 5);
	}

	void testAdd(void)
	{
		TS_ASSERT(!element.add(&chunk, NULL));

		TS_ASSERT(chunk.add(&element, ++chunk.getIterator()));
	}

	void testRemove(void)
	{
		TS_ASSERT(!element.remove(NULL));

		TS_ASSERT(chunk.remove(--chunk.getIterator()));
	}

private:
	Chunk chunk;
	Element element;
};
