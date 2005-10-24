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
		Bin* bin1 = new Bin();
		bin1->gram = "下";
		bin1->logprob = -3.0f;
		bin1->backoff = -1.6f;
		lm.insert(make_pair(bin1->gram, bin1));
		
		Bin* bin2 = new Bin();
		bin2->gram = "雨";
		bin2->logprob = -2.5f;
		bin2->backoff = -1.3f;
		lm.insert(make_pair(bin2->gram, bin2));
		
		Bin* bin3 = new Bin();
		bin3->gram = "天";
		bin3->logprob = -1.5f;
		bin3->backoff = -0.8f;
		lm.insert(make_pair(bin3->gram, bin3));
		
		Bin* bin4 = new Bin();
		bin4->gram = "留";
		bin4->logprob = -4.0f;
		bin4->backoff = -2.2f;
		lm.insert(make_pair(bin4->gram, bin4));
		
		Bin* bin5 = new Bin();
		bin5->gram = "客";
		bin5->logprob = -2.0f;
		bin5->backoff = -1.0f;
		lm.insert(make_pair(bin5->gram, bin5));
		
		Bin* bin12 = new Bin();
		bin12->gram = "下雨";
		bin12->logprob = -2.0f;
		bin12->backoff = -1.0f;
		lm.insert(make_pair(bin12->gram, bin12));
		
		Bin* bin23 = new Bin();
		bin23->gram = "雨天";
		bin23->logprob = -0.2f;
		bin23->backoff = -0.1f;
		lm.insert(make_pair(bin23->gram, bin23));
		
		Bin* bin45 = new Bin();
		bin45->gram = "留客";
		bin45->logprob = -1.5f;
		bin45->backoff = -0.7f;
		lm.insert(make_pair(bin45->gram, bin45));
		
		Bin* bin23_45 = new Bin();
		bin23_45->gram = "雨天 留客";
		bin23_45->logprob = -0.1f;
		bin23_45->backoff = 0.0f;
		lm.insert(make_pair(bin23_45->gram, bin23_45));
		
		Bin* unk = new Bin();
		unk->gram = "<unk>";
		unk->logprob = -5.0f;
		unk->backoff = 0.0f;
		lm.insert(make_pair(unk->gram, unk));
		
		TS_ASSERT(seg.loadModel(lm));
	}
	
	void testSegment(void)
	{
		//string inputString("ABCDE");
		//string outputString;
		vector<Token*> inputTokens;
		Token* tokenA = new Token();
		tokenA->word = "下";
		inputTokens.push_back(tokenA);
		
		Token* tokenB = new Token();
		tokenB->word = "雨";
		inputTokens.push_back(tokenB);
		
		Token* tokenC = new Token();
		tokenC->word = "天";
		inputTokens.push_back(tokenC);
		
		Token* tokenD = new Token();
		tokenD->word = "留";
		inputTokens.push_back(tokenD);
		
		Token* tokenE = new Token();
		tokenE->word = "客";
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
	map<string, Bin*> lm;
	WordSegmenter seg;
};