#include <cxxtest/TestSuite.h>
#include "KeystrokeToWordConverter.h"

#include <string>
#include <map>
#include <vector>

using namespace std;
          
class TestKeystrokeToWordConverter : public CxxTest::TestSuite 
{
public:
	void setUp(void)
	{
		string lmFilePath("./as_large-kn-2.lm");		
		TS_ASSERT(k2w.loadModel(lmFilePath));

		string dicFilePath("./tsi.src");
		TS_ASSERT(k2w.loadDictionary(dicFilePath));
	}
	
	void testConvert(void)
	{
		//string inputString("ABCDE");
		//string outputString;
		vector<Token*> inputTokens;
		Token* tokenA = new Token();
		tokenA->word = "ㄒㄧㄚˋ";
		inputTokens.push_back(tokenA);
		
		Token* tokenB = new Token();
		tokenB->word = "ㄩˇ";
		inputTokens.push_back(tokenB);
		
		Token* tokenC = new Token();
		tokenC->word = "ㄊㄧㄢ";
		inputTokens.push_back(tokenC);
		
		Token* tokenD = new Token();
		tokenD->word = "ㄌㄧㄡˊ";
		inputTokens.push_back(tokenD);
		
		Token* tokenE = new Token();
		tokenE->word = "ㄎㄜˋ";
		inputTokens.push_back(tokenE);
		
		vector<Token*> outputTokens;
		TS_ASSERT(k2w.convert(inputTokens, outputTokens));
		
		string outputString("");
		for(int i = 0; i < outputTokens.size(); i++)
			outputString += outputTokens[i]->word + " ";
		//TS_WARN(outputString);
		cerr << outputString;
	}
	
private:
	KeystrokeToWordConverter k2w;
};
