#include <cxxtest/TestSuite.h>
#include "Sequence.h"

#include <string>
#include <vector>

using namespace std;
          
class TestSequence : public CxxTest::TestSuite 
{
public:
	void setUp(void)
	{
		string keystroke("a");
		m_element.setView(keystroke);

		m_iter = m_chunk.getIterator();
		m_chunk.add(&m_element, m_iter);

		string word("apple");		
		m_chunk.setView(word);
	}

	void testGetView(void)
	{
		TS_ASSERT_SAME_DATA(m_element.getView().c_str(), "a", 1);

		TS_ASSERT_SAME_DATA(m_chunk.getView().c_str(), "apple", 5);
	}

	void testAdd(void)
	{
		TS_ASSERT_THROWS_ANYTHING(m_element.add(&m_chunk, NULL));

		++m_iter;
		TS_ASSERT_THROWS_NOTHING(m_chunk.add(&m_element, m_iter));
	}

	void testRemove(void)
	{
		TS_ASSERT_THROWS_ANYTHING(m_element.remove(NULL));

		TS_ASSERT_THROWS_NOTHING(m_chunk.remove(m_iter));
		--m_iter;
	}

	void testAccept(void)
	{
		m_chunk.accept(m_visitor);
		TS_ASSERT_SAME_DATA(m_visitor.getElementViews().c_str(), "a", 1);
		TS_ASSERT_SAME_DATA(m_visitor.getChunkViews().c_str(), "apple", 5);
	}

private:
	Chunk m_chunk;
	Element m_element;
	
	vector<Sequence*>::iterator m_iter;
	SequenceVisitor m_visitor;
};
