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
		m_element = new Element();
		m_element->setView(keystroke);
		
		string word("A");
		m_chunk = new Chunk();
		m_chunk->setView(word);
		
		string phrase("a->A");
		m_sentence = new Chunk();
		m_sentence->setView(phrase);
	}
	
	void tearDown(void)
	{
		delete m_sentence;
	}

	void testGetView(void)
	{
		TS_ASSERT_SAME_DATA(m_element->getView().c_str(), "a", 1);
		TS_ASSERT_SAME_DATA(m_chunk->getView().c_str(), "A", 1);
		TS_ASSERT_SAME_DATA(m_sentence->getView().c_str(), "a->A", 4);
	}

	void testAdd(void)
	{
		vector<Sequence*>::iterator iter;
		
		iter = m_chunk->getIterator();
		TS_ASSERT_THROWS_ANYTHING(m_element->add(m_chunk, iter));
		TS_ASSERT_THROWS_NOTHING(m_chunk->add(m_element, iter));
		
		iter = m_sentence->getIterator();
		TS_ASSERT_THROWS_NOTHING(m_sentence->add(m_chunk, iter));
		
		m_element1 = new Element();
		string temp("B");
		m_element1->setView(temp);
		iter = m_sentence->getIterator();	// Initial again or die.
		++iter;
		TS_ASSERT_THROWS_NOTHING(m_sentence->add(m_element1, iter));
	}

	void testRemove(void)
	{
		testAdd();
		vector<Sequence*>::iterator iter = m_chunk->getIterator();

		TS_ASSERT_THROWS_ANYTHING(m_element->remove(iter));
		TS_ASSERT_THROWS_NOTHING(m_chunk->remove(iter));
		
		iter = m_sentence->getIterator();
		TS_ASSERT_THROWS_NOTHING(m_sentence->remove(iter));
	}

	void testAccept(void)
	{
		testAdd();

		SequenceVisitor m_visitor;
		m_sentence->accept(m_visitor);

		TS_ASSERT_SAME_DATA(m_visitor.getElementViews().c_str(), "aB", 2);
		TS_ASSERT_SAME_DATA(m_visitor.getChunkViews().c_str(), "Aa->A", 5);
	}

private:
	Element *m_element, *m_element1;
	Chunk* m_chunk;
	Chunk* m_sentence;
};
