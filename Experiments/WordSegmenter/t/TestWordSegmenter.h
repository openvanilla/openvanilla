#include <cxxtest/TestSuite.h>
#include "WordSegmenter.h"

#include <string>
#include <map>
#include <vector>

using namespace std;
          
class TestWordSegmenter : public CxxTest::TestSuite 
{
public:
	void setUp(void)
	{
		string lmFilePath("./as_large-kn-2.lm");		
		TS_ASSERT(seg.loadModel(lmFilePath));
	}
	
	void testSegment(void)
	{
		//string inputString("ABCDE");
		//string outputString;
		vector<Token*> inputTokens;
		Token* tokenA = new Token();
		tokenA->word = "¤U";
		inputTokens.push_back(tokenA);
		
		Token* tokenB = new Token();
		tokenB->word = "«B";
		inputTokens.push_back(tokenB);
		
		Token* tokenC = new Token();
		tokenC->word = "¤Ñ";
		inputTokens.push_back(tokenC);
		
		Token* tokenD = new Token();
		tokenD->word = "¯d";
		inputTokens.push_back(tokenD);
		
		Token* tokenE = new Token();
		tokenE->word = "«È";
		inputTokens.push_back(tokenE);
		
		///TS_ASSERT(seg.segment(inputString, outputString));
		vector<Token*> outputTokens;
		TS_ASSERT(seg.segment(inputTokens, outputTokens));
		
		string outputString;
		for(int i = 0; i < outputTokens.size(); i++)
			outputString += outputTokens[i]->word + " ";
		//TS_WARN(outputString);
		cerr<<outputString;
	}
	
private:
	WordSegmenter seg;
};