#include <iostream>
#include <UnitTest++.h>
#include "LanguageModel.h"

using namespace std;
using namespace UnitTest;

class TestLanguageModel
{
public:
	TestLanguageModel()
	{
		string lmFilePath("./as_large-kn-2.lm");
		lm_ = new LanguageModel(lmFilePath);
	}
	
        ~TestLanguageModel() 
        {
                delete lm_; 
        }       

        LanguageModel* lm_; 
};              

TEST_FIXTURE(TestLanguageModel, GetLogProbLanguageModelTest)
{
	string gram = "下雨 天";
	cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
	gram = "下雨天";
	cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
	gram = "下雨";
	cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
	gram = "雨天";
	cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
	gram = "下";
	cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
	gram = "雨";
	cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
		
	gram = "天";
	cerr << gram << " logprob: " << lm_->getLogProb(gram) << endl;
}
	
TEST_FIXTURE(TestLanguageModel, GetBackOffLanguageModelTest)
{
	string gram = "下雨 天";
	cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
	gram = "下雨天";
	cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
	gram = "下雨";
	cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
	gram = "雨天";
	cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
	gram = "下";
	cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
	gram = "雨";
	cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
		
	gram = "天";
	cerr << gram << " back-off: " << lm_->getBackOff(gram) << endl;
}

int main()
{
	return RunAllTests();
}

