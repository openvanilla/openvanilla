#include <cxxtest/TestSuite.h>
#include "WordSegmenter.h"

#include <string>
#include <map>
#include <vector>

using namespace std;
          
class WordSegmenterTest : public CxxTest::TestSuite 
{
public:
	void setUp(void)
	{
		string lmFilePath("../../as_large-kn-2.lm");		
		TS_ASSERT(seg.loadModel(lmFilePath));
	}
	
	void testSegment(void)
	{
		vector<Token*> inputTokens;
		Token* tokenA = new Token();
		tokenA->word = "\xE4\xB8\x8B";
		inputTokens.push_back(tokenA);
		
		Token* tokenB = new Token();
		tokenB->word = "\xE9\x9B\xA8";
		inputTokens.push_back(tokenB);
		
		Token* tokenC = new Token();
		tokenC->word = "\xE5\xA4\xA9";
		inputTokens.push_back(tokenC);
		
		Token* tokenD = new Token();
		tokenD->word = "\xE7\x95\x99";
		inputTokens.push_back(tokenD);
		
		Token* tokenE = new Token();
		tokenE->word = "\xE5\xAE\xA2";
		inputTokens.push_back(tokenE);
		
		vector<Token*> outputTokens;
		TS_ASSERT(seg.segment(inputTokens, outputTokens));
		
		string outputString("");
		for(int i = 0; i < outputTokens.size(); i++)
			outputString += outputTokens[i]->word + " ";
		TS_TRACE(outputString);
	}
	
private:
	WordSegmenter seg;
};
