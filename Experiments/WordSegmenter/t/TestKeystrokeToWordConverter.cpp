#include <iostream>
#include <UnitTest++.h>
#include "KeystrokeToWordConverter.h"

#include <string>
#include <map>
#include <vector>

using namespace std;
using namespace UnitTest;
          
class TestKeystrokeToWordConverter
{
public:
	TestKeystrokeToWordConverter()
	{
		k2w = new KeystrokeToWordConverter();

		string lmFilePath("./as_large-kn-2.lm");		
		k2w->loadModel(lmFilePath);

		string dicFilePath("./tsi.src");
		k2w->loadDictionary(dicFilePath);
	}

	~TestKeystrokeToWordConverter()
	{
		delete k2w;
	}
	
	KeystrokeToWordConverter* k2w;
};
	
TEST_FIXTURE (TestKeystrokeToWordConverter, ConvertKeystrokeToWordTest)
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
	CHECK(k2w->convert(inputTokens, outputTokens));
	
	string outputString("");
	for(int i = 0; i < outputTokens.size(); i++)
		outputString += outputTokens[i]->word + " ";
	cerr << outputString;
}

int main()
{
        return RunAllTests();
}
