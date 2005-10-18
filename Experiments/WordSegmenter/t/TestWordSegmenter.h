#include <cxxtest/TestSuite.h>
#include "WordSegmenter.h"

#include <string>
#include <map>

using namespace std;
          
class TestWordSegmenter : public CxxTest::TestSuite 
{
public:
	void setUp(void)
	{
		string word1("A");
		lm.insert(make_pair(word1, 10));
		string word2("B");
		lm.insert(make_pair(word2, 20));
		string word3("C");
		lm.insert(make_pair(word3, 20));
		string word4("D");
		lm.insert(make_pair(word4, 15));
		string word5("E");
		lm.insert(make_pair(word5, 60));
		string word6("F");
		lm.insert(make_pair(word6, 60));
		
		string word12("AB");
		lm.insert(make_pair(word12, 35));
		string word45("DE");
		lm.insert(make_pair(word45, 80));
		string word56("DF");
		lm.insert(make_pair(word56, 9));
		
		string word123("ABC");
		lm.insert(make_pair(word123, 6));
		string word456("DEF");
		lm.insert(make_pair(word456, 160));
		
		
		string bigram12_3("DE F");
		lm.insert(make_pair(bigram12_3, 1));
		string bigram123_456("ABC DEF");
		lm.insert(make_pair(bigram123_456, 5));

		TS_ASSERT(seg.loadModel(lm));
	}
	
	void testSegment(void)
	{
		string inputString("ABCDEF");
		string outputString;
		TS_ASSERT(seg.segment(inputString, outputString));
		TS_WARN(outputString);
	}
	
private:
	map<string, int> lm;
	WordSegmenter seg;
};