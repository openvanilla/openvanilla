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
		
		Token* tokenA = new Token();
		tokenA->word = "ㄒㄧㄚˋ";
		m_inputTokens.push_back(tokenA);
		
		Token* tokenB = new Token();
		tokenB->word = "ㄩˇ";
		m_inputTokens.push_back(tokenB);
		
		Token* tokenC = new Token();
		tokenC->word = "ㄊㄧㄢ";
		m_inputTokens.push_back(tokenC);
		
		Token* tokenD = new Token();
		tokenD->word = "ㄌㄧㄡˊ";
		m_inputTokens.push_back(tokenD);
		
		Token* tokenE = new Token();
		tokenE->word = "ㄎㄜˋ";
		m_inputTokens.push_back(tokenE);
	}
	
	void testConvert(void)
	{
		TS_ASSERT(k2w.convert(m_inputTokens, m_outputTokens));
	}
	
	void testFixedToken(void)
	{
		m_inputTokens[0]->isFixed = true;
		m_inputTokens[0]->word = "夏";

        TS_ASSERT(k2w->convert(m_inputTokens, m_outputTokens));
		TS_ASSERT_SAME_DATA(m_outputTokens[0]->word.c_str(), "夏");
	}

	void tearDown(void)
	{
		string outputString("");
		for(int i = 0; i < m_outputTokens.size(); i++)
			outputString += m_outputTokens[i]->word + " ";
		TS_TRACE(outputString);
		
		m_inputTokens.clear();
		m_outputTokens.clear();
	}

private:
	KeystrokeToWordConverter k2w;
	vector<Token*> m_inputTokens, m_outputTokens;
};
