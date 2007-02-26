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
	
		//string inputString("ABCDE");
		//string outputString;
		Token* tokenA = new Token();
		tokenA->characterStringVector.push_back("ㄒㄧㄚˋ");
		inputTokens.push_back(tokenA);
		
		Token* tokenB = new Token();
		tokenB->characterStringVector.push_back("ㄩˇ");
		inputTokens.push_back(tokenB);
	
		Token* tokenC = new Token();
		tokenC->characterStringVector.push_back("ㄊㄧㄢ");
		inputTokens.push_back(tokenC);
		
		Token* tokenD = new Token();
		tokenD->characterStringVector.push_back("ㄌㄧㄡˊ");
		inputTokens.push_back(tokenD);
		
		Token* tokenE = new Token();
		tokenE->characterStringVector.push_back("ㄎㄜˋ");
		inputTokens.push_back(tokenE);
	}
       
	~TestKeystrokeToWordConverter()
        {
                delete k2w;
        }

        KeystrokeToWordConverter* k2w;
	vector<Token*> inputTokens;
};

TEST_FIXTURE(TestKeystrokeToWordConverter, ConvertTest)
{
	vector<Token*> outputTokens;
	CHECK(k2w->convert(inputTokens, outputTokens));
	
	string outputString("");
	for(int i = 0; i < outputTokens.size(); i++)
		outputString += outputTokens[i]->word + " ";
	cerr << outputString;
}

TEST_FIXTURE(TestKeystrokeToWordConverter, ConvertWithFixedTokenTest)
{
	inputTokens[0]->isFixed = true;
	inputTokens[0]->word = "夏";

        vector<Token*> outputTokens;
        CHECK(k2w->convert(inputTokens, outputTokens));

        string outputString("");
        for(int i = 0; i < outputTokens.size(); i++)
                outputString += outputTokens[i]->word + " ";
        cerr << outputString;

	CHECK(outputString.find("夏") == 0);
}

int main()
{
        return RunAllTests();
}
