#include <cxxtest/TestSuite.h>
#include "KeystrokeToWordConverter.h"

#include <string>
#include <map>
#include <vector>

using namespace std;
          
class KeystrokeToWordConverterTest : public CxxTest::TestSuite 
{
public:
	KeystrokeToWordConverterTest()
	{
		string lmFilePath("../../as_large-kn-2.lm");
		TS_ASSERT(m_k2w.loadModel(lmFilePath));

		string dicFilePath("../../tsi.src");
		TS_ASSERT(m_k2w.loadDictionary(dicFilePath));
	}

	void setUp(void)
	{		
		Token* tokenA = new Token();
		tokenA->characterStringVector.push_back(
			"\xE3\x84\x92\xE3\x84\xA7\xE3\x84\x9A\xCB\x8B");
			//xia
		m_inputTokens.push_back(tokenA);
		
		Token* tokenB = new Token();
		tokenB->characterStringVector.push_back(
			"\xE3\x84\xA9\xCB\x87");
			//yv
		m_inputTokens.push_back(tokenB);
		
		Token* tokenC = new Token();
		tokenC->characterStringVector.push_back(
			"\xE3\x84\x8A\xE3\x84\xA7\xE3\x84\xA2");
			//tian
		m_inputTokens.push_back(tokenC);
		
		Token* tokenD = new Token();
		tokenD->characterStringVector.push_back(
			"\xE3\x84\x8C\xE3\x84\xA7\xE3\x84\xA1\xCB\x8A");
			//liu
		m_inputTokens.push_back(tokenD);
		
		Token* tokenE = new Token();
		tokenE->characterStringVector.push_back(
			"\xE3\x84\x8E\xE3\x84\x9C\xCB\x8B");
			//ke
		m_inputTokens.push_back(tokenE);
	}
	
	void testConvert(void)
	{
		TS_ASSERT(m_k2w.convert(m_inputTokens, m_outputTokens));
	}
	
	void testFixedToken(void)
	{
		m_inputTokens[0]->isFixed = true;
		m_inputTokens[0]->word = "\xE5\xA4\x8F"; //summer /xia/

        TS_ASSERT(m_k2w.convert(m_inputTokens, m_outputTokens));
		TS_ASSERT_EQUALS(
			m_outputTokens[0]->word.find_first_of("\xE5\xA4\x8F"),
			0);
		TS_TRACE(m_outputTokens[0]->word.c_str());
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
	KeystrokeToWordConverter m_k2w;
	vector<Token*> m_inputTokens, m_outputTokens;
};
