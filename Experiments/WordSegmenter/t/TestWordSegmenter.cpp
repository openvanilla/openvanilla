#include <iostream>
#include <UnitTest++.h>
#include "WordSegmenter.h"

#include <string>
#include <map>
#include <vector>

using namespace std;
using namespace UnitTest;
          
class TestWordSegmenter
{
public:
	TestWordSegmenter()
	{
		seg = new WordSegmenter();
		string lmFilePath("./as_large-kn-2.lm");		
		seg->loadModel(lmFilePath);
	}

	~TestWordSegmenter()
	{
		delete seg;
	}

	WordSegmenter* seg;
};
	
TEST_FIXTURE(TestWordSegmenter, WordSegmenterTest)
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
		
	vector<Token*> outputTokens;
	CHECK(seg->segment(inputTokens, outputTokens));
		
	string outputString("");
	for(int i = 0; i < outputTokens.size(); i++)
		outputString += outputTokens[i]->word + " ";
	cerr << outputString;
}

int main()
{
	return RunAllTests();
}
